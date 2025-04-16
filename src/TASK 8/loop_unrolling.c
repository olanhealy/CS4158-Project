#include <stdio.h>
#include <time.h>

// Large array size (1,000,000 elements)
#define SIZE 1000000

// Fill the array with 1's
void fillArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = 1;
    }
}

// Regular summation
long long sum_regular(const int arr[], int size) {
    long long total = 0;
    for (int i = 0; i < size; i++) {
        total += arr[i];
    }
    return total;
}

// Unrolled summation: process 4 elements per iteration
long long sum_unrolled(const int arr[], int size) {
    long long total = 0;
    int i;

    for (i = 0; i <= size - 4; i += 4) {
        total += arr[i];
        total += arr[i + 1];
        total += arr[i + 2];
        total += arr[i + 3];
    }
    // Handle leftovers if size not multiple of 4
    for (; i < size; i++) {
        total += arr[i];
    }
    return total;
}

int main() {
    static int arr[SIZE];
    fillArray(arr, SIZE);

    clock_t start, end;
    double time_taken;

    // Regular loop
    start = clock();
    long long regSum = sum_regular(arr, SIZE);
    end = clock();
    time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Regular loop: Sum = %lld\n", regSum);
    printf("Time taken by regular loop: %.10f seconds\n\n", time_taken);

    // Unrolled loop
    start = clock();
    long long unrolledSum = sum_unrolled(arr, SIZE);
    end = clock();
    time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Unrolled loop: Sum = %lld\n", unrolledSum);
    printf("Time taken by loop unrolling: %.10f seconds\n", time_taken);

    return 0;
}
