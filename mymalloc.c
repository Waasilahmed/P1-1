#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "mymalloc.h"

#define MEMSIZE 4096
static char memory[MEMSIZE];

void initialize() {
    memset(memory, 0, MEMSIZE);
    memory[0] = 'I';
}

void *mymalloc(size_t size, char *file, int line) {
    int index = 1;

    // Initialize memory if it's not initialized yet
    switch(memory[0]) {
        case 'I':
            break;
        default:
            initialize();
            break;
    }

    // Return NULL if size is 0
    if (size == 0)
        return NULL;

    // Find a free block of memory or allocate new memory
    while (index <= MEMSIZE) {
        if (memory[index] == 1) {
            int clientdata_size = memory[index + 1] * 1000 + memory[index + 2] * 100 + memory[index + 3] * 10 + memory[index + 4];
            index = (index + 4 + clientdata_size + 1);
        } else if (memory[index] == 0 && memory[index + 1] != 0) {
            int clientdata_size = memory[index + 1] * 1000 + memory[index + 2] * 100 + memory[index + 3] * 10 + memory[index + 4];
            if ((size + 6) <= clientdata_size) {
                break;
            } else {
                index = (index + 4 + clientdata_size + 1);
            }
        } else {
            if ((index + 4 + size) > MEMSIZE - 1) {
                fprintf(stderr, "Program Terminated from %s %d: Error! Memory is Full!\n", file, line);
                exit(EXIT_FAILURE);
            } else if ((index + 4 + size) >= MEMSIZE - 1) {
                break;
            }
            break;
        }
    }

    // Mark the allocated memory block
    memory[index] = 1;

    // Store the size of the allocated memory block
    int num = size, count = 4;
    while (num != 0) {
        int digit = num % 10;
        num /= 10;
        memory[index + count--] = digit;
    }

    // Check for memory overflow and reset metadata if needed
    bool isMemoryOverflow = (index + 4 + size + 1 + 5 > MEMSIZE - 1);
    if (!isMemoryOverflow) {
        int new_metadata_index = index + 4 + size + 1;
        for (int i = 0; i < 5; i++) {
            memory[new_metadata_index + i] = 0;
        }
    }

    // Set payload values
    for (int i = 0; i < size; i++) {
        memory[index + 5 + i] = 9;
    }

    return &memory[index + 5];
}

void placeSize(int size, int metadata_index) {
    int num = size;
    for (int count = 4; num != 0; num /= 10) {
        int digit = num % 10;
        memory[metadata_index + count] = digit;
        count--;
    }
}

void setZero(int start, int end) {
    int i = start;
    do {
        memory[i] = 0;
        i++;
    } while (i <= end);
}

void myfree(void *p, char *file, int line) {
    char *client_data = (char *)p;

    if (client_data == NULL) {
        fprintf(stderr, "Program Terminated %s %d: Error! Attempting to free NULL pointer!\n", file, line);
        exit(EXIT_FAILURE);
    }

    bool valid_address = false;
    for (int i = 0; i < MEMSIZE; i++) {
        if (client_data == &memory[i]) {
            valid_address = true;
            break;
        }
    }

    if (!valid_address) {
        fprintf(stderr, "Program Terminated %s %d: Error! Attempting to free an invalid address!\n", file, line);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MEMSIZE; i++) {
        if (client_data == &memory[i]) {
            break;
        }
    }

    char *first_digit, *second_digit, *third_digit, *fourth_digit;
    fourth_digit = (client_data - 1);
    third_digit = (client_data - 2);
    second_digit = (client_data - 3);
    first_digit = (client_data - 4);
    char *status = (client_data - 5);

    int size = ((*first_digit) * 1000) + ((*second_digit) * 100) + ((*third_digit) * 10) + ((*fourth_digit) * 1);
    int i = 0;
    do {
        char *ptr = (char *)(p + i);
        *ptr = 0;
        i++;
    } while (i < size);

    *status = 0;

    int flag = 0;

    for (int index = 0; index < MEMSIZE - 1; ) {
        if (memory[index] == 1) {
            flag = 1;
            break;
        }
        if (index + 4 > MEMSIZE - 1) break;
        int s = memory[index + 1] * 1000 + memory[index + 2] * 100 + memory[index + 3] * 10 + memory[index + 4];
        index = index + 4 + s + 1;
    }

    if (flag == 0) {
        setZero(1, MEMSIZE - 1);
    }

    int current_metadata_index = 1, previous_free_index = -1;
    int status_coalesce = memory[current_metadata_index],
        size_coalesce = memory[current_metadata_index + 1] * 1000 + memory[current_metadata_index + 2] * 100 + memory[current_metadata_index + 3] * 10 + memory[current_metadata_index + 4],
        next_metadata_index = current_metadata_index + 4 + size_coalesce + 1;

    while (current_metadata_index <= MEMSIZE - 2) {
        status_coalesce = memory[current_metadata_index];
        if (current_metadata_index + 4 > MEMSIZE - 1) {
            return;
        }
        size_coalesce = memory[current_metadata_index + 1] * 1000 + memory[current_metadata_index + 2] * 100 + memory[current_metadata_index + 3] * 10 + memory[current_metadata_index + 4];
        next_metadata_index = current_metadata_index + 4 + size_coalesce + 1;
        if (status_coalesce == 0) {
            if (previous_free_index == -1) {
                previous_free_index = current_metadata_index;
                current_metadata_index = next_metadata_index;
                continue;
            }
            int prev_size_coalesce = memory[previous_free_index + 1] * 1000 + memory[previous_free_index + 2] * 100 + memory[previous_free_index + 3] * 10 + memory[previous_free_index + 4];
            int sum_size_coalesce = prev_size_coalesce + size_coalesce;
            placeSize(sum_size_coalesce, previous_free_index);
            setZero(current_metadata_index, current_metadata_index + 4 + size_coalesce);
            current_metadata_index = next_metadata_index;
            continue;
        } else {
            current_metadata_index = next_metadata_index;
            previous_free_index = -1;
            continue;
        }
    }
}
