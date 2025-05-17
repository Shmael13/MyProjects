import numpy as np
import matplotlib.pyplot as plt

T = 200
alpha = 0.9
x = np.empty(T+1)
x[0] = 0

for t in range(T):
    x[t+1] = alpha*abs(x[t]) + np.random.randn()

plt.plot(x, label=f'$\\alpha = {alpha}$')
plt.legend()
plt.show()
