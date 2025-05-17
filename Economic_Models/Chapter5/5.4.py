"""
Write a function that takes two sequences seq_a and seq_b as arguments and returns True if every element in seq_a is also an element of seq_b, else False.

By “sequence” we mean a list, a tuple or a string.

Do the exercise without using sets and set methods.
"""

def func(seq1, seq2):
    return all([i in seq2 for i in seq1])

print(func([1,2,3], [1,2,3]))
