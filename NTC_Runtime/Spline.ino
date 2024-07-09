
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