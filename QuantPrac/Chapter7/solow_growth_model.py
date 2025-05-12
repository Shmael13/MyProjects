"""

The Solow growth model is a neoclassical growth model,
in which the per capita capital stock k(t) evolves according to the rule:

k(t+1) = (szk(t)^a + (1 - d)k(t)) / 1 + n

s is a saving rate

z is a productivity parameter

a is capital’s share of income

n is the population growth rate

d is the depreciation rate

A steady state of the model is a k that solves the
equation when k(t+1) = k(t) = k
"""
import matplotlib.pyplot as plt

class Solow:
    r"""
    Implements the Solow growth model with the update rule
        
        k_{t+1} = [s z k^a_t] + (1 - d)k_t] / (1+n)

    """
    def __init__(self,
                 n=0.05, #pop growth rate
                 s=0.25, #savings rate
                 d=0.1,  #deprecation rate
                 a=0.3,  #share of income
                 z=2.0,  #productivity
                 k=1.0  #current capital stock
                ):
        self.n, self.s, self.d, self.a, self.z = n, s, d, a, z
        self.k = k

    def h(self):
        "Eval the h func"
        #Unpack params
        n, s, d, a, z , k = self.n, self.s, self.d, self.a, self.z, self.k
        return ((s * z * (k ** a)) + ((1-d)*k) )  / (1 + n)

    def update(self):
        self.k = self.h()
    
    def steady_state(self):
        "Compute the steady state val of capital"
        #Unpack params
        n, s, d, a, z  = self.n, self.s, self.d, self.a, self.z        

        #Compute steady state
        return ((n+d)/(s*z)) ** (1/(a-1))
    
    def generate_sequence(self, t):
        "Generate time series of len t"
        path = []
        for i in range(t):
            path.append(self.k)
            self.update()
        return path

""" Examples of usage """
"""
s1 = Solow()
s2 = Solow(k=10)

timesteps = 60
fig, subplot = plt.subplots(figsize = (9,6))

#Plot common steady state val of capital
subplot.plot([s1.steady_state()]*timesteps, 'k-', label='steady state')

#Plot time series for each economy
for s in s1, s2:
    lb = f'Capital series from initial state {s.k}'
    subplot.plot(s.generate_sequence(timesteps), 'o-', lw=2, alpha=0.6, label=lb)

subplot.set_xlabel('$t$', fontsize = 15)
subplot.set_ylabel('$k_t$', fontsize = 15)
subplot.legend()
plt.show()
"""
