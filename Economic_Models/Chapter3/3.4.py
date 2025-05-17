import numpy as np
import matplotlib.pyplot as plt

T = 200
alpha = 0.9
x = np.empty(T+1)
x[0] = 0

for t in range(T):
    abs_x = -x[t] if x[t] < 0 else x[t]
    x[t+1] = alpha*abs_x+ np.random.randn()
   
plt.plot(x, label=f'$\\alpha = {alpha}$')
plt.legend()
plt.show()


