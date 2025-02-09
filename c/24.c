#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define defaultInput "../../Inputs/2020_24.txt"
#define dataLine 100

typedef struct tile {
    bool state;
    int q, r, s; // Col, row, 3rd cube coordinate
} Tile;

Tile *createTile(int col, int row, bool side) {
    Tile *t = (Tile*)calloc(1, sizeof(Tile));
    t->state = side;
    t->q = col;
    t->r = row;
    t->s = -t->q - t->r;

    return t;
}

void printTile(Tile *t) {
    printf("Tile %d,%d: Color: %s\n", t->q, t->r, (t->state) ? "black" : "white");
    return;
}

int getIndex(int col, int row, int max) {
    col += max;
    row += max;

    return (row * ((2 * max) + 1)) + col;
}

Tile **readData(int *numT, int *maxVal, char *fileName) {
	char textRead[dataLine];
	Tile **tiles = (Tile **)calloc(1, sizeof(Tile *));
	int numTiles = 0, allocated = 1, max = 0;

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return NULL;
    }

	while(fgets(textRead, dataLine, inFile)) {
        char *p = &textRead[0];
        int col = 0, row = 0;

        while (strlen(p) > 1) {
            switch (p[0]) {
                case 'n':
                    row++;
                    col += (int)(p[1] == 'e');
                    p++;
                    break;
                case 's':
                    row--;
                    col -= (int)(p[1] == 'w');
                    p++;
                    break;
                case 'e':
                    col++;
                    break;
                case 'w':
                    col--;
                    break;
            }

            p++;
        }

        if (abs(col) > max)
            max = abs(col);

        if (abs(row) > max)
            max = abs(row);

        if (abs(-col - row) > max)
            max = abs(-col - row);

        bool inTiles = false;
        Tile *t;

        for (int i = 0; i < numTiles; i++) {
            t = tiles[i];
            if (t->q == col && t->r == row) {
                inTiles = true;
                break;
            }
        }

        if (inTiles) {
            t->state = !t->state;
        } else {
            if (allocated == numTiles) {
                allocated *= 2;
                tiles = realloc(tiles, allocated * sizeof(Tile *));
            }

            tiles[numTiles] = createTile(col, row, true);
            numTiles++;
        }
	}

	fclose(inFile);

	*(numT) = numTiles;
	*(maxVal) = max;

	return tiles;
}

int countNeighbors(int col, int row, bool *states, int max) {
    int count = 0;
    int axial_directions[6][2] = {
        {1, 0}, {0, -1}, {-1, -1},
        {-1, 0}, {0, 1}, {1, 1}
    };

    for (int i = 0; i < 6; i++) {
        int c1 = col + axial_directions[i][0];
        int r1 = row + axial_directions[i][1];
        int s1 = -c1 - r1;

        if (abs(c1) > max || abs(r1) > max || abs(s1) > max)
            continue;

        int index = getIndex(c1, r1, max);
        count += (int)(states[index]);
    }


    return count;
}

int part1(char *fileName) {
    int numTiles, max;
    Tile **tiles = readData(&numTiles, &max, fileName);

    int count = 0;

    for (int i = 0; i < numTiles; i++) {
        Tile *t = tiles[i];
        count += (int)(t->state);
    }

    return count;
}

int part2(char *fileName) {
    int numTiles, max;
    Tile **tiles = readData(&numTiles, &max, fileName);
    int size = ((2 * max) + 1) * ((2 * max) + 1), allocated;
    bool *states = (bool*)calloc(size, sizeof(bool)), resize = true;

    for (int day = 0; day < 100; day++) {
        // Resize if necessary
        if (resize) {
            if (day != 0) {
                numTiles = 0;
                allocated = 1;
                tiles = (Tile **)calloc(allocated, sizeof(Tile *));

                for (int r = -max; r <= max; r++) {
                    for (int c = -max; c <= max; c++) {
                        int index = getIndex(c, r, max);

                        if (states[index]) {
                            if (numTiles == allocated) {
                                allocated *= 2;
                                tiles = realloc(tiles, allocated * sizeof(Tile *));
                            }

                            tiles[numTiles] = createTile(c, r, true);
                            numTiles++;
                        }
                    }
                }

                max++;
                size = ((2 * max) + 1) * ((2 * max) + 1);
                free(states);
                states = (bool*)calloc(size, sizeof(bool));
            }

            for (int i = 0; i < numTiles; i++) {
                Tile *t = tiles[i];
                int index = getIndex(t->q, t->r, max);

                states[index] = t->state;
            }

            free(tiles);
        }

        resize = false;

        // Print day's results
        // if (day < 10 || day % 10 == 0)
        //     printf("Day #%d: %d\n", day, countBlack(states, max));


        // Iteration of Game of Life Rules
        bool *newStates = (bool*)calloc(size, sizeof(bool));

        for (int r = -max; r <= max; r++) {
            for (int c = -max; c <= max; c++) {
                int index = getIndex(c, r, max);
                int count = countNeighbors(c, r, states, max);

                if (states[index]) {
                    // Black Tile Rules
                    if (count == 1 || count == 2) {
                        // Tile should be black
                        newStates[index] = true;
                    } else {
                        // Tile should be white
                        newStates[index] = false;
                    }
                } else {
                    // White Tile Rules
                    if (count == 2) {
                        // Tile should be black
                        newStates[index] = true;
                    } else {
                        // Tile should be white
                        newStates[index] = false;
                    }
                }
            }
        }


        // Determine quantities for next iteration
        for (int r = -max; r <= max; r++) {
            for (int c = -max; c <= max; c++) {
                int index = getIndex(c, r, max), s = -c - r;

                if (newStates[index] && (abs(r) == max || abs(c) == max || abs(s) == max)) {
                    resize = true;
                }

                states[index] = newStates[index];
            }
        }

        free(newStates);
    }
    
    int count = 0;

    for (int r = -max; r <= max; r++) {
        for (int c = -max; c <= max; c++) {
            int index = getIndex(c, r, max);
            count += states[index];
        }
    }

    return count;
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
    printf("\nPart 1:\n# of black tiles: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock(); 
    int p2 = part2(inputPath);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\n# of black tiles: %d\nRan in %f seconds\n", p2, t_p2);

    return 0;
}