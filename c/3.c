#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#define dataLine 33
#define defaultInput "../../Inputs/2020_3.txt"

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

int part1(char *fileName) {
	int numLines, count = 0;
	int slope[2] = {3, 1};
	int loc[2] = {0, 0};

	numLines = findLines(fileName);

	if (numLines == -1) {
		printf("Error: could not open file. Quitting\n");
		return -1;
	}

	char textRead[dataLine];
	int lineNum = 0;

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	while(fgets(textRead, dataLine, inFile)) {
        int charIndex = loc[0] % (dataLine - 2);
        if (textRead[charIndex] == '#') {
            count++;
            textRead[charIndex] = 'X';
        } else {
            textRead[charIndex] = 'O';
        }

        loc[0] += slope[0];
        loc[1] += slope[1];
	}

	fclose(inFile);

    return count;
}

int part2(char *fileName) {
	int numLines, count, product = 1;
	int slope[2];
	int loc[2];


	numLines = findLines(fileName);

	if (numLines == -1) {
		printf("Error: could not open file. Quitting\n");
		return -1;
	}

    for (int i = 0; i < 5; i++) {
        if (i == 0) {
            slope[0] = 1;
            slope[1] = 1;
        } else if (i == 4) {
            slope[0] = 1;
            slope[1] = 2;
        } else {
            slope[0] += 2;
        }

        loc[0] = 0;
        loc[1] = 0;

        count = 0;

        char textRead[dataLine];
        int lineNum = -1;

        // Open the file
        FILE *inFile = fopen(fileName, "r");

        while(fgets(textRead, dataLine, inFile)) {
            lineNum++;
            if (lineNum != loc[1]) {
                continue;
            }
            int charIndex = loc[0] % (dataLine - 2);
            if (textRead[charIndex] == '#') {
                count++;
                textRead[charIndex] = 'X';
            } else {
                textRead[charIndex] = 'O';
            }

            loc[0] += slope[0];
            loc[1] += slope[1];
        }

        fclose(inFile);

        // printf("Trees Hit: %d\n", count);

        product *= count;
    }

    return product;
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
    printf("\nPart 1:\nTrees hit: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    int p2 = part2(inputPath);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nProduct of trees hit at different slopes: %d\nRan in %f seconds\n", p2, t_p2);

	return 0;
}