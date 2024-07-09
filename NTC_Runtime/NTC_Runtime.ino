/*
    Messung der Laufzeit für Abruf eines Messwertes nach Polynomapproximation und ANN
    Approximationspolynom: T = 1.2521U4 - 15.664U3 + 67.375U2 - 135.54U + 140.17
    Laufzeiten ermittelt auf einem XIAO RP2040 - Arduino 2.1.1
    2024-02-17 Claus Kühnel info@ckuehnel.ch
*/

/*  Autor: Josef Bernhardt  IDE: Arduino 1.8.20  Date: 12.2.2024  Arduino Software to Read NTC 10 K Sensor with a trained neuronal net   */ 

#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>  

#define MAX_INPUT_NEURONS 1 
#define MAX_HIDDEN_NEURONS 9 
#define MAX_OUTPUT_NEURONS 1  

#define nil ((float*)0)

// "The values in xs must be in ascending order in the array."
float xs[] = {0.164, 0.186, 0.241, 0.316, 0.417, 0.554, 0.740, 0.989, 1.320, 1.731, 2.227, 2.5, 2.778, 3.333, 3.836, 4.244, 4.542, 4.652};
float ys[] = {125.000, 120.000, 110.000, 100.000, 90.000, 80.000, 70.000, 60.000, 50.000, 40.000, 30.000, 25.000, 20.000, 10.000, 0.000, -10.000, -20.000, -25.000};
float hx[20], hy[20], M[20];
int n = 18;

typedef struct 
{     
  float inputToHiddenWeights[MAX_INPUT_NEURONS][MAX_HIDDEN_NEURONS];     
  float hiddenToOutputWeights[MAX_HIDDEN_NEURONS][MAX_OUTPUT_NEURONS]; 
} NeuralNetwork;   

NeuralNetwork network;  

float tanh_activation(float x) 
{     
  return tanh(x); 
}

// Hidden layer Values for NTC 10K  
void initializeNetworkWithTrainedValues(NeuralNetwork *network) 
{     
network->inputToHiddenWeights[0][0]= -0.2826134;
network->inputToHiddenWeights[0][1]= 2.6443331;
network->inputToHiddenWeights[0][2]= -2.6443331;
network->inputToHiddenWeights[0][3]= 2.5296659;
network->inputToHiddenWeights[0][4]= 0.0965367;
network->inputToHiddenWeights[0][5]= 0.0965367;
network->inputToHiddenWeights[0][6]= -6.7455325;
network->inputToHiddenWeights[0][7]= 2.6329384;
network->inputToHiddenWeights[0][8]= 0.0959342;

network->hiddenToOutputWeights[0][0]= -0.3132751;
network->hiddenToOutputWeights[1][0]= -1.7767541;
network->hiddenToOutputWeights[2][0]= 1.7767541;
network->hiddenToOutputWeights[3][0]= 0.1200805;
network->hiddenToOutputWeights[4][0]= -1.1013447;
network->hiddenToOutputWeights[5][0]= -1.1013447;
network->hiddenToOutputWeights[6][0]= -6.0046711;
network->hiddenToOutputWeights[7][0]= -1.7639061;
network->hiddenToOutputWeights[8][0]= -0.8765046; 
}  

void forwardPropagation(NeuralNetwork *network, float input[MAX_INPUT_NEURONS], float hidden[MAX_HIDDEN_NEURONS], float output[MAX_OUTPUT_NEURONS]) 
{     
  for (int j = 0; j < MAX_HIDDEN_NEURONS; ++j) 
  {         
    float netInput = 0.0;         
    for (int i = 0; i < MAX_INPUT_NEURONS; ++i) 
    {             
      netInput += input[i] * network->inputToHiddenWeights[i][j];         
    }         
    hidden[j] = tanh_activation(netInput);     
  }      
  for (int k = 0; k < MAX_OUTPUT_NEURONS; ++k) 
  {         
    float netInput = 0.0;         
    for (int j = 0; j < MAX_HIDDEN_NEURONS; ++j) 
    {             
      netInput += hidden[j] * network->hiddenToOutputWeights[j][k];         
    }         
    output[k] = tanh_activation(netInput);     
  }
}  

float NetworkOutput(NeuralNetwork *network, float inputPatterns) 
{     
  float input = inputPatterns;     
  float hidden[MAX_HIDDEN_NEURONS];    
  float output[MAX_OUTPUT_NEURONS];     
  forwardPropagation(network, &input, hidden, output);     
  return output[0] * 100.0; 
}  

float calculate_temp_poly(float U)
{
  // 1.2521U4 - 15.664U3 + 67.375U2 - 135.54U + 140.17
  return (1.2521*U*U*U*U - 15.664*U*U*U + 67.375*U*U - 135.54*U + 140.17);
}

void setup() 
{     
  Serial.begin(115200); 
  while(!Serial);    
  
  unsigned long time = millis();
  spline_natural_init(n, xs, ys, hx, hy, M);
  Serial.printf("Calculation of Spline Coefficients = %f ms\n", millis() - time);

  Serial.println("Spline Values hx hy M:");
  for (int i = 0; i < 20; i++) 
  {
    Serial.print(hx[i]);
    Serial.print(" ");
    Serial.print(hy[i]);
    Serial.print(" ");
    Serial.println(M[i]);
  }

  // initialize digital pin LED_BUILTIN as an output.     
  pinMode(LED_BUILTIN, OUTPUT);     
  initializeNetworkWithTrainedValues(&network);     
}   

void loop() 
{        
  float temp1, temp2, temp3;
  float Voltage;

  for (int i = 0; i < 5000; i += 100) 
  {
    Voltage = (float)i / 1000.0;
        
    // float voltage = analogRead(A0) * (5.0 / 1023.0);

    // Check if Voltage is out of bounds
    if (Voltage > 0.164 && Voltage < 4.652) 
    {
      unsigned long ts = micros();
      temp1 = NetworkOutput(&network, Voltage); 
      unsigned long ts1 = micros() - ts;
      ts = micros();      
      temp2 = calculate_temp_poly(Voltage); 
      unsigned long ts2 = micros() - ts;
      ts = micros();
      temp3 = spline_value(n - 1, xs, ys, hx, hy, M, Voltage);
      unsigned long ts3 = micros()-ts;
      Serial.printf("Voltage = %3.3f V\tANN Temp = %7.3f °C\tRuntime = %3d us\t\tPOLY Temp = %7.3f °C\tRuntime = %3d us\t\tSPLINE Temp = %7.3f °C\tRuntime = %3d us \n", Voltage, temp1, ts1, temp2, ts2, temp3, ts3); 
    }
  }
    Serial.println("--------------------------------------------------");
    delay(5000); // Wait 1000 ms 
} 