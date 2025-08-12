import numpy as np
import matplotlib.pyplot as plt

r = 0.025 #interest_rate
T = 50    #end date
b = np.empty(T+1) #empty numpy arr to store all b_t
b[0] = 10 # init balance

for t in range(T):
    b[t+1] = (1+r) * b[t]

plt.plot(b, label='bank balance')
plt.legend()
plt.show()
