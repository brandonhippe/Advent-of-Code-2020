#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define defaultInput "../../Inputs/2020_12.txt"
#define dataLine 10
#define PI 3.14159265

double degToRad(int deg);

typedef struct instruction {
    char action;
    int value;
} Instruction;

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

void readData(struct instruction *ins, char *fileName) {
    int lineNum = 0;
	char textRead[dataLine];

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return;
    }

	while(fgets(textRead, dataLine, inFile)) {
        ins[lineNum].action = textRead[0];
        char temp[strlen(textRead)];
        for (int i = 1; i < strlen(textRead); i++) {
            temp[i - 1] = textRead[i];
        }

        ins[lineNum].value = atoi(temp);

        lineNum++;
	}

	fclose(inFile);

    return;
}

void swap(int *arr, int a, int b) {
    int temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
    return;
}

void followIns_p1(struct instruction *ins, int *loc, int numIns) {
    int orientation = 0;
    int newLoc[2] = {0, 0};
    for (int i = 0; i < numIns; i++){
        //printf("%c%d\n", ins[i].action, ins[i].value);
        char tempAction;
        switch (ins[i].action) {
            case 'F':
                if (orientation % 360 == 0) {
                    tempAction = 'E';
                } else if (orientation % 180 == 0) {
                    tempAction = 'W';
                }

                if ((orientation + 90) % 360 == 0) {
                    tempAction = 'S';
                } else if ((orientation + 90) % 180 == 0) {
                    tempAction = 'N';
                }
                break;
            default:
                tempAction = ins[i].action;
        }

        switch (tempAction) {
            case 'N':
                newLoc[1] += ins[i].value;
                break;
            case 'S':
                newLoc[1] -= ins[i].value;
                break;
            case 'E':
                newLoc[0] += ins[i].value;
                break;
            case 'W':
                newLoc[0] -= ins[i].value;
                break;
            case 'L':
                orientation += ins[i].value;
                break;
            case 'R':
                orientation -= ins[i].value;
                break;
        }
    }
    loc[0] = newLoc[0];
    loc[1] = newLoc[1];

    return;
}

void followIns_p2(struct instruction *ins, int *shipLoc, int *wayLoc, int numIns) {
    for (int i = 0; i < numIns; i++){
        //printf("%c%d\n", ins[i].action, ins[i].value);
        int temp;
        switch (ins[i].action) {
            case 'N':
                wayLoc[1] += ins[i].value;
                break;
            case 'S':
                wayLoc[1] -= ins[i].value;
                break;
            case 'E':
                wayLoc[0] += ins[i].value;
                break;
            case 'W':
                wayLoc[0] -= ins[i].value;
                break;
            case 'L':
                temp = ins[i].value;
                while (temp > 0) {
                    swap(wayLoc, 0, 1);
                    wayLoc[0] *= -1;
                    temp -= 90;
                }
                break;
            case 'R':
                temp = ins[i].value;
                while (temp > 0) {
                    swap(wayLoc, 0, 1);
                    wayLoc[1] *= -1;
                    temp -= 90;
                }
                break;
            case 'F':
                shipLoc[0] += (ins[i].value * wayLoc[0]);
                shipLoc[1] += (ins[i].value * wayLoc[1]);
                break;
        }
    }

    return;
}

int manhatDist(int *loc) {
    return(abs(loc[0]) + abs(loc[1]));
}

int part1(Instruction *ins, int numLines) {
    int loc[2] = {0, 0};
    followIns_p1(ins, &loc[0], numLines);
    return manhatDist(&loc[0]);
}

int part2(Instruction *ins, int numLines) {
    int shipLoc[2] = {0, 0}, wayLoc[2] = {10, 1};
    followIns_p2(ins, &shipLoc[0], &wayLoc[0], numLines);
    return manhatDist(&shipLoc[0]);
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    int numLines;
    struct instruction *ins;

	numLines = findLines(inputPath);

	if (numLines == -1) {
        printf("Error: Could not read input file. Quitting\n");
        return -1;
	}

	ins = (Instruction *)calloc(numLines, sizeof(Instruction));
    readData(ins, inputPath);

    clock_t t; 
    t = clock(); 
    int p1 = part1(ins, numLines);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nManhattan Distance: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    int p2 = part2(ins, numLines);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nManhattan Distance: %d\nRan in %f seconds\n", p2, t_p2);

	return 0;
}