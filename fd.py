import numpy as np

# Parameters
S0 = 421.53        # Initial asset price
K = 460.00         # Strike price
T = 1.0        # Time to maturity (1 year)
r = 0.051       # Risk-free interest rate
sigma = 0.185   # Volatility
N = 50        # Number of asset price steps
S_max = 2 * K  # Maximum asset price considered

# Grid setup
dS = S_max / N
M = 100
dt = T / M

# Initialize the option price grid
V = np.zeros((N+1, M+1))

# Asset prices at the grid points
S = np.linspace(0, S_max, N+1)

# Payoff at maturity (American call option)
V[:, -1] = np.maximum(S - K, 0)

# Boundary conditions
V[0, :] = 0  # Call option is worthless when S=0
V[-1, :] = S_max - K * np.exp(-r * dt * np.arange(M+1))  # Large S approximation

# Finite difference coefficients
alpha = 0.5 * dt * (sigma**2 * np.arange(N+1)**2 - r * np.arange(N+1))
beta = 1 - dt * (sigma**2 * np.arange(N+1)**2 + r)
gamma = 0.5 * dt * (sigma**2 * np.arange(N+1)**2 + r * np.arange(N+1))

# Iterate backwards in time
for j in range(M-1, -1, -1):
    for i in range(1, N):
        # Finite difference approximation
        V[i, j] = alpha[i] * V[i-1, j+1] + beta[i] * V[i, j+1] + gamma[i] * V[i+1, j+1]
        
        # Apply the early exercise condition
        V[i, j] = np.maximum(V[i, j], S[i] - K)

# Option value at the initial asset price S0
option_price = np.interp(S0, S, V[:, 0])

print(f"PYTHON | American Call Option Price: {option_price:.2f}")
