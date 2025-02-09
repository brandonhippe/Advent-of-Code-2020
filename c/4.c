#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#define defaultInput "../../Inputs/2020_4.txt"
#define dataLine 100


typedef struct passport_p1 {
	int birth, issue, expiration, height, pid, cid;
	char hair[20], eyes[20];
} Passport_p1;

typedef struct passport_p2 {
	int birth, issue, expiration, cid;
	char pid[20], height[20], hair[20], eyes[20];
} Passport_p2;

int findLines(char *fileName) {
	int numLines = 1;
	char textRead[dataLine];

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

	while(fgets(textRead, dataLine, inFile)) {
        if (textRead[0] == '\n') {
            ++numLines;
        }
	}

	fclose(inFile);

	return numLines;
}

bool valid_p1(struct passport_p1 *passports, int i) {
    if (passports[i].birth == -1) {
        return false;
    } else if (passports[i].pid == -1) {
        return false;
    } else if (passports[i].issue == -1) {
        return false;
    } else if (passports[i].expiration == -1) {
        return false;
    } else if (passports[i].height == -1) {
        return false;
    } else if ((strcmp(passports[i].eyes, "\n")) == 0) {
        return false;
    } else if ((strcmp(passports[i].hair, "\n")) == 0) {
        return false;
    }

    return true;
}

bool valid_p2(struct passport_p2 *passports, int i) {
    if (passports[i].birth < 1920 || passports[i].birth > 2002) {
        return false;
    } else if (passports[i].issue < 2010 || passports[i].issue > 2020) {
        return false;
    } else if (passports[i].expiration < 2020 || passports[i].expiration > 2030) {
        return false;
    }

    int num = 0, j = 0;
    char temp[20];
    strcpy(temp, passports[i].height);
    char tempc = passports[i].height[j];

    while (isdigit(tempc) != 0) {
        num *= 10;
        num += tempc - 48;
        j++;
        tempc = passports[i].height[j];
    }

    if (tempc == 'c') {
        if (num < 150 || num > 193) {
            return false;
        }
    } else if (tempc == 'i') {
        if (num < 59 || num > 76) {
            return false;
        }
    } else {
        return false;
    }

    if (passports[i].hair[0] != '#') {
        return false;
    }

    int count = 0;
    j = 1;
    while (isxdigit(passports[i].hair[j]) != 0) {
        count++;
        j++;
    }

    if (count != 6) {
        return false;
    }

    if (!(strncmp(passports[i].eyes, "amb", 3) == 0 || strncmp(passports[i].eyes, "blu", 3) == 0 || strncmp(passports[i].eyes, "brn", 3) == 0 || strncmp(passports[i].eyes, "gry", 3) == 0 || strncmp(passports[i].eyes, "grn", 3) == 0 || strncmp(passports[i].eyes, "hzl", 3) == 0 || strncmp(passports[i].eyes, "oth", 3) == 0)) {
        return false;
    }

    count = 0;
    j = 0;
    while (isdigit(passports[i].pid[j]) != 0) {
        count++;
        j++;
    }

    if (count != 9) {
        return false;
    }

    return true;
}

int part1(int numLines, char *fileName) {
    int count = 0;

    struct passport_p1 *passports;

    passports = (Passport_p1 *)calloc(numLines, sizeof(Passport_p1));
    for (int i = 0; i < numLines; i++) {
        passports[i].birth = -1;
        passports[i].cid = -1;
        passports[i].expiration = -1;
        strcpy(passports[i].eyes, "\n");
        strcpy(passports[i].hair, "\n");
        passports[i].height = -1;
        passports[i].issue = -1;
        passports[i].pid = -1;
    }

    int i = 0;
	char textRead[dataLine], *p;

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

	while(fgets(textRead, dataLine, inFile)) {
        if (textRead[0] == '\n') {
            i++;
        }

        p = strtok(textRead, ":");

        while (p != NULL) {
            char field[5], data[20];
            strcpy(field, p);

            p = strtok(NULL, " ");
            if (p == NULL) {
                break;
            }
            strcpy(data, p);

            if (strncmp(field, "byr", 3) == 0) {
                passports[i].birth = atoi(data);
            } else if (strncmp(field, "iyr", 3) == 0) {
                passports[i].issue = atoi(data);
            } else if (strncmp(field, "eyr", 3) == 0) {
                passports[i].expiration = atoi(data);
            } else if (strncmp(field, "hgt", 3) == 0) {
                passports[i].height = atoi(data);
            } else if (strncmp(field, "hcl", 3) == 0) {
                strcpy(passports[i].hair, data);
            } else if (strncmp(field, "ecl", 3) == 0) {
                strcpy(passports[i].eyes, data);
            } else if (strncmp(field, "pid", 3) == 0) {
                passports[i].pid = atoi(data);
            } else if (strncmp(field, "cid", 3) == 0) {
                passports[i].cid = atoi(data);
            }

            p = strtok(NULL, ":");
        }
	}

	fclose(inFile);

    for (int i = 0; i < numLines; i++) {
        if (valid_p1(passports, i)) {
            count++;
        }
    }

    return count;
}

int part2(int numLines, char *fileName) {
    int count = 0;

    struct passport_p2 *passports;

    passports = (Passport_p2 *)calloc(numLines, sizeof(Passport_p2));
    for (int i = 0; i < numLines; i++) {
        passports[i].birth = -1;
        passports[i].cid = -1;
        passports[i].expiration = -1;
        strcpy(passports[i].eyes, "\n");
        strcpy(passports[i].hair, "\n");
        strcpy(passports[i].height, "\n");
        passports[i].issue = -1;
        strcpy(passports[i].pid, "\n");
    }

    int i = 0;
	char textRead[dataLine], *p;

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

	while(fgets(textRead, dataLine, inFile)) {
        if (textRead[0] == '\n') {
            i++;
        }

        p = strtok(textRead, ":");

        while (p != NULL) {
            char field[5], data[20];
            strcpy(field, p);

            p = strtok(NULL, " ");
            if (p == NULL) {
                break;
            }
            strcpy(data, p);

            if (strncmp(field, "byr", 3) == 0) {
                passports[i].birth = atoi(data);
            } else if (strncmp(field, "iyr", 3) == 0) {
                passports[i].issue = atoi(data);
            } else if (strncmp(field, "eyr", 3) == 0) {
                passports[i].expiration = atoi(data);
            } else if (strncmp(field, "hgt", 3) == 0) {
                strcpy(passports[i].height, data);
            } else if (strncmp(field, "hcl", 3) == 0) {
                strcpy(passports[i].hair, data);
            } else if (strncmp(field, "ecl", 3) == 0) {
                strcpy(passports[i].eyes, data);
            } else if (strncmp(field, "pid", 3) == 0) {
                strcpy(passports[i].pid, data);
            } else if (strncmp(field, "cid", 3) == 0) {
                passports[i].cid = atoi(data);
            }

            p = strtok(NULL, ":");
        }
	}

	fclose(inFile);

    for (int i = 0; i < numLines; i++) {
        if (valid_p2(passports, i)) {
            count++;
        }
    }

    return count;
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

	int numLines, count = 0;
	struct passport *passports;

	numLines = findLines(inputPath);

    if (numLines == -1) {
		printf("Error: could not open file. Quitting\n");
		return -1;
	}

    clock_t t; 
    t = clock(); 
    int p1 = part1(numLines, inputPath);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nValid passports: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    int p2 = part2(numLines, inputPath);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nValid passports: %d\nRan in %f seconds\n", p2, t_p2);

	return 0;
}
