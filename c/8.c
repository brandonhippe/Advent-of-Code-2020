#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#define defaultInput "../../Inputs/2020_8.txt"
#define dataLine 15

typedef struct code {
	int num, op;
	bool visited;
} Code;

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

void readData(struct code *codeLines, char *fileName) {
    int lineNum = 0;
	char textRead[dataLine], *p;

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return;
    }

	while(fgets(textRead, dataLine, inFile)) {
        p = strtok(textRead, " ");

        if (strncmp(textRead, "nop", 3) == 0) {
            codeLines[lineNum].op = 1;
        } else if (strncmp(textRead, "acc", 3) == 0) {
            codeLines[lineNum].op = 0;
        } else if (strncmp(textRead, "jmp", 3) == 0) {
            codeLines[lineNum].op = -1;
        }

        p = strtok(NULL, "\n");

        codeLines[lineNum].num = atoi(p);
        codeLines[lineNum].visited = false;

        lineNum++;
	}

	fclose(inFile);

    return;
}

bool runCode(struct code *codeLines, int lineNum, int numLines, int* acc) {
    if (lineNum >= numLines) {
        return true;
    }

    if (codeLines[lineNum].visited) {
        return false;
    }

    codeLines[lineNum].visited = true;

    switch (codeLines[lineNum].op) {
        case -1:
            lineNum += codeLines[lineNum].num;
            break;
        case 0:
            *acc += codeLines[lineNum].num;
        default:
            lineNum++;
    }

    return runCode(codeLines, lineNum, numLines, acc);
}

int part1(struct code *codeLines, int numLines) {
    int acc = 0;
    runCode(codeLines, 0, numLines, &acc);
    return acc;
}

int part2(struct code *codeLines, int numLines) {
    int acc = 0;

    for (int i = 0; i < numLines; i++) {
        if (codeLines[i].op != 0) {
            codeLines[i].op *= -1;

            acc = 0;
            if (runCode(codeLines, 0, numLines, &acc)) {
                break;
            }

            codeLines[i].op *= -1;

            for (int j = 0; j < numLines; j++) {
                codeLines[j].visited = false;
            }
        }
    }

    return acc;
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    int numLines;
    struct code *codeLines;
	numLines = findLines(inputPath);

	if (numLines == -1) {
        printf("Error: Could not read input file. Quitting\n");
        return -1;
	}

	codeLines = (Code *)calloc(numLines, sizeof(Code));
    readData(codeLines, inputPath);

    clock_t t; 
    t = clock(); 
    int p1 = part1(codeLines, numLines);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nAccumulator: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    int p2 = part2(codeLines, numLines);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nAccumulator: %d\nRan in %f seconds\n", p2, t_p2);

	return 0;
}