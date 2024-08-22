#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_array(double *array, int n) {
    for (int i = 0; i < n; i++) {
        array[i] = (double)rand() / RAND_MAX;
    }
}

double dot_product(double *array1, double *array2, int n) {
    double product = 0.0;
    for (int i = 0; i < n; i++) {
        product += array1[i] * array2[i];
    }
    return product;
}

void report_time(int n, double gen_time, double calc_time) {
    printf("Array Size: %d\n", n);
    printf("Generation Time: %f seconds\n", gen_time);
    printf("Calculation Time: %f seconds\n", calc_time);
    printf("--------------------------------------\n");
}

int main() {
    int sizes[] = {1e7, 5e7, 1e8, 2e8, 5e8};  // Tamaños del array
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    srand(time(NULL));  // Seed para la generación de números aleatorios

    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        double *array1 = (double *)malloc(n * sizeof(double));
        double *array2 = (double *)malloc(n * sizeof(double));

        // Medir el tiempo de generación de arrays
        clock_t start_time = clock();
        generate_array(array1, n);
        generate_array(array2, n);
        clock_t end_time = clock();
        double gen_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

        // Medir el tiempo de cálculo del producto punto
        start_time = clock();
        double result = dot_product(array1, array2, n);
        end_time = clock();
        double calc_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

        // Reportar resultados
        report_time(n, gen_time, calc_time);

        free(array1);
        free(array2);
    }

    return 0;
}
