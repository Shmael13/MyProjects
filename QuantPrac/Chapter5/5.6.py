"""
Using list comprehension syntax, we can simplify the loop in the following code.

import numpy as np

n = 100
ϵ_values = []
for i in range(n):
    e = np.random.randn()
    ϵ_values.append(e)
"""

import numpy as np

n = 100
e_vals = [np.random.randn() for i in range(n)]
