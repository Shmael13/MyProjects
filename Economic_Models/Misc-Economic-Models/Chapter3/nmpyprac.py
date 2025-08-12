import numpy as np

a = np.linspace(-np.pi, np.pi, 100)
print(a)

b = np.cos(a)
c = np.sin(a)

print(b @ c)

