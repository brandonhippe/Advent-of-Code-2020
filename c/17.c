#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define defaultInput "../../Inputs/2020_17.txt"
#define dataLine 10

int numLines, iterations, wLen, xLen, yLen, zLen;
int dist_p1(int x1, int y1, int z1, int x2, int y2, int z2);
int dist_p2(int x1, int y1, int z1, int w1, int x2, int y2, int z2, int w2);

typedef struct cube_p1 {
    struct cube_p1 *neighbors[27];
    int x_, y_, z_, nIndex, state;
} cube_p1;

void printCube_p1(struct cube_p1 *c, bool printNeighbors) {
    printf("cube Addr: %p, X: %d, Y: %d, Z: %d\n", c, c->x_, c->y_, c->z_);
    //printf("%d\n", c->x_);
    //printf("%d\n", c->y_);
    //printf("%d\n", c->z_);
    //printf("%d\n", c->arr_);
    printf("State: %s\n", (c->state == 1) ? "true" : "false");

    if (printNeighbors) {
        printf("Neighbors:\n");
        for (int i = 0; i < 27; i++) {
            struct cube_p1 *c1 = c->neighbors[i];
            printf("Index: %d\n", i);
            if (c1) {
                printCube_p1(c1, false);
            } else {
                printf("No neighbor.\n");
            }
        }

        printf("\n\n\n");
    }

    return;
}

void printCubes_p1(struct cube_p1 **cubes) {
    for (int z = 0; z < zLen; z++) {
        printf("Z: %d\n", z - iterations);
        for (int y = 0; y < yLen; y++) {
            for (int x = 0; x < xLen; x++) {
                for (int i = 0; i < xLen * yLen * zLen; i++) {
                    struct cube_p1 *c = cubes[i];
                    int cx = c->x_, cy = c->y_, cz = c->z_, cs = c->state;

                    if (dist_p1(x - iterations, y - iterations, z - iterations, cx, cy, cz) == 0) {
                        printf("%s", (cs == 1) ? "#" : ".");
                        break;
                    }
                }
            }

            printf("\n");
        }

        printf("\n\n");
    }

    return;
}

struct cube_p1 *createCube_p1(struct cube_p1 *c, int x, int y, int z) {
    c->x_ = x;
    c->y_ = y;
    c->z_ = z;
    c->state = 0;
    c->nIndex = 0;

    for (int i = 0; i < 27; i++) {
        c->neighbors[i] = 0;
    }

    return c;
}

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
        numLines++;
	}

	fclose(inFile);

	return numLines;
}

void readData_p1(int *states, char *fileName) {
    int index = 0;
	char textRead[dataLine];

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	while(fgets(textRead, dataLine, inFile)) {
        for (int i = 0; i < numLines; i++) {
            char temp = textRead[i];
            if (temp != '\n') {
                int s = (temp == '#') ? 1 : 0;
                states[index] = s;
                index++;
            }
        }
	}

	fclose(inFile);

	return;
}

int dist_p1(int x1, int y1, int z1, int x2, int y2, int z2) {
    return (pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
}

void assignNeighbors_p1(struct cube_p1 **cubes) {
    for (int i = 0; i < xLen * yLen * zLen - 1; i++) {
        struct cube_p1 *c = cubes[i];
        int cx = c->x_, cy = c->y_, cz = c->z_;

        for (int j = i + 1; j < xLen * yLen * zLen; j++) {
            struct cube_p1 *n = cubes[j];
            int nx = n->x_, ny = n->y_, nz = n->z_;

            if (dist_p1(cx, cy, cz, nx, ny, nz) <= 3) {
                c->neighbors[c->nIndex] = n;
                n->neighbors[n->nIndex] = c;
                c->nIndex++;
                n->nIndex++;
            }
        }
    }

    return;
}

typedef struct cube_p2 {
    struct cube_p2 *neighbors[81];
    int x_, y_, z_, w_, nIndex, state;
} Cube;

void printCube_p2(struct cube_p2 *c, bool printNeighbors) {
    printf("Cube Addr: %p, X: %d, Y: %d, Z: %d, W: %d\n", c, c->x_, c->y_, c->z_, c->w_);
    //printf("%d\n", c->x_);
    //printf("%d\n", c->y_);
    //printf("%d\n", c->z_);
    //printf("%d\n", c->arr_);
    printf("State: %s\n", (c->state == 1) ? "true" : "false");

    if (printNeighbors) {
        printf("Neighbors:\n");
        for (int i = 0; i < 27; i++) {
            struct cube_p2 *c1 = c->neighbors[i];
            printf("Index: %d\n", i);
            if (c1) {
                printCube_p2(c1, false);
            } else {
                printf("No neighbor.\n");
            }
        }

        printf("\n\n\n");
    }

    return;
}

void printCubes_p2(struct cube_p2 **cubes) {
    for (int w = 0; w < wLen; w++) {
        for (int z = 0; z < zLen; z++) {
            printf("Z: %d, W: %d\n", z - iterations, w - iterations);
            for (int y = 0; y < yLen; y++) {
                for (int x = 0; x < xLen; x++) {
                    for (int i = 0; i < xLen * yLen * zLen * wLen; i++) {
                        struct cube_p2 *c = cubes[i];
                        int cx = c->x_, cy = c->y_, cz = c->z_, cw = c->w_, cs = c->state;

                        if (dist_p2(x - iterations, y - iterations, z - iterations, w - iterations, cx, cy, cz, cw) == 0) {
                            printf("%s", (cs == 1) ? "#" : ".");
                            break;
                        }
                    }
                }

                printf("\n");
            }
        }

        printf("\n\n");
    }

    return;
}

struct cube_p2 *createCube_p2(struct cube_p2 *c, int x, int y, int z, int w) {
    c->x_ = x;
    c->y_ = y;
    c->z_ = z;
    c->w_ = w;
    c->state = 0;
    c->nIndex = 0;

    for (int i = 0; i < 81; i++) {
        c->neighbors[i] = 0;
    }

    return c;
}

void readData_p2(int *states, char *fileName) {
    int index = 0;
	char textRead[dataLine];

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	while(fgets(textRead, dataLine, inFile)) {
        for (int i = 0; i < numLines; i++) {
            char temp = textRead[i];
            if (temp != '\n') {
                int s = (temp == '#') ? 1 : 0;
                states[index] = s;
                index++;
            }
        }
	}

	fclose(inFile);

	return;
}

int dist_p2(int x1, int y1, int z1, int w1, int x2, int y2, int z2, int w2) {
    int d[4] = {abs(x1 - x2), abs(y1 - y2), abs(z1 - z2), abs(w1 - w2)}, maxD = 0;
    for (int i = 0; i < 4; i++) {
        if (d[i] > maxD) {
            maxD = d[i];
        }
    }

    return maxD;
}

void assignNeighbors_p2(struct cube_p2 **cubes) {
    for (int i = 0; i < xLen * yLen * zLen * wLen - 1; i++) {
        struct cube_p2 *c = cubes[i];
        int cx = c->x_, cy = c->y_, cz = c->z_, cw = c->w_;

        for (int j = i + 1; j < xLen * yLen * zLen * wLen; j++) {
            struct cube_p2 *n = cubes[j];
            int nx = n->x_, ny = n->y_, nz = n->z_, nw = n->w_;

            if (dist_p2(cx, cy, cz, cw, nx, ny, nz, nw) <= 1) {
                c->neighbors[c->nIndex] = n;
                n->neighbors[n->nIndex] = c;
                c->nIndex++;
                n->nIndex++;
            }
        }
    }

    return;
}

int part1(char *fileName) {
    iterations = 6;
    xLen = numLines + 2 * iterations;
    yLen = numLines + 2 * iterations;
    zLen = 1 + 2 * iterations;

    if (numLines < 0) {
        printf("Error: Could not open file.\n");
        return -1;
    }

    struct cube_p1 **cubes = (struct cube_p1**)calloc(xLen * yLen * zLen, sizeof(struct cube_p1*));
    int z = -iterations - 1, y = -iterations, x = -iterations;
    for (int i = 0; i < xLen * yLen * zLen; i++) {
        if (i % xLen == 0) {
            y++;
            x = -iterations;
        }
        if (i % (xLen * yLen) == 0) {
            z++;
            y = -iterations;
        }

        struct cube_p1 *c = (cube_p1*)calloc(1, sizeof(cube_p1));
        cubes[i] = createCube_p1(c, x, y, z);

        x++;
    }

    int initial[numLines * numLines];
    readData_p1(&initial[0], fileName);

    y = -1;
    z = 0;
    for (int i = 0; i < numLines * numLines; i++) {
        x = i % numLines;
        if (x == 0) {
            y++;
        }

        for (int j = 0; j < xLen * yLen * zLen; j++) {
            struct cube_p1 *c = cubes[j];
            int cx = c->x_, cy = c->y_, cz = c->z_;
            if (dist_p1(x, y, z, cx, cy, cz) == 0) {
                c->state = initial[i];
            }
        }
    }

    assignNeighbors_p1(cubes);

    for (int loops = 0; loops < iterations; loops++) {
        int *nextStates = (int*)calloc(xLen * yLen * zLen, sizeof(int));

        for (int i = 0; i < xLen * yLen * zLen; i++) {
            struct cube_p1 *c = cubes[i], *c1;
            int neighborCount = 0, j = 0;

            c1 = c->neighbors[j];
            while (c1) {
                neighborCount += (c1->state) ? 1 : 0;
                j++;
                c1 = c->neighbors[j];
            }

            if (c->state == 0) {
                if (neighborCount == 3) {
                    nextStates[i] = 1;
                } else {
                    nextStates[i] = 0;
                }
            } else {
                if (neighborCount == 2 || neighborCount == 3) {
                    nextStates[i] = 1;
                } else {
                    nextStates[i] = 0;
                }
            }
        }

        for (int i = 0; i < xLen * yLen * zLen; i++) {
            struct cube_p1 *c = cubes[i];
            c->state = nextStates[i];
        }

        free(nextStates);
    }

    int count = 0;

    for (int i = 0; i < xLen * yLen * zLen; i++) {
        struct cube_p1 *c = cubes[i];
        count += c->state;
    }

    return count;
}

int part2(char *fileName) {
    xLen = numLines + 2 * iterations;
    yLen = numLines + 2 * iterations;
    zLen = 1 + 2 * iterations;
    wLen = 1 + 2 * iterations;

    if (numLines < 0) {
        printf("Error: Could not open file.\n");
        return -1;
    }

    struct cube_p2 **cubes = (struct cube_p2 **)calloc(xLen * yLen * zLen * wLen, sizeof(struct cube_p2 *));
    int w = -iterations - 1, z = -iterations, y = -iterations, x = -iterations;
    for (int i = 0; i < xLen * yLen * zLen * wLen; i++) {
        if (i % xLen == 0) {
            y++;
            x = -iterations;
        }
        if (i % (xLen * yLen) == 0) {
            z++;
            y = -iterations;
        }
        if (i % (xLen * yLen * zLen) == 0) {
            w++;
            z = -iterations;
        }

        struct cube_p2 *c = (Cube*)calloc(1, sizeof(Cube));
        cubes[i] = createCube_p2(c, x, y, z, w);

        x++;
    }

    int initial[numLines * numLines];
    readData_p2(&initial[0], fileName);

    y = -1;
    z = 0;
    w = 0;
    for (int i = 0; i < numLines * numLines; i++) {
        x = i % numLines;
        if (x == 0) {
            y++;
        }

        for (int j = 0; j < xLen * yLen * zLen * wLen; j++) {
            struct cube_p2 *c = cubes[j];
            int cx = c->x_, cy = c->y_, cz = c->z_, cw = c->w_;
            if (dist_p2(x, y, z, w, cx, cy, cz, cw) == 0) {
                c->state = initial[i];
            }
        }
    }

    assignNeighbors_p2(cubes);

    for (int loops = 0; loops < iterations; loops++) {
        int *nextStates = (int*)calloc(xLen * yLen * zLen * wLen, sizeof(int));

        for (int i = 0; i < xLen * yLen * zLen * wLen; i++) {
            struct cube_p2 *c = cubes[i], *c1;
            int neighborCount = 0, j = 0;

            c1 = c->neighbors[j];
            while (c1) {
                neighborCount += (c1->state) ? 1 : 0;
                j++;
                c1 = c->neighbors[j];
            }

            if (c->state == 0) {
                if (neighborCount == 3) {
                    nextStates[i] = 1;
                } else {
                    nextStates[i] = 0;
                }
            } else {
                if (neighborCount == 2 || neighborCount == 3) {
                    nextStates[i] = 1;
                } else {
                    nextStates[i] = 0;
                }
            }
        }

        for (int i = 0; i < xLen * yLen * zLen * wLen; i++) {
            struct cube_p2 *c = cubes[i];
            c->state = nextStates[i];
        }

        free(nextStates);
    }

    int count = 0;

    for (int i = 0; i < xLen * yLen * zLen * wLen; i++) {
        struct cube_p2 *c = cubes[i];
        count += c->state;
    }

    return count;
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    numLines = findLines(inputPath);

    clock_t t; 
    t = clock(); 
    int p1 = part1(inputPath);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nActive cubes: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    int p2 = part2(inputPath);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nActive cubes: %d\nRan in %f seconds\n", p2, t_p2);

    return 0;
}