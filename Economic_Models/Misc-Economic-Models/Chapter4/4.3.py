"""First, write a function that returns one realization of the following random device

Flip an unbiased coin 10 times.

If a head occurs k or more times consecutively within this sequence at least once, pay one dollar.

If not, pay nothing.

Second, write another function that does the same task except that the second rule of the above random device becomes

If a head occurs k or more times within this sequence, pay one dollar.

Use no import besides from numpy.random import uniform."""

from numpy.random import uniform

def flipper(k):
    head_cnt = 0
    for i in range(10):
        U = uniform()
        if (U < 0.5):
            head_cnt += 1
        else:
            break
    if (head_cnt > k):
        return 1
    return 0

def flipper2(k):
    head_cnt = 0
    for i in range(10):
        U = uniform()
        if (U < 0.5):
            head_cnt += 1
    if (head_cnt > k):
        return 1
    return 0

