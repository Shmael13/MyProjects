"""
Write a function that takes a string as an argument and returns the number of capital letters in the string.
"""

def strcnt(x):
    return sum([s.isupper() for s in x])

print(strcnt('The rain in SPain is good. Right?'))
