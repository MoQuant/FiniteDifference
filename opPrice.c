#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double Solve(double S0, double S[], double V[], int n){
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

    double price = V1 + (S0 - S1)*(V2 - V1)/(S2 - S1);
    return price;
}

int main()
{
    double S0 = 421.53;
    double K = 460.00;
    double T = 1.0;
    double r = 0.051;
    double sigma = 0.185;
    double S_max = 2.0*K;

    int N = 50;
    int M = 100;

    double dS = S_max / (double) N;
    double dt = T / (double) M;

    double V[N+1][M+1];
    double S[N+1];

    double alpha[N+1];
    double beta[N+1];
    double gamma[N+1];

    for(int i = 0; i < N+1; ++i){
        for(int j = 0; j < M+1; ++j){
            V[i][j] = 0.0;
        }
        S[i] = i*dS;
        V[i][M] = fmax(S[i] - K, 0.0);

        alpha[i] = 0.5*dt*(pow(sigma, 2)*pow(i, 2) - r*i);
        beta[i] = 1.0 - dt*(pow(sigma, 2)*pow(i, 2) + r);
        gamma[i] = 0.5*dt*(pow(sigma, 2)*pow(i, 2) + r*i);
    }

    for(int j = 0; j < M+1; ++j){
        V[0][j] = 0.0;
        V[N][j] = S_max - K*exp(-r*dt*j);
    }

    for(int j = M - 1; j >= 0; --j){
        for(int i = 1; i < N; ++i){
            V[i][j] = alpha[i]*V[i-1][j+1] + beta[i]*V[i][j+1] + gamma[i]*V[i+1][j+1];
            V[i][j] = fmax(V[i][j], S[i] - K);
        }
    }

    double VX[N+1];
    for(int i = 0; i < N+1; ++i){
        VX[i] = V[i][0];
    }

    double optionPrice = Solve(S0, S, VX, N+1);

    printf("C | Option Price: %f\n", optionPrice);

    return 0;
}