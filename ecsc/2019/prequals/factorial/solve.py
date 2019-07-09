#coding: utf-8
from sacricat.client import Server, logging

import  math
import re

class ChineseRemainderTheorem():
    #https://raw.githubusercontent.com/Meng-Gen/ProjectEuler/master/160.py
    """
    Solve
        x = a (mod m)
        x = b (mod n)
    where m and n are coprime.
    """
    def solve(self, a, m, b, n):
        q = m*n
        (x, y) = self._extended_gcd(m, n)
        root = a + (b - a) * x * m
        return ((root % q) + q) % q
    
    def _extended_gcd(self, a, b):
        (x, y) = (0, 1)
        (last_x, last_y) = (1, 0)
        while b != 0:
            (q, r) = divmod(a, b);
            (a, b) = (b, r)
            (x, last_x) = (last_x - q * x, x)
            (y, last_y) = (last_y - q * y, y)
        return (last_x, last_y)

class ModInverse():
    """
    Solve ax = 1 (mod m).
    """
    def get(self, a, m):
        g, x, y = self._extended_gcd(a, m)
        if g != 1:
            raise Exception('modular inverse does not exist')
        else:
            return x % m
    
    def _extended_gcd(self, a, b):
        if a == 0:
            return (b, 0, 1)
        else:
            g, y, x = self._extended_gcd(b % a, a)
            return (g, x - (b // a) * y, y)

class Problem():
    def __init__(self):
        self.theorem = ChineseRemainderTheorem()
        self.mod_inverse = ModInverse()

    def get_last_nonzero_digits(self, n, s=5):
        norm = self.get_norm(n, 5)
        a = self.get_last_factorial(n, 5, s)
        x = self.theorem.solve(a, 5**s, 0, 2**s)
        y = pow(self.mod_inverse.get(2, 5**s), norm, 5**s)
        return (x*y) % 10**s

    def get_last_factorial(self, n, p, s):
        result = 1
        t = 1
        i = 0
        while t <= n:
            result *= (-1)**(n // p**(s + i)) * self.get_free_factorial_naive((n // t) % p**s, p, s)
            result %= p**s
            t *= p
            i += 1
        return result

    def get_free_factorial_naive(self, n, p, s):
        result = 1
        mod = p**s
        for i in range(1, n + 1):
            if i % p != 0:
                result = (result * i) % mod
        return result

    def get_norm(self, n, p):
        t = p
        result = 0
        while t <= n:
            result += n // t
            t *= p
        return result

problem = Problem()

def factendzero(n):
    x = n // 5
    y = x 
    while x > 0:
        x /= 5
        y += int(x)
    return y

def find_n_last_digit(fact,n):
	res = problem.get_last_nonzero_digits(fact)
	return str(res)[-n:]

re_fact = re.compile('[0-9]+!')
re_non_zero_digit = re.compile('What are the ([0-9]+) last nonzero digits of')

client = Server('challenges.ecsc-teamfrance.fr',3000, prompt=">> ",timeout=5, logLevel=logging.BASIC)

statement = client.recvUntil()
client.sendLine('720')
statement = client.recvUntil()
client.sendLine('4')

step = 0

statement = client.recvUntil()
while 'ECSC{' not in statement:
	print("Step : %s" % (step))
	print(statement, end='')
	fact = re_fact.findall(statement)
	fact = fact_bak = int(fact[-1][:-1])
	if step < 8:
		fact = math.factorial(fact)
		fact = str(fact)
		res = fact[-1]
		print(res)
		client.sendLine(fact[-1])
	else:
		length = int(re_non_zero_digit.search(statement).group(1))
		res = find_n_last_digit(fact, length)
		print(res)

		client.sendLine(str(res))

	step += 1
	statement = client.recvUntil()

print(statement)

'''
[2019-05-14 17:05:36][*] challenges.ecsc-teamfrance.fr:3001 - Sent - '203294274751'
[2019-05-14 17:05:36][*] challenges.ecsc-teamfrance.fr:3001 - Received - 'Congrats! Here is your flag: ECSC{e076963c132ec49bce13d47ea864324326d4cefa}\n'
[2019-05-14 17:05:36][-] challenges.ecsc-teamfrance.fr:3001 - Disconnected
'''