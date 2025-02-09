#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#define defaultInput "../../Inputs/2020_10.txt"
#define dataLine 20

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

void readData(int *nums, char *fileName) {
    int lineNum = 0;
	char textRead[dataLine];

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return;
    }

	while(fgets(textRead, dataLine, inFile)) {
        nums[lineNum] = atoi(textRead);

        lineNum++;
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

int part1(int *nums, int numLines) {

    int diff1 = 0, diff3 = 0;
    for (int i = 0; i <= numLines; i++) {
        if (nums[i + 1] - nums[i] == 1) {
            diff1++;
        } else if (nums[i + 1] - nums[i] == 3) {
            diff3++;
        }
    }

    return diff1 * diff3;
}

long long findArrangements(int *arr, int start, int end) {
    if (start >= end) {
        return 1;
    }

    int i = start + 1;
    long long count = 0;

    while (arr[i] - arr[start] <= 3 && i <= end) {
        count += findArrangements(arr, i, end);
        i++;
    }

    return count;
}

long long part2(int *nums, int numLines) {
    long long count = 1;
    int numReq = 1;
    for (int i = 1; i < numLines + 1; i++) {
        if (nums[i + 1] - nums[i - 1] >= 4) {
            numReq++;
        }
    }

    int mustVisit[numReq], index = 1;
    mustVisit[0] = 0;
    for (int i = 1; i < numLines + 1; i++) {
        if (nums[i + 1] - nums[i - 1] >= 4) {
            mustVisit[index] = i;
            index++;
        }
    }

    for (int i = 0; i < numReq; i++) {
        count *= findArrangements(&nums[0], mustVisit[i], mustVisit[i + 1]);
    }

    return count;
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    int numLines, error;
	numLines = findLines(inputPath);

	if (numLines == -1) {
        printf("Error: Could not read input file. Quitting\n");
        return -1;
	}

	int nums[numLines + 2];

    readData(&nums[0], inputPath);
    nums[numLines] = 0;
    quickSort(nums, 0, numLines);
    nums[numLines + 1] = nums[numLines] + 3;

    clock_t t; 
    t = clock(); 
    int p1 = part1(&nums[0], numLines);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nProduct: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    long long p2 = part2(&nums[0], numLines);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nTotal number of distinct arrangements: %lld\nRan in %f seconds\n", p2, t_p2);

	return 0;
}