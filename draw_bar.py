import matplotlib.pyplot as plt
import numpy as np

# Data
algorithms = ['Greedy', 'DP_recursive', 'DP_top_down', 'DP_bottom_up']
scenarios = ['Original', 'Replace 50', 'Replace 10', 'Replace 10 & 50']

# Execution times (in seconds)
times = [
    [3.81e-06, 0.000105858, 0.000102997, 0.00229597],   # Greedy
    [3.81e-05, 0.00219107, 0.00113106, 0.041693],       # DP_recursive
    [1.69e-05, 0.000754118, 0.000935078, 0.0162182],    # DP_top_down
    [2.22e-05, 0.000854015, 0.000777006, 0.0180609],    # DP_bottom_up
]

# Plotting
bar_width = 0.2
x = np.arange(len(scenarios))

plt.figure(figsize=(10, 6))
for i, algo in enumerate(algorithms):
    plt.bar(x + i * bar_width, times[i], width=bar_width, label=algo)

# Labels and legend
plt.xlabel('Coin Set Scenario', fontsize=14)
plt.ylabel('Average Execution Time (seconds)', fontsize=14)
plt.title('Execution Time Comparison of Coin Change Algorithms', fontsize=14)
plt.xticks(x + 1.5 * bar_width, scenarios)
plt.yscale('log')  # Use log scale due to wide time range
plt.legend(fontsize=14)
plt.grid(axis='y', linestyle='--', alpha=0.6)

plt.tight_layout()
plt.show()
