"""
When we cover the numerical libraries, we will see they include many alternatives for interpolation and function approximation.

Nevertheless, let’s write our own function approximation routine as an exercise.

In particular, without using any imports, write a function linapprox that takes as arguments

A function f mapping some interval [a,b] into R.

Two scalars a and b providing the limits of this interval.

An integer n determining the number of grid points.

A number x satisfying a <= x <= b.

and returns the piecewise linear interpolation of f at x, based on n evenly spaced grid points a = point[0] < point[1] < ... < point[n-1] = b.

Aim for clarity, not efficiency.

"""
def approx_func(f, a, b, n, x):
    interval_len = b - a
    num_subintervals = n - 1
    step = interval_len / num_subintervals

    #first gridpt larger than x
    point = a
    while point <= x:
        point += step

    #x is b/w gridpoints (point - step) and point
    u, v = point - step, point

    return f(u) + (x-u) * (f(v) - f(u)) / step
