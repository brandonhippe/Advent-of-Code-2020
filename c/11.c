#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#define defaultInput "../../Inputs/2020_11.txt"
#define dataLine 105

int findLines(int *len, char *fileName) {
	int numLines = 0;
	char textRead[dataLine];

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

	while(fgets(textRead, dataLine, inFile)) {
        *len = strlen(textRead);
        ++numLines;
	}

	fclose(inFile);

	return numLines;
}

void readData(int *nums, int line, char *fileName) {
    int lineNum = 0, index;
	char textRead[dataLine];

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return;
    }

	while(fgets(textRead, dataLine, inFile)) {
        index = 0;
        while (textRead[index] != '\n') {
            switch (textRead[index]) {
                case 'L':
                    *(nums + (line * lineNum) + index) = -1;
                    break;
                case '.':
                    *(nums + (line * lineNum) + index) = 0;
                    break;
                case '#':
                    *(nums + (line * lineNum) + index) = 1;
                    break;
            }

            index++;
        }

        lineNum++;
	}

	fclose(inFile);

    return;
}

void cpyArr(int *arr1, int *arr2, int size) {
    for (int i = 0; i < size; i++) {
        *(arr1 + i) = *(arr2 + i);
    }

    return;
}

int countOcc(int *arr, int size) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (*(arr + i) > 0) {
            count++;
        }
    }

    return count;
}

void printSeats(int *seats, int numLines, int line) {
    for (int i = 0; i < numLines; i++) {
        for (int j = 0; j < line; j++) {
            printf("%s", (*(seats + (line * i) + j) > 0) ? "#" : ((*(seats + (line * i) + j) < 0) ? "L" : "."));
        }
        printf("\n");
    }

    printf("\n\n");

    return;
}

bool iterate_p1(int *seats, int lines, int line) {
    int newSeats[lines][line];
    bool changed = false;

    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < line; j++) {
            if (*(seats + (line * i) + j) == 0) {
                newSeats[i][j] = 0;
                continue;
            }

            int surr = 0;
            for (int m = -1; m <= 1; m++) {
                if (i + m >= 0 && i + m < lines) {
                    for (int n = -1; n <= 1; n++) {
                        if (j + n >= 0 && j + n < line && !(m == 0 && n == 0)) {
                            if (*(seats + (line * (i + m)) + (j + n)) > 0) {
                                surr++;
                            }
                        }
                    }
                }
            }

            if (((*(seats + (line * i) + j) < 0) && surr == 0) || ((*(seats + (line * i) + j) > 0) && surr >= 4)) {
                newSeats[i][j] = *(seats + (line * i) + j) * -1;
                changed = true;
            } else {
                newSeats[i][j] = *(seats + (line * i) + j);
            }
        }
    }

    cpyArr(seats, &newSeats[0][0], lines * line);
    return changed;
}

bool iterate_p2(int *seats, int lines, int line) {
    bool changed = false;
    int newSeats[lines][line];

    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < line; j++) {
            if (*(seats + (line * i) + j) == 0) {
                newSeats[i][j] = 0;
                continue;
            }

            int surr = 0;
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    if (m == 0 && n == 0) {
                        continue;
                    }

                    for (int mult = 1; i + (m * mult) >= 0 && i + (m * mult) < lines && j + (n * mult) >= 0 && j + (n * mult) < line; mult++) {
                        if (*(seats + (line * (i + (m * mult))) + (j + (n * mult))) != 0) {
                            surr += (*(seats + (line * (i + (m * mult))) + (j + (n * mult))) + 1) / 2;
                            break;
                        }
                    }
                }
            }

            if (((*(seats + (line * i) + j) < 0) && surr == 0) || ((*(seats + (line * i) + j) > 0) && surr >= 5)) {
                newSeats[i][j] = *(seats + (line * i) + j) * -1;
                changed = true;
            } else {
                newSeats[i][j] = *(seats + (line * i) + j);
            }
        }
    }

    cpyArr(seats, &newSeats[0][0], lines * line);
    return changed;
}

int part1(int *seats, int lines, int line) {
    bool changed;
    do {
        changed = iterate_p1(seats, lines, line);
    } while (changed);

    return countOcc(seats, lines * line);
}

int part2(int *seats, int lines, int line) {
    bool changed;

    do {
        changed = iterate_p2(seats, lines, line);
    } while (changed);

    return countOcc(seats, lines * line);
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    int numLines, len = 0;
	numLines = findLines(&len, inputPath);
	len--;

	if (numLines == -1) {
        printf("Error: Could not read input file. Quitting\n");
        return -1;
	}

	int seats[numLines][len];
	readData(&seats[0][0], len, inputPath);

    clock_t t; 
    t = clock(); 
    int p1 = part1(&seats[0][0], numLines, len);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nNumber of occupied seats: %d\nRan in %f seconds\n", p1, t_p1);

    readData(&seats[0][0], len, inputPath);
    t = clock();
    int p2 = part2(&seats[0][0], numLines, len);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nNumber of occupied seats: %d\nRan in %f seconds\n", p2, t_p2);

	return 0;
}