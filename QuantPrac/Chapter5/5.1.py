"""

Part 1: Given two numeric lists or tuples x_vals and y_vals of equal length, compute their inner product using zip().

Part 2: In one line, count the number of even numbers in 0,…,99.

Part 3: Given pairs = ((2, 5), (4, 2), (9, 8), (12, 10)), count the number of pairs (a, b) such that both a and b are even.
"""

#Part 1:
x_vals = (1,2,3,2,1)
y_vals = (5,4,5,3,2)

inner_prod = sum([x*y for x, y in zip(x_vals, y_vals)])

print(inner_prod)

#Part 2
count = 0
evenNums = sum(1 for num in range(0, 100) if num % 2 == 0)
print(evenNums)
evenNums = len(range(0,100,2))
print(evenNums)

#Part 3
pairs = ((2, 5), (4, 2), (9, 8), (12, 10))
print(len(list( (x,y) for x, y in pairs if x%2==y%2)))
print(sum([x%2==0 and y%2==0 for x, y in pairs]))
