#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define defaultInput "../../Inputs/2020_15.txt"
#define dataLine 100

int* readData(int *start, int *prev, int end, char *fileName) {
    int *arr = (int*)calloc(end + 1, sizeof(int));
    char textRead[dataLine], *p;
    int i = 0;

    // Open the file
    FILE *inFile = fopen(fileName, "r");

    // Check if the file exists or not
    if (inFile == NULL) {
        return NULL;
    }

    fgets(textRead, dataLine, inFile);
    p = strtok(textRead, ",");
    while (p != NULL) {
        int num = atoi(p);
        *prev = num;
        i++;
        arr[num] = i;
        p = strtok(NULL, ",");
    }

    *start = i + 1;
    fclose(inFile);

    return arr;
}

int cycle(int start, int end, int *spoken, int prev) {
    for (int i = start; i <= end; i++) {
        int temp = prev;

        if (spoken[temp] > 0) {
            prev = i - spoken[temp] - 1;
        } else {
            prev = 0;
        }

        spoken[temp] = i - 1;
    }

    return prev;
}

int part1(char *fileName) {
    int *start = (int*)calloc(1, sizeof(int)), *prev = (int*)calloc(1, sizeof(int));
    int *spoken = readData(start, prev, 2020, fileName);

    return cycle(*start, 2020, spoken, *prev);
}

int part2(char *fileName) {
    int *start = (int*)calloc(1, sizeof(int)), *prev = (int*)calloc(1, sizeof(int));
    int *spoken = readData(start, prev, 30000000, fileName);

    return cycle(*start, 30000000, spoken, *prev);
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
    printf("\nPart 1:\nThe 2020th spoken number is: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    int p2 = part2(inputPath);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nThe 30000000th spoken number is: %d\nRan in %f seconds\n", p2, t_p2);

	return 0;
}