import numpy as np
import time

def generate_random_arrays(n):
    start_time = time.perf_counter()
    array1 = np.round(np.random.rand(n), 9)
    array2 = np.round(np.random.rand(n), 9)
    end_time = time.perf_counter()
    generation_time = end_time - start_time
    return array1, array2, generation_time

def dot_product_numpy(arr1, arr2):
    start_time = time.perf_counter()
    result = np.dot(arr1, arr2)
    end_time = time.perf_counter()
    dot_time = end_time - start_time
    return result, dot_time

def run_tests():
    sizes = [10**7, 5*10**7, 10**8, 2*10**8, 5*10**8]
    for n in sizes:
        print(f"\nCalculando con n = {n}")
        arr1, arr2, gen_time = generate_random_arrays(n)
        dot_result, dot_time = dot_product_numpy(arr1, arr2)

        print(f"Tiempo para generar los arreglos: {gen_time:.6f} segundos")
        print(f"Tiempo para calcular el producto punto: {dot_time:.10f} segundos")
        print(f"Resultado del producto punto: {dot_result:.9f}")

# Ejecución
run_tests()
