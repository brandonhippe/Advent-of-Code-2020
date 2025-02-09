#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#define defaultInput "../../Inputs/2020_5.txt"
#define dataLine 15

int findLines(char *fileName) {
	int numLines = 0;
	char textRead[dataLine];

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

	while(fgets(textRead, dataLine, inFile)) {
        ++numLines;
	}

	fclose(inFile);

	return numLines;
}

void readData(int *seats, char *fileName) {
    int index = 0;
	char textRead[dataLine], *p;

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	while(fgets(textRead, dataLine, inFile)) {
        p = strtok(textRead, "\n");
        for (int i = 0; i < 10; i++) {
            if (p[i] == 'F' || p[i] == 'L') {
                p[i] = '0';
            } else if (p[i] == 'B' || p[i] == 'R') {
                p[i] = '1';
            }
        }

        int num = strtol(p, NULL, 2);
        seats[index] = num;
        index++;
	}

	fclose(inFile);

    return;
}

void swap (int *array, int a, int b) {
	int temp;

	// Only swap if indecies are different
    if (a != b) {
        temp = array[a];
        array[a] = array[b];
        array[b] = temp;
    }
}

void findPivotIndex(int *sortArray, int start, int end, int *pivotIndex) {
	// Set pivot index to the start of the array
	*pivotIndex = start;

    // Set pivot value to value at end of array
    int pivotValue;
    pivotValue = sortArray[end];

    // Check each item to see if it is less than the pivot value. Swap and increment pivot index if less
    for (int i = start; i < end; ++i) {
        if (sortArray[i] < pivotValue) {
            swap(sortArray, i, *pivotIndex);
            ++*pivotIndex;
        }
    }

    // Swap the pivot value and the current item at pivot index
    swap(sortArray, *pivotIndex, end);
}

void quickSort(int *sortArray, int start, int end) {
	int pivotIndex;

	// Stop recursively calling quickSort
	if (start >= end) {
		return;
	}

	// Selects a pivot value, puts all values less than pivot value on left of pivot, all values larger than pivot on right of pivot, returns where the pivot ended up
	findPivotIndex(sortArray, start, end, &pivotIndex);

	// Sort all items before pivot index, then all items after pivot index
	quickSort(sortArray, start, pivotIndex - 1);
	quickSort(sortArray, pivotIndex + 1, end);
}

int part1(int *arr, int total) {
    int max = 0;
    for (int i = 0; i < total; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    return max;
}

int part2(int *seats, int numLines) {
    quickSort(seats, 0, numLines - 1);

    int seat = seats[0] + 1;
    for (int i = 1; i < numLines; i++) {
        if ((seats[i] == seat + 1) && (seats[i - 1] == seat - 1)) {
            break;
        }

        seat++;
    }

    return seat;
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

	int numLines, count = 0, max;

	numLines = findLines(inputPath);

	if (numLines == -1) {
        printf("Error: Could not read input file. Quitting\n");
        return -1;
	}

    int seats[numLines];
    readData(&seats[0], inputPath);

    clock_t t; 
    t = clock(); 
    int p1 = part1(&seats[0], numLines);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nMaximum seat id: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    int p2 = part2(&seats[0], numLines);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nOpen seat: %d\nRan in %f seconds\n", p2, t_p2);

	return 0;
}
