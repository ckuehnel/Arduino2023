/*
Neuronal Net Example with Codeblocks in Ansi C for function approximation
of a NTC 10K Sensor
Josef Bernhardt Bad Abbach
Date 30.1.2024
This example utilizes a simple two-layer network architecture with a
hidden layer and the TANH activation function.
The trainNetwork function implements the backpropagation algorithm
for a simple function with 14 input values and 14 output values.
 Please note that the learning rate (0.0001) and other parameters
 can be adjusted according to the application's requirements.
Example Values from NTC 10 3950 Sensor
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_PATTERNS 18
#define MAX_INPUT_NEURONS 1
#define MAX_HIDDEN_NEURONS 9
#define MAX_OUTPUT_NEURONS 1
#define MAX_EPOCHS 10000000
#define LEARNINGRATE 0.0001
#define ERRORTHRESHOLD 0.05

typedef struct
{
 // Matrix of weights connecting input neurons to hidden neurons
 float inputToHiddenWeights[MAX_INPUT_NEURONS][MAX_HIDDEN_NEURONS];
 // Matrix of weights connecting hidden neurons to output neurons
 float hiddenToOutputWeights[MAX_HIDDEN_NEURONS][MAX_OUTPUT_NEURONS];
} NeuralNetwork;

// Function to initialize the weights of a neural network
void initializeNetwork(NeuralNetwork *network)
{
 // Initialize weights from input neurons to hidden neurons
 for (int i = 0; i < MAX_INPUT_NEURONS; ++i) {
 for (int j = 0; j < MAX_HIDDEN_NEURONS; ++j) {
 // Set weight to a random value between -0.5 and 0.5
 network->inputToHiddenWeights[i][j] = ((float)rand() / RAND_MAX) - 0.5;
 }
 }
 // Initialize weights from hidden neurons to output neurons
 for (int i = 0; i < MAX_HIDDEN_NEURONS; ++i) {
 for (int j = 0; j < MAX_OUTPUT_NEURONS; ++j) {
 // Set weight to a random value between -0.5 and 0.5
 network->hiddenToOutputWeights[i][j] = ((float)rand() / RAND_MAX) -
0.5;
 }
 }
}

// Function to initialize the weights of a neural network
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

void DisplayWeights(NeuralNetwork *network)
{
 for (int i = 0; i < MAX_INPUT_NEURONS; ++i)
    {
 for (int j = 0; j < MAX_HIDDEN_NEURONS; ++j)
 {
 printf("network->inputToHiddenWeights[%d][%d]= %.7f;\n",i,j,network->inputToHiddenWeights[i][j]);
 }
 }
 printf("\n");
 for (int i = 0; i < MAX_HIDDEN_NEURONS; ++i)
    {
 for (int j = 0; j < MAX_OUTPUT_NEURONS; ++j)
 {
 printf("network->hiddenToOutputWeights[%d][%d]= %.7f;\n",i,j,network->hiddenToOutputWeights[i][j]);
 }
 }
}
float tanh_activation(float x)
{
 // Compute the hyperbolic tangent (tanh) activation function for the given input x
 return tanh(x);
}
float sigmoid(float x)
{
 return 1.0 / (1.0 + exp(-x));
}

void forwardPropagation(NeuralNetwork *network, float input[MAX_INPUT_NEURONS], float hidden[MAX_HIDDEN_NEURONS], float output[MAX_OUTPUT_NEURONS])
{
 // Forward propagation from input layer to hidden layer
 // Iterate over each hidden neuron
 for (int j = 0; j < MAX_HIDDEN_NEURONS; ++j)
    {
 // Initialize net input for the current hidden neuron
 float netInput = 0.0;
 // Calculate the weighted sum of inputs from input neurons
 for (int i = 0; i < MAX_INPUT_NEURONS; ++i)
 {
 netInput += input[i] * network->inputToHiddenWeights[i][j];
 }
 // Apply activation function (tanh) to the net input and store the result in the hidden layer
 hidden[j] = tanh_activation(netInput);
 }
 // Forward propagation from hidden layer to output layer
 // Iterate over each output neuron
 for (int k = 0; k < MAX_OUTPUT_NEURONS; ++k) {
 // Initialize net input for the current output neuron
 float netInput = 0.0;
 // Calculate the weighted sum of inputs from hidden neurons
 for (int j = 0; j < MAX_HIDDEN_NEURONS; ++j) {
 netInput += hidden[j] * network->hiddenToOutputWeights[j][k];
 }
 // Apply activation function (tanh) to the net input and store the result in the output layer
 output[k] = tanh_activation(netInput);
 }
}

void backPropagation(NeuralNetwork *network, float input[MAX_INPUT_NEURONS], float hidden[MAX_HIDDEN_NEURONS], float output[MAX_OUTPUT_NEURONS], float targetOutput[MAX_OUTPUT_NEURONS])
{
 // Backpropagation for output layer
 // Iterate over each output neuron
 for (int k = 0; k < MAX_OUTPUT_NEURONS; ++k)
    {
 // Calculate the error between the actual output and the target output
 float error = targetOutput[k] - output[k];
 // Calculate the gradient of the output neuron's activation function
 float outputGradient = 1.0 - output[k] * output[k];
 // Update weights connecting hidden layer to output layer
 for (int j = 0; j < MAX_HIDDEN_NEURONS; ++j)
 {
 // Update weight using the gradient descent algorithm
 network->hiddenToOutputWeights[j][k] += LEARNINGRATE * error *
outputGradient * hidden[j];
 }
 }
 // Backpropagation for hidden layer
 // Iterate over each hidden neuron
 for (int j = 0; j < MAX_HIDDEN_NEURONS; ++j) {
 // Initialize hidden layer error
 float hiddenError = 0.0;
 // Calculate error contributed by each output neuron
 for (int k = 0; k < MAX_OUTPUT_NEURONS; ++k) {
 hiddenError += (targetOutput[k] - output[k]) * (1.0 - output[k] *
output[k]) * network->hiddenToOutputWeights[j][k];
 }
 // Calculate the gradient of the hidden neuron's activation function
 float hiddenGradient = 1.0 - hidden[j] * hidden[j];
 // Update weights connecting input layer to hidden layer
 for (int i = 0; i < MAX_INPUT_NEURONS; ++i) {
 // Update weight using the gradient descent algorithm
 network->inputToHiddenWeights[i][j] += LEARNINGRATE * hiddenError *
hiddenGradient * input[i];
 }
 }
}

void trainNetwork(NeuralNetwork *network, float trainingPatterns[MAX_PATTERNS][MAX_INPUT_NEURONS], float targetOutputs[MAX_PATTERNS][MAX_OUTPUT_NEURONS])
{
 // Define maximum number of epochs and error threshold
 int maxEpochs = MAX_EPOCHS;
 float errorThreshold = ERRORTHRESHOLD;
 // Iterate over each epoch
 for (int epoch = 0; epoch < maxEpochs; ++epoch)
    {
 // Initialize total error for the epoch
 float totalError = 0.0;
 // Iterate over each training pattern
 for (int p = 0; p < MAX_PATTERNS; ++p)
 {
 // Initialize arrays to store input, hidden, and output values for the current pattern
 float input[MAX_INPUT_NEURONS];
 float hidden[MAX_HIDDEN_NEURONS];
 float output[MAX_OUTPUT_NEURONS];
 // Copy input values from the training patterns
 for (int i = 0; i < MAX_INPUT_NEURONS; ++i)
    {
 input[i] = trainingPatterns[p][i];
 }
 // Perform forward propagation to calculate the network's output for the current pattern
 forwardPropagation(network, input, hidden, output);
 // Calculate the error for the current pattern using mean squared error
 float patternError = 0.0;
 for (int k = 0; k < MAX_OUTPUT_NEURONS; ++k) {
 patternError += 0.5 * pow(targetOutputs[p][k] - output[k], 2);
 }
 // Add pattern error to the total error for the epoch
 totalError += patternError;
 // Perform backpropagation to update weights based on the error
 backPropagation(network, input, hidden, output, targetOutputs[p]);
 }
 // Check if total error is below the error threshold
 if (totalError < errorThreshold)
    {
 printf("Training successful! Epoch: %d, Error: %.8f\n", epoch + 1,
totalError);
 // Break out of the training loop if training is successful
 break;
 }
 // Print epoch and total error every 10000 epochs
 if (epoch % 100000 == 0) {
 printf("Epoch: %d, Error: %.8f\n", epoch + 1, totalError);
 }
 }
}

/*
* This function tests the neural network by computing the output for a set of
input patterns and printing it.
*
* Parameters:
* - network: A pointer to the neural network to be tested.
* - inputPatterns: A 2D array of input patterns, where each row represents a
pattern and each column represents an input value.
* The size of the array is MAX_PATTERNS x MAX_INPUT_NEURONS.
*/
void testNetwork(NeuralNetwork *network, float inputPatterns[MAX_PATTERNS][MAX_INPUT_NEURONS])
{
 for (int p = 0; p < MAX_PATTERNS; ++p)
    {
 float input[MAX_INPUT_NEURONS];
 float hidden[MAX_HIDDEN_NEURONS];
 float output[MAX_OUTPUT_NEURONS];
 // Copy input values for the current pattern from the inputPatterns array
 for (int i = 0; i < MAX_INPUT_NEURONS; ++i)
 {
 input[i] = inputPatterns[p][i];
 }
 forwardPropagation(network, input, hidden, output);
 printf("Input: %.3f, Output: %.7f\n", input[0], output[0]);
 }
}

void testNetworkOneInput(NeuralNetwork *network, float inputPatterns)
{
 // Store the single input pattern
 float input = inputPatterns;
 // Arrays to store hidden and output neuron values
 float hidden[MAX_HIDDEN_NEURONS];
 float output[MAX_OUTPUT_NEURONS];
 // Perform forward propagation to calculate the output based on the input
 forwardPropagation(network, &input, hidden, output);
 // Print the input and scaled output value (assuming output represents a percentage)
 printf("IN: %.5f, OUT: %.2f\n", input, output[0]*100);
}

int main()
{
 NeuralNetwork network;
 float trainingPatterns[MAX_PATTERNS][MAX_INPUT_NEURONS] =
 { {4.652} , {4.542} , {4.244} , {3.836} , {3.333} , {2.778} , {2.5 } ,{2.227} , {1.731} , {1.320} , {0.989} , {0.740} , {0.554} , {0.417} , {0.316} , {0.241} , {0.186} , {0.164} };
 float targetOutputs[MAX_PATTERNS][MAX_OUTPUT_NEURONS] =
 { {-0.25}, {-0.20}, {-0.10}, {0.0}, {0.10}, {0.20}, {0.25}, {0.30}, {0.40}, {0.50}, {0.60}, {0.70}, {0.80}, {0.90}, {1.0} , {1.1}, {1.2} , {1.25} };
 
 initializeNetwork(&network);
 // initializeNetworkWithTrainedValues(&network);
 printf("Before training:\n");
 testNetwork(&network, trainingPatterns);
 trainNetwork(&network, trainingPatterns, targetOutputs);
 printf("\nAfter training:\n");
 testNetwork(&network, trainingPatterns);
 for (float x= 0.0; x < 4.9; x+=0.1)
 {
 testNetworkOneInput(&network, x);
 }
 DisplayWeights(&network);
 return 0;
}
 