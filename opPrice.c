#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double Interpolate(double S0, double S[], double V[], int n){
    int i = 0;
    for(i = 0; i < n - 1; ++i){
        if(S0 >= S[i] && S0 <= S[i + 1]){
            break;
        }
    }

    if(i > n - 1){
        return V[n - 1];
    }

    double S1 = S[i];
    double S2 = S[i + 1];
    double V1 = V[i];
    double V2 = V[i + 1];

    return V1 + (S0 - S1)*(V2 - V1)/(S2 - S1);
}

double OptionPrice(double S0, double K, double T, double r, double sigma, int N, int M)
{
    double S_max = 2.0*K;

    double dS = S_max / (double) N;
    double dT = T / (double) M;

    double V[N+1][M+1];
    double S[N+1];

    double alpha[N+1];
    double beta[N+1];
    double gamma[N+1];

    for(int i = 0; i < N + 1; ++i){
        for(int j = 0; j < M + 1; ++j){
            V[i][j] = 0.0;
        }
        S[i] = i*dS;
        V[i][M] = fmax(S[i] - K, 0.0);

        alpha[i] = 0.5*dT*(pow(sigma, 2)*pow(i, 2) - r*i);
        beta[i] = 1.0 - dT*(pow(sigma, 2)*pow(i, 2) + r);
        gamma[i] = 0.5*dT*(pow(sigma, 2)*pow(i, 2) + r*i);
    }

    for(int j = 0; j < M + 1; ++j){
        V[0][j] = 0.0;
        V[N][j] = S_max - K*exp(-r*dT*j);
    }

    for(int j = M - 1; j >= 0; --j){
        for(int i = 1; i < N; ++i){
            V[i][j] = alpha[i]*V[i-1][j+1] + beta[i]*V[i][j+1] + gamma[i]*V[i+1][j+1];
            V[i][j] = fmax(V[i][j], S[i] - K);
        }
    }

    double VX[N+1];
    for(int i = 0; i < N + 1; ++i){
        VX[i] = V[i][0];
    }

    double opPrice = Interpolate(S0, S, VX, N+1);

    printf("C | American Call Option Price: %f\n", opPrice);

    return 0.0;
}


int main()
{
    double S0 = 421.53;
    double K = 460.00;
    double T = 0.25;
    double r = 0.051;
    double sigma = 0.185;
    int N = 50;
    int M = 100;
    
    OptionPrice(S0, K, T, r, sigma, N, M);

}