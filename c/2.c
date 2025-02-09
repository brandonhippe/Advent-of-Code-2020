#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#define passwordLength 50
#define defaultInput "../../Inputs/2020_2.txt"
#define dataLine 100

typedef struct passwordInfo {
	char password[passwordLength];
	char character;
	int min;
	int max;
} PasswordInfo;

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

void readData(struct passwordInfo *passwords, int numLines, char *fileName) {
	char textRead[dataLine], *p;
	int lineNum = 0;

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	while(fgets(textRead, dataLine, inFile)) {
		p = strtok(textRead, "-");
		int min = atoi(p);
		passwords[lineNum].min = min;

		p = strtok(NULL, " ");
		int max = atoi(p);
		passwords[lineNum].max = max;

		p = strtok(NULL, ":");
		char character = *p;
		passwords[lineNum].character = character;

		p = strtok(NULL, " ");
		strcpy(passwords[lineNum].password, p);

		++lineNum;
	}

	fclose(inFile);

	return;
}

int charCount_p1(struct passwordInfo *passwords, int index) {
    int count = 0;
    char password[passwordLength];

    strcpy(password, passwords[index].password);
    for (int i = 0; i < passwordLength; i++) {
        char char1 = password[i];
        char char2 = passwords[index].character;
        if (passwords[index].password[i] == passwords[index].character) {
            count++;
        }
    }

    return count;
}

int charCount_p2(struct passwordInfo *passwords, int index) {
    int count = 0;
    int index1 = passwords[index].min - 1;
    int index2 = passwords[index].max - 1;

    if (passwords[index].password[index1] == passwords[index].character) {
           count++;
    }

    if (passwords[index].password[index2] == passwords[index].character) {
           count++;
    }

    return count;
}

int part1(PasswordInfo *passwords, int numLines) {
    int countValid = 0;

    for (int i = 0; i < numLines; i++) {
        int count = charCount_p1(passwords, i);
        if (count >= passwords[i].min && count <= passwords[i].max) {
            countValid++;
        }
    }

    return countValid;
}

int part2(PasswordInfo *passwords, int numLines) {
    int countValid = 0;

    for (int i = 0; i < numLines; i++) {
        int count = charCount_p2(passwords, i);
        if (count == 1) {
            countValid++;
        }
    }

    return countValid;
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

	int numLines;
	struct passwordInfo *passwords;

	numLines = findLines(inputPath);

	if (numLines == -1) {
		printf("Error: could not open file. Quitting\n");
		return -1;
	}

	passwords = (PasswordInfo *)calloc(numLines, sizeof(PasswordInfo));

	readData(passwords, numLines, inputPath);

    clock_t t; 
    t = clock(); 
    int p1 = part1(passwords, numLines);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nValid Passwords: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    int p2 = part2(passwords, numLines);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nValid Passwords: %d\nRan in %f seconds\n", p2, t_p2);

	return 0;
}
