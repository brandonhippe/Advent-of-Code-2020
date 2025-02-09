#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define defaultInput "../../Inputs/2020_20.txt"
#define outputFile "input2.txt"
#define monsterFile "monster.txt"
#define monsterLen 21

int size, side, dataLine;


typedef struct tile {
    int id, match[8], type, loc;
    struct tile *sides[4];
    char image[102];
} Tile;

void createTile(struct tile *t, int id_, char *img) {
    t->id = id_;
    t->type = 0;
    t->loc = -1;
    if (strlen(img) > 101) {
        printf("Error: Image too large: %ld.\n", strlen(img));
        return;
    }

    strncpy(t->image, img, strlen(img));

    for (int i = 0; i < 8; i++) {
        t->match[i] = 0;
        t->sides[i / 2] = 0;
    }

    return;
}

void printTile(struct tile *t) {
    printf("Id: %d\n", t->id);
    char *img = (char*)calloc((dataLine * dataLine) + 2, sizeof(char)), *p = img, *temp = (char*)calloc(dataLine + 2, sizeof(char));
    strcpy(img, t->image);

    while (strlen(p) > 0) {
        strncpy(temp, p, dataLine);
        printf("%s\n", temp);
        p += dataLine * sizeof(char);
    }

    printf("\nMatches in each orientation:");
    for (int i = 0; i < 8; i++) {
        printf("\n%d", t->match[i]);
    }


    printf("\n");
    //printf("Flip: %d ", t->flipped);
    printf("%s", t->type == 2 ? "***CORNER***" : t->type == 1 ? "***EDGE***" : "***CENTER***");
    printf("\n\n");

    return;
}

int findLines(char *fileName) {
	int numLines = 0;
	char textRead[dataLine + 2];

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

	while(fgets(textRead, dataLine + 2, inFile)) {
        if (textRead[1] == 'i') {
            numLines++;
        }
	}

	fclose(inFile);

	return numLines;
}

void readData(struct tile **tiles, char *fileName) {
    int tNum = -1, id_;
	char textRead[dataLine + 2], *img = (char*)calloc((dataLine * dataLine) + 2, sizeof(char));

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return;
    }

	while(fgets(textRead, dataLine + 2, inFile)) {
        if (textRead[0] == 'T') {
            if (tNum >= 0) {
                img[strlen(img) - 1] = '\0';
                createTile(tiles[tNum], id_, img);
                strcpy(img, "");
            }

            char *p = strchr(textRead, ' ');
            id_ = atoi(p);
            ++tNum;
        } else {
            strncat(img, textRead, dataLine);
        }
	}

    img[strlen(img) - 1] = '\0';
    createTile(tiles[tNum], id_, img);

	fclose(inFile);

	return;
}

char *flip(char *str, int slen) {
    char *res = (char*)calloc(strlen(str) + 10, sizeof(char)), *p = str + ((slen - 1) * slen);
    
    // printf("Str len: %ld\n", strlen(str));

    while (p >= str) {
        // printf("P len: %ld\n", strlen(p));
        strncat(res, p, slen);
        p -= slen;
    }

    // strcat(res, "\n");
    // printf("Res len: %ld\n", strlen(res));

    return res;
}

char *rotate(char *str, int slen) {
    char *res = (char*)calloc(strlen(str) + 1, sizeof(char)), *p;
    int index = 0, i = 0;

    while (index < slen) {
        while (i < slen * slen) {
            p = str + i;
            if (i % slen == index) {
                strncat(res, p, 1);
            }
            i++;
        }

        i = 0;
        index++;
    }

    // strcat(res, "\n");

    return res;
}

void shiftArr(struct tile **arr, int len, int amt) {
    while (amt > 0) {
        struct tile *temp = arr[0];

        for (int i = 0; i < len - 1; i++) {
            arr[i] = arr[i + 1];
        }

        arr[len - 1] = temp;

        amt--;
    }

    return;
}

void shiftArr_int(int *arr, int len, int amt) {
    while (amt > 0) {
        int temp = arr[0];

        for (int i = 0; i < len - 1; i++) {
            arr[i] = arr[i + 1];
        }

        arr[len - 1] = temp;

        amt--;
    }

    return;
}

int indexFrom2d(int x, int y) {
    return (y * side) + x;
}

void matchSides(struct tile **tiles) {
    for (int n = 0; n < size; n++) {
        struct tile *t = tiles[n];

        for (int i = 0; i < 8; i++) {
           for (int j = 0; j < size; j++) {
                if (n == j) {
                    continue;
                }
                struct tile *t1 = tiles[j];

                for (int k = 0; k < 8; k++) {
                    if (strncmp(t->image, t1->image, dataLine) == 0) {
                        t->match[i]++;
                    }

                    strcpy(t1->image, (k % 2 == 0) ? flip(t1->image, dataLine) : rotate(t1->image, dataLine));
                }
            }

            strcpy(t->image, (i % 2 == 0) ? flip(t->image, dataLine) : rotate(t->image, dataLine));
        }
    }

    return;
}

void genMatches(struct tile *t, struct tile **tiles, int tloc) {
    t->loc = tloc;

    if ((t->match[1] == 1 || t->match[4] == 1) && t->sides[2] == 0) {
        // Match bottom

        int search = tloc + side;

        bool found = false;
        for (int i = 0; i < size; i++) {
            struct tile *t1 = tiles[i];
            if (t1->loc == search) {
                t->sides[2] = t1;
                t1->sides[0] = t;
                found = true;
            }
        }

        if (!found) {
            char *endT = (char*)calloc(dataLine + 1, sizeof(char));
            strncpy(endT, t->image + dataLine * (dataLine - 1), dataLine);

            for (int i = 0; i < size; i++) {
                if (tiles[i] == t) {
                    continue;
                }

                struct tile *t1 = tiles[i];

                for (int or = 0; or < 8; or++) {
                    char *startT1 = (char*)calloc(dataLine + 1, sizeof(char));
                    strncpy(startT1, t1->image, dataLine);

                    if (strcmp(endT, startT1) == 0) {
                        t->sides[2] = t1;
                        t1->sides[0] = t;
                        genMatches(t1, tiles, search);

                        found = true;
                        break;
                    }

                    strcpy(t1->image, (or % 2 == 0) ? flip(t1->image, dataLine) : rotate(t1->image, dataLine));
                    shiftArr_int(t1->match, 8, 1);
                    shiftArr(t1->sides, 4, or % 2);
                }

                if (found) {
                    break;
                }
            }
        }
    }

    if ((t->match[3] == 1 || t->match[6] == 1) && t->sides[1] == 0) {
        // Match right

        int search = tloc + 1;

        bool found = false;
        for (int i = 0; i < size; i++) {
            struct tile *t1 = tiles[i];
            if (t1->loc == search) {
                t->sides[2] = t1;
                t1->sides[0] = t;
                found = true;
            }
        }

        if (!found) {
            char *endT = (char*)calloc(dataLine + 1, sizeof(char));
            for (int i = 0; i < dataLine; i++) {
                strncat(endT, t->image + i * dataLine + 9, 1);
            }

            for (int i = 0; i < size; i++) {
                if (tiles[i] == t) {
                    continue;
                }

                struct tile *t1 = tiles[i];

                for (int or = 0; or < 8; or++) {
                    char *startT1 = (char*)calloc(dataLine + 1, sizeof(char));
                    for (int i = 0; i < dataLine; i++) {
                        strncat(startT1, t1->image + i * dataLine, 1);
                    }

                    if (strcmp(endT, startT1) == 0) {
                        t->sides[1] = t1;
                        t1->sides[3] = t;
                        genMatches(t1, tiles, search);

                        found = true;
                        break;
                    }

                    strcpy(t1->image, (or % 2 == 0) ? flip(t1->image, dataLine) : rotate(t1->image, dataLine));
                    shiftArr_int(t1->match, 8, 1);
                    shiftArr(t1->sides, 4, or % 2);
                }

                if (found) {
                    break;
                }
            }
        }
    }

    return;
}

void formatImg(struct tile *s, char *img) {
    struct tile *curr = 0, *next = s;

    do {
        for (int line = 1; line < 9; line++) {
            do {
                if (curr == 0) {
                    curr = next;
                } else {
                    curr = curr->sides[1];
                }

                char *temp = curr->image + line * dataLine + 1, *p = (char*)calloc(9, sizeof(char));
                strncat(img, temp, 8);
                strncpy(p, temp, 8);
            } while (curr->sides[1] != 0);

            while (curr->sides[3] != 0) {
                curr = curr->sides[3];
            }

            next = curr;
            curr = 0;
        }

        next = next->sides[2];
    } while (next);

    return;
}

void genMonster(char *monster) {
    char textRead[monsterLen + 1];

	// Open the file
	FILE *inFile = fopen(monsterFile, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return;
    }

	while(fgets(textRead, dataLine, inFile)) {
        strncat(monster, textRead, monsterLen - 1);
	}

	fclose(inFile);

	return;
}

void writeFile(char *img) {
    char *p = img;
	FILE *outFile = NULL;

	outFile = fopen(outputFile, "w");

	// Check if file could be opened
	if (outFile == NULL) {
		printf("Couldn't write to file.\n");
		return;
	}

	while (p < img + strlen(img)) {
        char *temp = (char*)calloc(8 * side + 1, sizeof(char));
        strncpy(temp, p, 8 * side);
        fprintf(outFile, "%s\n", temp);
        p += 8 * side;
	}

	// Close file
	fclose(outFile);

	return;
}

int roughness(char *img) {
    char monster[] = "                  # #    ##    ##    ### #  #  #  #  #  #   ";

    for (int or = 0; or < 8; or++) {
        for (int i = 0; i < side - 2; i++) {
            for (int j = 0; j <= side - monsterLen + 1; j++) {
                int start = i * side + j, mIndex = 0;

                bool found = true;
                while (monster[mIndex] && found) {
                    if (monster[mIndex] == '#' && img[start + side * (mIndex / (monsterLen - 1)) + (mIndex % (monsterLen - 1))] == '.') {
                        found = false;
                    }

                    mIndex++;
                }

                if (found) {
                    mIndex = 0;
                    while (monster[mIndex]) {
                        if (monster[mIndex] == '#') {
                            img[start + side * (mIndex / (monsterLen - 1)) + (mIndex % (monsterLen - 1))] = 'O';
                        }

                        mIndex++;
                    }
                }
            }
        }

        strcpy(img, (or % 2 == 0) ? flip(img, side) : rotate(img, side));
    }

    char *p = img;
    for (int i = 0; i < side; i++) {
        char *temp = (char*)calloc(dataLine, sizeof(char));
        strncpy(temp, p, side);
        p += side;
    }

    int i = 0, count = 0;
    while (img[i]) {
        count += (img[i] == '#') ? 1 : 0;
        i++;
    }

    return count;
}

unsigned long long int part1(struct tile **tiles) {
    matchSides(tiles);

    for (int i = 0; i < size; i++) {
        struct tile *t = tiles[i];
        int zeros = 0;

        for (int j = 0; j < 8; j++) {
            zeros += (t->match[j] == 0) ? 1 : 0;
        }

        t->type = zeros / 2;
    }

    unsigned long long int product = 1;
    for (int i = 0; i < size; i++) {
        struct tile *t = tiles[i];

        if (t->type == 2) {
            product *= t->id;
        }
    }

    return product;
}

int part2(struct tile **tiles) {
    struct tile *c0;

    for (int i = 0; i < size; i++) {
        struct tile *t = tiles[i];
        if (t->type == 2) {
            for (int or = 0; or < 8 && t->match[1] + t->match[3] + t->match[4] + t->match[6] != 4; or++) {
                strcpy(t->image, (or % 2 == 0) ? flip(t->image, dataLine) : rotate(t->image, dataLine));
                shiftArr_int(t->match, 8, 1);
            }

            c0 = t;
            break;
        }
    }

    genMatches(c0, tiles, 0);
    struct tile *curr = 0, *next = c0;

    do {
        if (curr != 0) {
            next = curr->sides[2];
            curr = 0;
        }

        do {
            if (curr == 0) {
                curr = next;
            } else {
                curr = curr->sides[1];
            }
        } while (curr->sides[1] != 0);

        while (curr->sides[3] != 0) {
            curr = curr->sides[3];
        }
    } while (curr->sides[2] != 0);

    char *img = (char*)calloc(8 * 8 * side * side + 2, sizeof(char));
    formatImg(c0, img);

    char *p = img;
    while (p < img + strlen(img)) {
        char *temp = (char *)calloc(8 * side + 2, sizeof(char));
        strncpy(temp, p, 8 * side);
        p += 8 * side;
    }

    side = 96;
    dataLine = 100;

    return roughness(img);
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    dataLine = 10;
    size = findLines(inputPath), side = sqrt(size);

    struct tile **tiles = (struct tile **)calloc(size, sizeof(struct tile *));
    for (int i = 0; i < size; i++) {
        tiles[i] = (struct tile *)calloc(1, sizeof(struct tile));
    }

    readData(tiles, inputPath);

    clock_t t;
    t = clock(); 
    unsigned long long int p1 = part1(tiles);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nProduct of corner IDs: %llu\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    int p2 = part2(tiles);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nWater Roughness: %d\nRan in %f seconds\n", p2, t_p2);

    return 0;
}