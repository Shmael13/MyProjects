import matplotlib.pyplot as plt
import numpy as np

x = np.linspace(0, 2 * np.pi, 100)
y = np.sin(x)

plt.plot(x,y)
plt.xlabel('x')
plt.ylabel('y')
plt.title('Sin wave')
plt.show()
