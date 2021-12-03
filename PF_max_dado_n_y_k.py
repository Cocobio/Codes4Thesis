import math

n = 5
k = 2
c = math.ceil(n/k)
f = math.floor(n/k)

max_cardinal = c*(c+1)**((n-1)%k)*(f+1)**(k*c-n) + c**((n-1)%k)*f**(k*c-n)

print(c)
print(f)
print(max_cardinal)