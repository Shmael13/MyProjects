"""
    This is a simple class called Polynomial for representing and manipulating polynomial functions such as:
        p(x) = a_0 + a_1(x) + a_2(x^2) + ... + a_N(x^N) where x is in the set of real numbers

The instance data for the class Polynomial will be the coefficients (in the case of above, the numbers a_0,...,a_N).

    The provided methods:
        - Evaluate the polynomial, returning p(x) for any x
        - Differentiate the polynomial, replacing the original coefficients with those of its derivative p'
"""

class Polynomial:
    def __init__(self, coefficients):
        self.coeff = coefficients

    def __call__(self, x):
        return sum( (x**exp) * a_i for exp, a_i in enumerate(self.coeff))
    
    def differentiate(self):
        self.coeff = [coef * deriv for deriv, coef in enumerate(self.coeff)]
        del self.coeff[0]
        return self.coeff

p = Polynomial([4,3,2,1])
print(p(2))
print(p.differentiate())
