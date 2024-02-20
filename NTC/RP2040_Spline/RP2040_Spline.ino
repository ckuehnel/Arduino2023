/*
  File:RP2040_Spline.ino

  Cubic Spline Appoximation tested on RP2040
*/
#include <stdio.h>
#include <stdlib.h>

#define nil ((float*)0)

// "The values in xs must be in ascending order in the array."
float xs[] = {0.164, 0.186, 0.241, 0.316, 0.417, 0.554, 0.740, 0.989, 1.320, 1.731, 2.227, 2.5, 2.778, 3.333, 3.836, 4.244, 4.542, 4.652};
float ys[] = {125.000, 120.000, 110.000, 100.000, 90.000, 80.000, 70.000, 60.000, 50.000, 40.000, 30.000, 25.000, 20.000, 10.000, 0.000, -10.000, -20.000, -25.000};
float hx[20], hy[20], M[20];
int n = 18;

int spline_natural_init(int n, float *x, float *y, float *hx, float *hy, float *M) 
{
    int i;
    float *mue, *z, *lambda, *d;
    mue = (float *)malloc((n + 1) * sizeof(float));
    z = (float *)malloc((n + 1) * sizeof(float));
    lambda = (float *)malloc((n + 1) * sizeof(float));
    d = (float *)malloc((n + 1) * sizeof(float));
    if (mue == nil || z == nil || lambda == nil || d == nil)
        return 0;

    for (i = 1; i <= n; i++) 
    {
        hx[i] = x[i] - x[i - 1];
        hy[i] = y[i] - y[i - 1];
    }

    z[0] = 2;
    lambda[0] = 0;
    d[0] = 0;

    for (i = 1; i < n; i++) 
    {
        mue[i] = hx[i] / (hx[i] + hx[i + 1]);
        z[i] = 2;
        lambda[i] = hx[i + 1] / (hx[i] + hx[i + 1]);
        d[i] = 6 / (hx[i] + hx[i + 1]) * (hy[i + 1] / hx[i + 1] - hy[i] / hx[i]);
    }

    mue[n] = 0;
    z[n] = 2;
    d[n] = 0;

    for (i = 1; i <= n; i++) 
    {
        z[i] = z[i] - mue[i] / z[i - 1] * lambda[i - 1];
        d[i] = d[i] - mue[i] / z[i - 1] * d[i - 1];
    }

    M[n] = d[n] / z[n];
    for (i = n - 1; i >= 0; i--) 
    {
        M[i] = (d[i] - lambda[i] * M[i + 1]) / z[i];
    }

    free(mue);
    free(z);
    free(lambda);
    free(d);
    return 1;
}

float spline_value(int n, float *x, float *y, float *hx, float *hy, float *M, float x0) 
{
    int j;
    float A1, A2, A3, y0;

    j = 0;
    while (j < n - 1 && x0 > x[j + 1])
        j++;

    A1 = hy[j + 1] / hx[j + 1] - hx[j + 1] / 6 * (2 * M[j] + M[j + 1]);
    A2 = M[j] / 2;
    A3 = (M[j + 1] - M[j]) / 6 / hx[j + 1];

    y0 = A3;
    y0 = A2 + (x0 - x[j]) * y0;
    y0 = A1 + (x0 - x[j]) * y0;
    y0 = y[j] + (x0 - x[j]) * y0;

    return y0;
}

void setup() 
{
    Serial.begin(115200);
    while(!Serial);
    unsigned long time = millis();
    spline_natural_init(n, xs, ys, hx, hy, M);
    Serial.printf("Calculation of Spline Coefficients = %f ms\n", millis() - time);

    Serial.println("Spline Values hx hy M:");
    for (int i = 0; i < 20; i++) {
        Serial.print(hx[i]);
        Serial.print(" ");
        Serial.print(hy[i]);
        Serial.print(" ");
        Serial.println(M[i]);
    }

    Serial.println("Spline Test Example:");
}

void loop() 
{
    float Temperature;
    float Voltage;

    for (int i = 0; i < 5000; i += 100) 
    {
        Voltage = (float)i / 1000.0;
        
        // float voltage = analogRead(A0) * (5.0 / 1023.0);

        // Check if Voltage is out of bounds
        if (Voltage > 0.164 && Voltage < 4.652) 
        {
          unsigned long ts = micros();
          Temperature = spline_value(n - 1, xs, ys, hx, hy, M, Voltage);
          ts = micros()-ts;
          Serial.printf("Voltage = %.3f V\t Temperature = %.1f °C\t Runtime = %u us\n", Voltage, Temperature, ts);
        }
    }

    delay(5000);
}
