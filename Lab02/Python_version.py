import random
import time
import sys
from multiprocessing import Pool, cpu_count

def generate_matrix(n):
    """Generate an n x n matrix with random values in the interval [0, 1]."""
    return [[random.random() for _ in range(n)] for _ in range(n)]

def multiply_row(args):
    """Multiply a single row of A with matrix B."""
    row, B = args
    n = len(B)
    result_row = [0] * n
    for k in range(n):
        a_ik = row[k]
        row_b = B[k]
        for j in range(n):
            result_row[j] += a_ik * row_b[j]
    return result_row

def matrix_multiply_parallel(A, B):
    """Multiply two matrices A and B using multiprocessing."""
    with Pool(cpu_count()) as pool:
        result = pool.map(multiply_row, [(A[i], B) for i in range(len(A))])
    return result

def report_memory(matrix):
    """Report the memory size of the given matrix in bytes."""
    total_size = sys.getsizeof(matrix)
    for row in matrix:
        total_size += sys.getsizeof(row)
        total_size += sum(sys.getsizeof(element) for element in row)
    return total_size

def bytes_to_mb(bytes_size):
    """Convert bytes to megabytes."""
    return bytes_size / (1024 * 1024)

def main():
    # Input for the size of the matrix
    sizes = [1000, 2000, 5000, 10000]  # Reduced sizes for faster testing
    n = int(input(f"Enter the size of the matrix (choose from {sizes}): "))

    if n not in sizes:
        print("Invalid size. Please choose from the specified sizes.")
        return

    # Step 1: Generate the matrices and measure time
    start_time = time.time()
    A = generate_matrix(n)
    B = generate_matrix(n)
    generation_time = time.time() - start_time

    # Report memory used for matrix generation
    memory_used = report_memory(A) + report_memory(B)

    # Step 2: Perform the product of the matrices using parallel processing
    start_time = time.time()
    C = matrix_multiply_parallel(A, B)
    calculation_time = time.time() - start_time

    # Report memory used for the result matrix
    memory_used_calculation = report_memory(C)

    # Total memory used in the program
    total_memory_used = memory_used + memory_used_calculation

    # Outputs
    print(f"GT: Generation Time = {generation_time:.6f} seconds")
    print(f"CT: Calculation Time = {calculation_time:.6f} seconds")
    print(f"Total memory used for generation of matrices: {bytes_to_mb(memory_used):.6f} MB")
    print(f"Total memory used in multiplication of matrices: {bytes_to_mb(memory_used_calculation):.6f} MB")
    print(f"Memory size of each matrix: {bytes_to_mb(report_memory(A)):.6f} MB (Matrix A), "
          f"{bytes_to_mb(report_memory(B)):.6f} MB (Matrix B)")
    print(f"Total memory used in the program: {bytes_to_mb(total_memory_used):.6f} MB")

if __name__ == "__main__":
    main()
