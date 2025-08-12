import numpy as np

n = 10000000

count = 0

for i in range(n):
    #draw rand positions on square
    x, y = np.random.uniform(), np.random.uniform()

    #check if point falls within boundary of circle centered at 0,0
    dist = np.sqrt(((x-0.5)**2) + ((y-0.5)**2))

    if dist < 0.5:
        count += 1

area_estimate = count / n

print(area_estimate * 4)

