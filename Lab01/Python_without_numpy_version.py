#Python Code Implementation (Without NumPy)

import random
import time

#Generación de Arreglos:
def generate_arrays(n):
    a = [random.random() for _ in range(n)]
    b = [random.random() for _ in range(n)]
    return a, b

#Cálculo del Producto Punto:
def dot_product(a, b):
    return sum(x * y for x, y in zip(a, b))

#Ingrese Valor del tamaño de las Listas
n = int(input("Enter the size of Arrays: "))

#Tamaño de las Listas a evaluar: 
# 10^7 =10000000
# 5×10^7 =50000000
# 10^8 = 100000000
# 2x10^8 = 200000000
# 5x10^8 = 500000000

#Medición del Tiempo:
start_time = time.time()
a, b = generate_arrays(n)
gen_time = time.time() - start_time

start_time = time.time()
result = dot_product(a, b)
calc_time = time.time() - start_time

print(f"Size Array: {n}")
print(f"Dot Product: {result}")
print(f"Generation Time: {gen_time} seconds")
print(f"Calculation Time: {calc_time} seconds")