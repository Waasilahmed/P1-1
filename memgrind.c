#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "mymalloc.h"

static void test1() {
    for (int i = 1; i <= 120; i++) {
        char *ptr = malloc(1);
        free(ptr);
    }
}

static void test2() {
    char *array[120];
    for (int i = 0; i < 120; i++) {
        char *ptr = malloc(1);
        array[i] = ptr;
    }
    for (int i = 0; i < 120; i++) {
        free(array[i]);
    }
}

static void test3() {
    char *arr[120];
    int po = -1, nmal = 0;

    for (int i = 0; i < 120; i++) {
        int numberberber = rand() % 100 + 1;
        int umal;

        if (numberberber < 50) {
            if (nmal < 60)
                umal = 1; // Malloc if nmal < 60
            else
                umal = 0;
        } else {
            if (po >= 0)
                umal = 0; // If there's nothing to free, use malloc
            else
                umal = 1;
        }

        if (umal) {
            arr[++po] = malloc(sizeof(char));
            if (arr[po] == NULL) {
                printf("Memory allocation failed.\n");
                break;
            }
            nmal++;
        } else {
            if (po >= 0) {
                free(arr[po--]);
            }
        }
    }
    while (po >= 0) {
        free(arr[po--]);
    }
}

static void test4() {
    // Define varying sizes for allocations
    int sizes[] = {16, 32, 64, 128};
    const int numberber_sizes = sizeof(sizes) / sizeof(sizes[0]);
    char *ptrs[50]; // Array to hold pointers to allocated memory
    int i, ind_siz = 0;

    for (i = 0; i < 50; i++) {
        ind_siz = i % numberber_sizes; // Cycle through the sizes array
        ptrs[i] = malloc(sizes[ind_siz] * sizeof(char)); // Allocate memory of varying sizes
    }

    for (i = 49; i >= 0; i--) {
        int j = rand() % (i + 1);
        char *temp = ptrs[i];
        ptrs[i] = ptrs[j];
        ptrs[j] = temp;
    }

    // Free the memory in the shuffled order
    for (i = 0; i < 50; i++) {
        free(ptrs[i]);
    }
}

//Test Case 5 allocates memory at random sizes and then at the end, it frees everything.
//If the byte size is too big, it will throw an error.
static void test5() {
    const int allo_numb = 150;
    char *pointers[allo_numb];
    int sizes[] = {8, 16, 32};
    int numb_siz = sizeof(sizes) / sizeof(sizes[0]);
    int i;

    for (i = 0; i < allo_numb; i++) {
        int ind_siz = rand() % numb_siz; // Choose a random size
        pointers[i] = malloc(sizes[ind_siz]);
    }

    for (i = 0; i < allo_numb; i++) {
        free(pointers[i]);
    }
}

int main(int argc, char **argv) {
    struct timeval start_time, end_time;
    double execution_times[50][5] = {0};

    void (*test_functions[5])(void) = {test1, test2, test3, test4, test5};

    srand(time(0));
    for (int iteration = 0; iteration < 50; iteration++) {
        for (int ind_tes = 0; ind_tes < 5; ind_tes++) {
            gettimeofday(&start_time, NULL);
            test_functions[ind_tes]();
            gettimeofday(&end_time, NULL);

            execution_times[iteration][ind_tes] = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
        }
    }

    // Print the average runtime for each test after it runs 50 times
    for (int ind_tes = 0; ind_tes < 5; ind_tes++) {
        double total_execution_time = 0;
        for (int iteration = 0; iteration < 50; iteration++)
            total_execution_time += execution_times[iteration][ind_tes];

        printf("Test_%c average runtime: %f\n", 'A' + ind_tes, total_execution_time / 50);
    }

    return 0;
}
