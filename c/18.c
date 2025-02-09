#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define defaultInput "../../Inputs/2020_18.txt"
#define dataLine 200

int countChar(char *str, char search) {
    int i = 0, count = 0;
    while (i < strlen(str)) {
        count += (str[i] == search) ? 1 : 0;
        i++;
    }

    return count;
}

void replaceStr(char *dest, char *src1, char *src2, int start, int end) {
    strncat(dest, src1, start);
    strcat(dest, src2);

    int i = strlen(dest), index = end;

    while (index < strlen(src1)) {
        dest[i] = src1[index];
        i++;
        index++;
    }

    return;
}

unsigned long long int collapse_p1(char *str) {
    char *p = strchr(str, '(');

    while (p) {
        int dist = 0, openCount = 0;

        do {
            if (p[dist] == '(') {
                openCount++;
            } else if (p[dist] == ')') {
                openCount--;
            }

            dist++;
        } while (openCount != 0);

        char *paren = (char*)calloc(dist - 1, sizeof(char));
        strncpy(paren, p + sizeof(char), dist - 2);

        unsigned long long int rep = collapse_p1(paren);
        char *repStr = (char*)calloc((int)(log10(rep) + 1) + 1, sizeof(char));
        sprintf(repStr, "%llu", rep);

        int fIndex = 0;
        while (str[fIndex] != '(') {
            fIndex++;
        }
        char *next = (char*)calloc(strlen(str), sizeof(char));
        replaceStr(next, str, repStr, fIndex, fIndex + dist);
        strcpy(str, next);

        p = strchr(str, '(');
    }

    unsigned long long int count = 0;
    char Op = '+';

    p = strtok(str, " ");
    while (p) {
        if (isdigit(p[0]) == 0) {
            Op = p[0];
        } else {
            if (Op == '+') {
                count += strtoull(p, NULL, 10);
            } else {
                count *= strtoull(p, NULL, 10);
            }
        }

        p = strtok(NULL, " ");
    }

    return count;
}

unsigned long long int collapse_p2(char *str) {
    char *p = strchr(str, '(');

    while (p) {
        int dist = 0, openCount = 0;

        do {
            if (p[dist] == '(') {
                openCount++;
            } else if (p[dist] == ')') {
                openCount--;
            }

            dist++;
        } while (openCount != 0);

        char *paren = (char*)calloc(dist - 1, sizeof(char));
        strncpy(paren, p + sizeof(char), dist - 2);

        unsigned long long int rep = collapse_p2(paren);
        char *repStr = (char*)calloc((int)(log10(rep) + 1) + 1, sizeof(char));
        sprintf(repStr, "%llu", rep);

        int fIndex = 0;
        while (str[fIndex] != '(') {
            fIndex++;
        }
        char *next = (char*)calloc(strlen(str), sizeof(char));
        replaceStr(next, str, repStr, fIndex, fIndex + dist);
        strcpy(str, next);

        p = strchr(str, '(');
    }

    int plusCount = countChar(str, '+'), timesCount = countChar(str, '*');

    while (plusCount != 0 && timesCount != 0) {
        p = strchr(str, '+');

        p -= 2 * sizeof(char);
        int dist = 3;
        while (p[0] != ' ' && p >= str) {
            p -= sizeof(char);
            dist++;
        }
        p += sizeof(char);
        int fIndex = (p - str) / sizeof(char);

        while (str[fIndex + dist] && str[fIndex + dist] != ' ' && str[fIndex + dist] != '\n') {
            dist++;
        }

        char *temp = (char*)calloc(dist + 1, sizeof(char));
        strncpy(temp, p, dist);

        unsigned long long int rep = collapse_p2(temp);
        char *repStr = (char*)calloc((int)(log10(rep) + 1) + 1, sizeof(char));
        sprintf(repStr, "%llu", rep);

        char *next = (char*)calloc(strlen(str), sizeof(char));
        replaceStr(next, str, repStr, fIndex, fIndex + dist);
        strcpy(str, next);


        plusCount = countChar(str, '+');
        timesCount = countChar(str, '*');
    }

    unsigned long long int count = 0;
    char Op = '+';

    p = strtok(str, " ");
    while (p) {
        if (isdigit(p[0]) == 0) {
            Op = p[0];
        } else {
            if (Op == '+') {
                count += strtoull(p, NULL, 10);
            } else {
                count *= strtoull(p, NULL, 10);
            }
        }

        p = strtok(NULL, " ");
    }

    return count;
}

unsigned long long int part1(char *fileName) {
    unsigned long long int count = 0;
    char *textRead = (char*)calloc(dataLine, sizeof(char));

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

    unsigned long long int pCount = count;
    int line = 0;
	while(fgets(textRead, dataLine, inFile)) {
        unsigned long long int temp = collapse_p1(textRead);
        count = count + temp;
        line++;

        free(textRead);
        textRead = (char*)calloc(dataLine, sizeof(char));
        pCount = count;
	}

	fclose(inFile);

    return count;
}

unsigned long long int part2(char *fileName) {
    unsigned long long int count = 0;
    char *textRead = (char*)calloc(dataLine, sizeof(char));

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

    unsigned long long int pCount = count;
    int line = 0;
	while(fgets(textRead, dataLine, inFile)) {
        unsigned long long int temp = collapse_p2(textRead);
        count = count + temp;
        line++;

        free(textRead);
        textRead = (char*)calloc(dataLine, sizeof(char));
        pCount = count;
	}

	fclose(inFile);

	return count;
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    clock_t t; 
    t = clock(); 
    unsigned long long int p1 = part1(inputPath);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nSum: %llu\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    unsigned long long int p2 = part2(inputPath);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nSum: %llu\nRan in %f seconds\n", p2, t_p2);

    return 0;
}