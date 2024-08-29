import numpy as np
import time
import psutil

def generate_matrix(n):
    """Genera una matriz nxn con valores aleatorios en el intervalo [0, 1]."""
    start_time = time.time()
    matrix = np.random.rand(n, n)
    end_time = time.time()
    generation_time = end_time - start_time
    memory_usage = matrix.nbytes
    return matrix, generation_time, memory_usage

def multiply_matrices(matrix1, matrix2):
    """Realiza el producto de dos matrices."""
    start_time = time.time()
    result = np.dot(matrix1, matrix2)
    end_time = time.time()
    multiplication_time = end_time - start_time
    memory_usage = result.nbytes
    return result, multiplication_time, memory_usage

def main():
    """Función principal que ejecuta los pasos del programa."""
    # Definición de los tamaños de las matrices a generar
    sizes = [10**3, 2*10**3, 5*10**3, 10**4]
    
    for n in sizes:
        print(f"Generando matrices de tamaño {n}x{n}...")
        
        # Medición de la memoria antes de la generación de matrices
        process = psutil.Process()
        memory_before = process.memory_info().rss
        
        # Generar las dos matrices
        matrix1, gen_time1, mem_usage1 = generate_matrix(n)
        matrix2, gen_time2, mem_usage2 = generate_matrix(n)
        
        # Medición de la memoria después de la generación de matrices
        memory_after_generation = process.memory_info().rss
        memory_used_generation = memory_after_generation - memory_before
        
        print(f"Tiempo para generar una matriz de tamaño {n}x{n}: {gen_time1:.6f} segundos")
        print(f"Memoria utilizada para una matriz de tamaño {n}x{n}: {mem_usage1 / (1024 ** 2):.2f} MB")
        
        # Multiplicar las matrices
        result, mult_time, mem_usage_result = multiply_matrices(matrix1, matrix2)
        
        # Medición de la memoria después de la multiplicación
        memory_after_multiplication = process.memory_info().rss
        memory_used_multiplication = memory_after_multiplication - memory_after_generation
        
        print(f"Tiempo para multiplicar las matrices de tamaño {n}x{n}: {mult_time:.6f} segundos")
        print(f"Memoria utilizada para el resultado de tamaño {n}x{n}: {mem_usage_result / (1024 ** 2):.2f} MB")
        print(f"Memoria total utilizada en la generación de matrices: {memory_used_generation / (1024 ** 2):.2f} MB")
        print(f"Memoria total utilizada en la multiplicación de matrices: {memory_used_multiplication / (1024 ** 2):.2f} MB")
        print("-" * 50)

if __name__ == "__main__":
    main()
