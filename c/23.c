#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define defaultInput "../../Inputs/2020_23.txt"
#define dataLine 10

typedef struct deque {
    int *cups;
    int size, capacity;
    int start, end;
} Deque;

struct deque *createDeque() {
    struct deque *d = (struct deque*)calloc(1, sizeof(struct deque));
    d->cups = (int*)calloc(1, sizeof(int));
    d->size = 0;
    d->capacity = 1;
    d->start = 0;
    d->end = 0;

    return d;
}

bool isEmpty(struct deque *d) {
    return d->size == 0;
}

bool isFull(struct deque *d) {
    return d->size == d->capacity;
}

int popFront(struct deque *d) {
    if (isEmpty(d)) {
        return -1;
    }

    int c = d->cups[d->start];
    d->start = (d->start + 1) % d->capacity;
    d->size--;

    return c;
}

int popBack(struct deque *d) {
    if (isEmpty(d)) {
        return -1;
    }

    int c = d->cups[d->end];
    d->end = (d->end - 1 + d->capacity) % d->capacity;
    d->size--;

    return c;
}

int peekFront(struct deque *d) {
    if (isEmpty(d)) {
        return -1;
    }

    return d->cups[d->start];
}

int peekBack(struct deque *d) {
    if (isEmpty(d)) {
        return -1;
    }

    return d->cups[d->end];
}

void pushFront(struct deque *d, int c) {
    if (isFull(d)) {
        d->capacity *= 2;
        int *temp = (int *)calloc(d->capacity, sizeof(int));
        for (int i = 0; i < d->size; i++) {
            temp[i] = d->cups[(d->start + i) % d->capacity];
        }

        free(d->cups);
        d->cups = temp;
        d->start = 0;
        d->end = d->size - 1;
    }

    d->start = (d->start - 1 + d->capacity) % d->capacity;
    d->cups[d->start] = c;
    d->size++;

    return;
}

void pushBack(struct deque *d, int c) {
    if (isFull(d)) {
        d->capacity *= 2;
        int *temp = (int *)calloc(d->capacity, sizeof(int));
        for (int i = 0; i < d->size; i++) {
            temp[i] = d->cups[(d->start + i) % d->capacity];
        }

        free(d->cups);
        d->cups = temp;
        d->start = 0;
        d->end = d->size - 1;
    }

    d->end = (d->end + 1) % d->capacity;
    d->cups[d->end] = c;
    d->size++;

    return;
}

void rotate_left(struct deque *d, int amt) {
    if (isEmpty(d)) {
        return;
    }

    for (int i = 0; i < amt; i++) {
        int c = popFront(d);
        pushBack(d, c);
    }

    return;
}

void rotate_right(struct deque *d, int amt) {
    if (isEmpty(d)) {
        return;
    }

    for (int i = 0; i < amt; i++) {
        int c = popBack(d);
        pushFront(d, c);
    }

    return;
}

int find_front(struct deque *d, int c) {
    for (int i = 0; i < d->size; i++) {
        if (d->cups[(d->start + i) % d->capacity] == c) {
            return i;
        }
    }

    return -1;
}

int find_back(struct deque *d, int c) {
    for (int i = 0; i < d->size; i++) {
        if (d->cups[(d->start + i) % d->capacity] == c) {
            return d->size - i - 1;
        }
    }

    return -1;
}

int *readData(char *fileName) {
    int *d = (int*)calloc(10, sizeof(int));
	char textRead[dataLine];

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return NULL;
    }

	while(fgets(textRead, dataLine, inFile)) {
        for (int i = 0; i < strlen(textRead); i++) {
            if (isdigit(textRead[i])) {
                d[i] = textRead[i] - 48;
            }
        }
	}

	fclose(inFile);

	return d;
}

int arr_index(int *arr, int size, int val) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == val) {
            return i;
        }
    }

    return -1;
}

int part1(char *fileName) {
    int *data = readData(fileName);
    struct deque *order = createDeque();
    for (int i = 0; i < 9; i++) {
        pushBack(order, data[i]);
    }
    
    free(data);
    int moves = 100;

    for (int i = 0; i < moves; i++) {
        
        int current = popFront(order);
        pushBack(order, current);

        int a = popFront(order);
        int b = popFront(order);
        int c = popFront(order);

        int dest = current, destIndex;
        do {
            dest--;
            if (dest == 0) {
                dest = 9;
            }

            destIndex = find_front(order, dest);
        } while (destIndex == -1);

        rotate_left(order, destIndex + 1);        
        pushFront(order, c);
        pushFront(order, b);
        pushFront(order, a);
        rotate_right(order, find_back(order, current));
    }

    rotate_left(order, find_front(order, 1) + 1);

    int result = 0;
    for (int i = 0; i < 8; i++) {
        result = result * 10 + popFront(order);
    }

    free(order->cups);
    free(order);
    return result;
}

unsigned long long part2(char *fileName) {
    int *data = readData(fileName);
    int moves = 10000000;

    int *cups = (int*)calloc(1000001, sizeof(int));
    int i, j, dest, afterDest, held, afterHeld;
    int removed[3];

    // Initialize cups
    cups[0] = data[0];

    for (i = 1; i < 10; i++) {
        int ix = arr_index(data, 9, i) + 1;

        if (ix < 9) {
            cups[i] = data[ix];
        } else {
            cups[i] = 10;
        }
    }

    free(data);

    for (i = 10; i < 1000000; i++) {
        cups[i] = i + 1;
    }

    cups[1000000] = cups[0];

    // Main loop
    for (i = 0; i < moves; i++) {
        held = cups[cups[0]];
        afterHeld = held;

        // Remove 3 cups
        for (j = 0; j < 3; j++) {
            removed[j] = afterHeld;
            afterHeld = cups[afterHeld];
        }

        // Find destination
        dest = ((cups[0] - 2) + 1000000) % 1000000;
        while (dest + 1 == removed[0] || dest + 1 == removed[1] || dest + 1 == removed[2]) {
            dest = ((dest - 1) + 1000000) % 1000000;
        }

        afterDest = cups[dest + 1];

        // Rearrange cups
        cups[cups[0]] = afterHeld;
        cups[dest + 1] = held;
        cups[removed[2]] = afterDest;
        cups[0] = cups[cups[0]];
    }

    return (unsigned long long) cups[1] * (unsigned long long) cups[cups[1]];
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    clock_t t;
    t = clock(); 
    int p1 = part1(inputPath);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nCup order: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock(); 
    unsigned long long p2 = part2(inputPath);
    t = clock() - t; 
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nProduct of next two labels: %llu\nRan in %f seconds\n", p2, t_p2);

    return 0;
}