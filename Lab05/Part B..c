#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>  // Incluir OpenMP

void generate_matrix(double **matrix, int n) {
    #pragma omp parallel for  // Paralelizar la generación de la matriz pregunta 1
    //#pragma omp parallel for collapse(2) //pregunta 4
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (double)rand() / RAND_MAX;
        }
    }
}

void matrix_product(double **result, double **matrix1, double **matrix2, int n) {
    #pragma omp parallel for  // Paralelizar el cálculo del producto de matrices //pregunta 1
    //#pragma omp parallel for collapse(2)//pregunta 4
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

void report_time_memory(int n, double gen_time, double calc_time, size_t gen_memory, size_t calc_memory) {
    printf("Matrix Size: %dx%d\n", n, n);
    printf("Generation Time: %f seconds\n", gen_time);
    printf("Calculation Time: %f seconds\n", calc_time);
    printf("Memory Used for Generation: %zu bytes\n", gen_memory);
    printf("Memory Used for Calculation: %zu bytes\n", calc_memory);
    printf("Total Memory Used: %zu bytes\n", gen_memory + calc_memory);
    printf("--------------------------------------\n");
}

int main() {
    int sizes[] = {2000, 5000};  // Tamaños de las matrices
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    srand(time(NULL));  // Seed para la generación de números aleatorios

    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        size_t matrix_memory = n * n * sizeof(double);

        double **matrix1 = (double **)malloc(n * sizeof(double *));
        double **matrix2 = (double **)malloc(n * sizeof(double *));
        double **result = (double **)malloc(n * sizeof(double *));
        
        for (int j = 0; j < n; j++) {
            matrix1[j] = (double *)malloc(n * sizeof(double));
            matrix2[j] = (double *)malloc(n * sizeof(double));
            result[j] = (double *)malloc(n * sizeof(double));
        }

        // Medir el tiempo de generación de matrices
        clock_t start_time = clock();
        generate_matrix(matrix1, n);
        generate_matrix(matrix2, n);
        clock_t end_time = clock();
        double gen_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

        // Medir el tiempo de cálculo del producto de matrices
        start_time = clock();
        matrix_product(result, matrix1, matrix2, n);
        end_time = clock();
        double calc_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

        // Reportar resultados
        report_time_memory(n, gen_time, calc_time, 2 * matrix_memory, matrix_memory);

        for (int j = 0; j < n; j++) {
            free(matrix1[j]);
            free(matrix2[j]);
            free(result[j]);
        }
        free(matrix1);
        free(matrix2);
        free(result);
    }

    return 0;
}
