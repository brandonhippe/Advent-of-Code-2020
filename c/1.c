#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#define defaultInput "../../Inputs/2020_1.txt"
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

void readData(int *nums, char* fileName) {
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

int part1(int *nums, int numLines) {
    int product;

    for (int i = 0; i < numLines - 1; i++) {
        for (int j = i + 1; j < numLines; j++) {
            if (nums[i] + nums[j] == 2020) {
                product = nums[i] * nums[j];
            }
        }
    }

    return product;

}

int part2(int *nums, int numLines) {
    int product;

    for (int i = 0; i < numLines - 2; i++) {
        for (int j = i + 1; j < numLines - 1; j++) {
            for (int k = j + 1; k < numLines; k++) {
                if (nums[i] + nums[j] + nums[k] == 2020) {
                    product = nums[i] * nums[j] * nums[k];
                }
            }
        }
    }

    return product;
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    int numLines, product;
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
    printf("\nPart 1:\nProduct of numbers that sum to 2020: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    int p2 = part2(&nums[0], numLines);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nProduct of numbers that sum to 2020: %d\nRan in %f seconds\n", p2, t_p2);

	return 0;
}