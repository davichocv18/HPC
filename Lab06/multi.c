#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h> // For memory usage
#include <time.h> // For measuring time

#define M 2000  // Rows of matrix A and C
#define N 2000  // Columns of matrix A, Rows of matrix B
#define P 2000  // Columns of matrix B and C

// Function to multiply a local part of matrix A with matrix B
void matrix_multiply(double* local_A, double* B, double* local_C, int rows_per_process, int n, int p) {
    for (int i = 0; i < rows_per_process; i++) {
        for (int j = 0; j < p; j++) {
            local_C[i * p + j] = 0;
            for (int k = 0; k < n; k++) {
                local_C[i * p + j] += local_A[i * n + k] * B[k * p + j];
            }
        }
    }
}

// Function to print memory usage
void print_memory_usage(int rank) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    printf("Process %d Memory Usage: %ld kilobytes\n", rank, usage.ru_maxrss);
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define matrix dimensions and rows per process
    int rows_per_process = M / size;

    // Allocate memory for matrices
    double* A = NULL; // Matrix A (M x N)
    double* B = (double*)malloc(N * P * sizeof(double)); // Matrix B (N x P)
    double* C = NULL; // Matrix C (M x P)

    // Allocate memory for local parts of matrix A and C
    double* local_A = (double*)malloc(rows_per_process * N * sizeof(double));
    double* local_C = (double*)malloc(rows_per_process * P * sizeof(double));

    // Check memory allocation
    if (local_A == NULL || local_C == NULL || (rank == 0 && (B == NULL))) {
        printf("Error allocating memory on process %d\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Initialize matrix A and B (only process 0 needs to initialize)
    if (rank == 0) {
        A = (double*)malloc(M * N * sizeof(double)); // Allocate memory for A
        if (A == NULL) {
            printf("Error allocating memory for matrix A on process %d\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        for (int i = 0; i < M * N; i++) {
            A[i] = i + 1;  // Fill with dummy values (1, 2, 3, ...)
        }

        for (int i = 0; i < N * P; i++) {
            B[i] = i + 1;  // Fill with dummy values (1, 2, 3, ...)
        }
    }

    // Measure time for Bcast
    double start_time, end_time;
    start_time = MPI_Wtime();

    // Broadcast matrix B to all processes (all processes need B)
    MPI_Bcast(B, N * P, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();
    if (rank == 0) {
        printf("Time for MPI_Bcast: %f seconds\n", end_time - start_time);
    }
    print_memory_usage(rank); // Print memory usage after Bcast

    // Measure time for Scatter
    start_time = MPI_Wtime();

    // Scatter rows of matrix A among processes
    MPI_Scatter(A, rows_per_process * N, MPI_DOUBLE, local_A,
                rows_per_process * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();
    if (rank == 0) {
        printf("Time for MPI_Scatter: %f seconds\n", end_time - start_time);
    }
    print_memory_usage(rank); // Print memory usage after Scatter

    // Measure time for matrix multiplication
    start_time = MPI_Wtime();

    // Perform local computation (each process multiplies its assigned rows of A with matrix B)
    matrix_multiply(local_A, B, local_C, rows_per_process, N, P);

    end_time = MPI_Wtime();
    printf("Process %d Time for local matrix multiplication: %f seconds\n", rank, end_time - start_time);
    print_memory_usage(rank); // Print memory usage after multiplication

    // Measure time for Gather
    start_time = MPI_Wtime();

    // Gather the results from all processes into matrix C
    if (rank == 0) {
        C = (double*)malloc(M * P * sizeof(double)); // Allocate memory for C
        if (C == NULL) {
            printf("Error allocating memory for matrix C on process %d\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    MPI_Gather(local_C, rows_per_process * P, MPI_DOUBLE, C,
               rows_per_process * P, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();
    if (rank == 0) {
        printf("Time for MPI_Gather: %f seconds\n", end_time - start_time);
    }
    print_memory_usage(rank); // Print memory usage after Gather

    // Print additional information
    if (rank == 0) {
        // Calculate total memory used
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        long total_memory_used = usage.ru_maxrss;

        printf("\n--- Matrix Multiplication Results ---\n");
        printf("n: %d (Array size)\n", M * N); // Total number of elements in matrix A
        printf("GT: %f seconds (Generation Time)\n", end_time - start_time); // GT for the entire process
        printf("CT: %f seconds (Calculation Time)\n", end_time - start_time); // CT for the entire process
        printf("Total Memory Used: %ld kilobytes\n", total_memory_used); // Total memory used
    }

    // Free allocated memory
    free(local_A);
    free(local_C);
    if (rank == 0) {
        free(A);
        free(B);
        free(C);
    } else {
        free(B); // Process 0 allocates B, others do not need to free it
    }

    MPI_Finalize();
    return 0;
}


