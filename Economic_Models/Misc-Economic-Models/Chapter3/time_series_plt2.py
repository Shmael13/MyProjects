import numpy as np
import matplotlib.pyplot as plt
from time_series_plt import timeplt

a_vals = [0.0 ,0.8, 0.98]
T = 200
x = np.empty(T+1)
x[0] = 0

for a in a_vals:
    for t in range(T):
        x[t+1] = a * x[t] + np.random.randn()
    plt.plot(x, label=f'$\\alpha = {a}$')

plt.legend()
plt.show()
