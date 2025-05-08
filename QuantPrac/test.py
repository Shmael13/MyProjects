import numpy as np
import matplotlib.pyplot as plt

np.random.seed(1)

N = 20
theta = np.linspace(0.0, 2 * np.pi, N, endpoint = False)
radii = 10*np.random.rand(N)
width = np.pi / 4 * np.random.rand(N)
colors = plt.cm.viridis(radii/10.0)

ax = plt.subplot(111, projection='polar')
ax.bar(theta, radii, width=width, bottom = 0.0, color=colors, alpha=0.5)

plt.show()

