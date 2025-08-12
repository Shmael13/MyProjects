import numpy as np
import matplotlib.pyplot as plt

ts_len = 100
epsilon_vals = []

i = 0;
while(i < ts_len):
    e = np.random.randn()
    epsilon_vals.append(e)
    i+=1

plt.plot(epsilon_vals)
plt.show()
