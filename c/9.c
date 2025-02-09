#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#define defaultInput "../../Inputs/2020_9.txt"
#define dataLine 20
#define buffer 25

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

int findMin(int *nums, int start, int end) {
    int min = nums[start];
    for (int i = start; i < end; i++) {
        if (nums[i] < min) {
            min = nums[i];
        }
    }

    return min;
}

int findMax(int *nums, int start, int end) {
    int max = nums[start];
    for (int i = start; i < end; i++) {
        if (nums[i] > max) {
            max = nums[i];
        }
    }

    return max;
}

int part1(int *nums, int numLines) {
    int error;
    for (int i = buffer; i < numLines; i++) {
        bool valid = false;
        for (int j = i - buffer; j < i - 1; j++) {
            for (int k = j + 1; k < i; k++) {
                if (nums[j] + nums[k] == nums[i]) {
                    valid = true;
                    break;
                }
            }

            if (valid) {
                break;
            }
        }

        if (!valid) {
            error = nums[i];
            break;
        }
    }

    return error;
}

int part2(int *nums, int numLines) {
    int error = part1(nums, numLines);
    int weakness = -1;
    for (int i = 2; i < numLines; i++) {
        for (int j = 0; j < numLines; j++) {
            int count = 0;
            for (int k = j; (k < j + i && k < numLines); k++) {
                count += nums[k];
            }

            if (count == error) {
                weakness = findMin(nums, j, j + i) + findMax(nums, j, j + i);
            }
        }

        if (weakness != -1) {
            break;
        }
    }

    return weakness;
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

	int nums[numLines];
    readData(&nums[0], inputPath);

    clock_t t; 
    t = clock(); 
    int p1 = part1(&nums[0], numLines);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nInvalid found: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    int p2 = part2(&nums[0], numLines);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nThe encryption weakness is: %d\nRan in %f seconds\n", p2, t_p2);

	return 0;
}