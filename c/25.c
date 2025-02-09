#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define defaultInput "../../Inputs/2020_25.txt"
#define dataLine 10

int *readData(char *fileName) {
	char textRead[dataLine];
    int *nums = (int*)calloc(2, sizeof(int));
    int index = 0;

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return NULL;
    }

	while(fgets(textRead, dataLine, inFile)) {
        if (index == 2) {
            fclose(inFile);
            return NULL;
        }

        nums[index] = atoi(textRead);
        index++;
	}

	fclose(inFile);

	return nums;
}

int findLoopSize(int pk) {
    int count = 0, value = 1, subject = 7;

    while (value != pk) {
        value *= subject;
        value = value % 20201227;
        count++;
    }

    return count;
}

unsigned long long int part1(char *fileName) {
    int *nums = readData(fileName);
    if (!nums) {
        printf("Failed to read file\n");
        return 1;
    }

    int cardLoop = 0, doorLoop = 0, cardPk = nums[0], doorPk = nums[1];

    cardLoop = findLoopSize(cardPk);
    doorLoop = findLoopSize(doorPk);

    int loop = (cardLoop < doorLoop) ? cardLoop : doorLoop, subject = (cardLoop < doorLoop) ? doorPk : cardPk;

    unsigned long long int sk = 1;
    for (int i = 0; i < loop; i++) {
        sk *= subject;
        sk = sk % 20201227;
    }

    return sk;
}

char *part2() {
    return "Christmas has been saved!";
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
    printf("\nPart 1:\nThe encryption key is: %llu\nRan in %f seconds\n", p1, t_p1);

    t = clock(); 
    char *p2 = part2();
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\n%s\nRan in %f seconds\n", p2, t_p2);

    return 0;
}