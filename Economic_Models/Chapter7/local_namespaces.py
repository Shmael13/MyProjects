def f(x):
    a = 2
    print(locals())
    return a * x

print(f(2))
print(locals())
