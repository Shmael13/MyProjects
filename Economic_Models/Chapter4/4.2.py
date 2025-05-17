"""The binomial random variable Y ~ Bin(n,p) represents the number of successes in 
 binary trials, where each trial succeeds with probability p.

Without any import besides from numpy.random import uniform, write a function binomial_rv such that binomial_rv(n, p) generates one draw of Y."""


from numpy.random import uniform

def binomial_rv(n,p):
    count = 0
    for i in range(n):
        U = uniform()
        if U < p:
            count += 1
    return count

print(binomial_rv(1000, 0.5))
