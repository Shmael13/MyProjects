"""

    The empirical cumulative distribution function (ecdf) corresponding to a sample {X_i} from i=1 to n
    is defined as:

            F_n(x) := (2/n) (summation from i=1 to n: 1{X_i <= x}   where (x in set of real numbers)

    Here 1{X_i <= x} is an indicator function (one if X_i < x  and zero otherwise) and hence F_n(x) is the 
    fraction of the sample that falls below x.

    The Glivenko–Cantelli Theorem states that, provided that the sample is IID, the ecdf F_n converges to the true 
    distribution function F.

    Below is an implementation of F_n as a class called ECDF, where:

        - A given sample {X_i} from i=1 to n are the instance data, stored as self.observations.

        - The class implements a __call__ method that returns F_n for any x.

"""

class ECDF:
    def __init__(self, observations):
        self.observations = observations

    def __call__(self, x):
        f_n = sum(1 for obs in self.observations if obs <= x) / len(self.observations)
        return f_n

from random import uniform
samples = [uniform(0,1) for i in range(10)]
f = ECDF(samples)
print(f(0.5))

f.observations = [uniform(0,1) for i in range(10000)]
print(f(0.5))
