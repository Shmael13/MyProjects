"""
Consider the polynomial p(x) = a0 + a1x + a2x^2 + ... + anx^n

Write a function p such that p(x, coeff) that computes the value in  given a point x and a list of coefficients coeff (a1, a2, ... an).

Try to use enumerate() in your loop.
"""

def p(x, coeff):
    return sum(a*(x ** exp) for exp, a in enumerate(coeff))

print(p(1, [2,4]))

