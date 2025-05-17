import numpy as np
import matplotlib.pyplot as plt

def timeplt(T, alpha):
    t = 1
    x = np.empty(T+1)
    x[0] = 0
    e_vals = np.random.randn(T)

    while(t < T):
        x[t] = alpha * x[t-1] + e_vals[t-1]
        t+=1

    plt.plot(x)
    plt.show()

timeplt(200, 1)
