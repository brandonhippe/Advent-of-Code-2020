#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#define defaultInput "../../Inputs/2020_6.txt"
#define dataLine 30

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

void groupSize(int *groups, char *fileName) {
    int group = 0, i = 0;
	char textRead[dataLine];

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	while(fgets(textRead, dataLine, inFile)) {
        if (textRead[0] == '\n') {
            groups[i] = group;
            i++;
            group = 0;
        } else {
            group++;
        }
	}
    
	fclose(inFile);
    groups[i] = group;

    return;
}

int charCount(char *str, int ch) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        if (ch == str[i])
            ++count;
    }

    return count;
}

int part1(char *fileName) {
    int count = 0;
	char textRead[dataLine], ans[dataLine];
	for (int i = 0; i < dataLine; i++) {
        ans[i] = (i == 0) ? '\n' : ' ';
    }

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	while(fgets(textRead, dataLine, inFile)) {
        if (textRead[0] == '\n') {
            int i = 0;
            while (ans[i] != '\n') {
                count++;
                i++;
            }

            for (i = 0; i < dataLine; i++) {
                ans[i] = (i == 0) ? '\n' : ' ';
            }

            continue;
        }

        int i = 0;
        while (textRead[i] != '\n') {
            char *p;
            char temp = textRead[i];
            char temps[dataLine];
            strcpy(temps, ans);

            p = strchr(ans, textRead[i]);

            if (p == NULL) {
                p = strchr(ans, '\n');

                *p = temp;
                *(p + 1) = '\n';
            }

            i++;
        }
	}

	fclose(inFile);

    int i = 0;
    while (ans[i] != '\n') {
        count++;
        i++;
    }
    return count;
}

int part2(int *groups, char *fileName) {
    int count = 0, lineNum = 0;
	char textRead[dataLine], *ans;

	// Open the file
	FILE *inFile = fopen(fileName, "r");

    ans = (char *) malloc((groups[0] * dataLine) + 1);
    ans[0] = '\n';

	while(fgets(textRead, dataLine, inFile)) {
        if (textRead[0] == '\n') {
            for (int i = 97; i<= 122; i++) {
                if (charCount(ans, i) == groups[lineNum]) {
                    count++;
                }
            }

            lineNum++;
            free(ans);
            ans = (char *) malloc((groups[lineNum] * dataLine) + 1);
            ans[0] = '\n';
        } else {
            if (ans[0] == '\n') {
                strcpy(ans, textRead);
            } else {
                strcat(ans, textRead);
            }
        }
	}

	fclose(inFile);
    for (int i = 97; i<= 122; i++) {
        if (charCount(ans, i) == groups[lineNum]) {
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

	int numLines, count = 0, max;

	numLines = findLines(inputPath);

	if (numLines == -1) {
        printf("Error: Could not read input file. Quitting\n");
        return -1;
	}

    int groups[numLines];
    groupSize(&groups[0], inputPath);

    clock_t t; 
    t = clock(); 
    int p1 = part1(inputPath);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nNumber of questions answered yes: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    int p2 = part2(&groups[0], inputPath);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nNumber of questions answered yes by everyone in a group: %d\nRan in %f seconds\n", p2, t_p2);

	return 0;
}
