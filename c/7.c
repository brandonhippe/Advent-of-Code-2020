#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#define defaultInput "../../Inputs/2020_7.txt"
#define dataLine 500
#define nameLen 100


struct bag *createBag(struct bag *b, char bagName[25], int arrLen, int *arr, char *amounts);
void printBag(struct bag *b);
int findLines(char *fileName);
struct bag **readData(int numBags, char *bagNames, char *fileName);
int getBagIndex(char *search, char *bagsList);
char* getBagNames(char *fileName);
void findBagContents(char *inputText, int *arr, int arrIndex);
int getAmount(char *amts, int index);
int numInstances(char *str, char character);
bool canContain(int searchFor, struct bag *thisBag, struct bag **bags);
int numContain(struct bag *thisBag, struct bag **bags);

typedef struct bag {
    char name[nameLen], amts[nameLen];
    int structSize, arrSize, contains[];
} Bag;

struct bag *createBag(struct bag *b, char bagName[25], int arrLen, int *arr, char *amounts) {
    b = malloc(sizeof(*b) + sizeof(int) * arrLen);

    strcpy(b->name, bagName);
    strcpy(b->amts, amounts);
    b->arrSize = arrLen;
    for (int i = 0; i < arrLen; i++) {
        b->contains[i] = *(arr + i);
    }

    b->structSize = (sizeof(*b) + sizeof(int) * arrLen);

    return b;
}

void printBag(struct bag *b) {
    printf("Bag color: %s\nContains bags", b->name);
    for (int i = 0; i < b->arrSize; i++) {
        printf("%s %dx%d", (i == 0 ? ":" : ","), b->contains[i], getAmount(b->amts, i));
    }

    printf("\n");
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
        ++numLines;
	}

	fclose(inFile);

	return numLines;
}

struct bag **readData(int numBags, char *bagNames, char *fileName) {
    struct bag **bags = (struct bag **)calloc(numBags, sizeof(struct bag *));
    int lineNum = 0;
	char textRead[dataLine];

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return NULL;
    }

	while(fgets(textRead, dataLine, inFile)) {
        int numContain = numInstances(textRead, ',') + 1, contains[numContain], index = 0, com = 0;
        contains[0] = -1;
        char *bagName = (char *)calloc(nameLen, sizeof(char));
        char *amts = (char *)calloc(nameLen, sizeof(char));
        char *bagSearch = (char *)calloc(nameLen, sizeof(char));

        char *p = strtok(textRead, " ");

        while (p) {
            if (strncmp(p, "bag", 3) == 0) {
                com++;
            }

            switch (com) {
                case 0:
                    strcat(bagName, p);
                    strcat(bagName, " ");
                    break;
                case 1:
                    bagName[strlen(bagName) - 1] = '\0';
                    com++;
                    break;
                case 2:
                    if (isdigit(p[0]) != 0) {
                        strcat(amts, p);
                        strcat(amts,",");
                        com++;
                    }
                    break;
                case 3:
                    strcat(bagSearch, p);
                    strcat(bagSearch, " ");
                    break;
                case 4:
                    bagSearch[strlen(bagSearch) - 1] = '\0';
                    int bag_index = getBagIndex(bagSearch, bagNames);
                    if (bag_index == -1) {
                        printf("Error: Bag {%s} not found in list\n", bagSearch);
                        exit(1);
                    }
                    contains[index] = bag_index;
                    index++;
                    com = 2;
                    free(bagSearch);
                    bagSearch = (char *)calloc(nameLen, sizeof(char));
                    break;
            }

            p = strtok(NULL, " ");
        }

        amts[strlen(amts) - 1] = '\0';

        bags[lineNum] = createBag(bags[lineNum], bagName, numContain, &contains[0], amts);
        lineNum++;
	}

	fclose(inFile);

    return bags;
}

int getBagIndex(char *search, char *bagsList) {
    char text[strlen(search)], bagNames[strlen(bagsList)];

    strcpy(text, search);
    strcpy(bagNames, bagsList);

    int index = 0, prev = 0;
    bool found = false;

    for (int i = 0; i < strlen(bagNames); i++) {
        if (bagNames[i] == ',') {
            int strIndex = 0;
            char *temp;
            temp = (char *)calloc(i - prev, sizeof(char));

            for (int j = prev; j < i; j++) {
                temp[strIndex] = bagNames[j];
                strIndex++;
            }

            if (strncmp(search, temp, i - prev) == 0) {
                found = true;
                break;
            }

            prev = i + 1;
            index++;
        }
    }

    if (!found) {
        index = -1;
    }

    return index;
}

char* getBagNames(char *fileName) {
    int lineNum = 0;
	char textRead[dataLine], *p, *bagStr;

	bagStr = (char *)calloc(nameLen, sizeof(char));

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return NULL;
    }

	while(fgets(textRead, dataLine, inFile)) {
        bagStr = (char *)realloc(bagStr, ((lineNum + 1) * (nameLen * sizeof(char))));
        p = strtok(textRead, " ");

        while (strncmp(p, "bags", 4) != 0) {
            strcat(bagStr, p);
            strcat(bagStr, " ");

            p = strtok(NULL, " ");
        }

        int tempLen = strlen(bagStr);
        bagStr[tempLen - 1] = ',';
        lineNum++;
	}

	fclose(inFile);
    return bagStr;
}

int getAmount(char *amts, int index) {
    char text[strlen(amts)];
    strcpy(text, amts);

    char *p = strtok(text, ",");
    int i = 0;

    while (p) {
        if (i == index) {
            return (atoi(p));
        }

        i++;
        p = strtok(NULL, ",");
    }

    return -1;
}

int numInstances(char *str, char character) {
    int count = 0;

    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == character) {
            count++;
        }
    }

    return count;
}

bool canContain(int searchFor, struct bag *thisBag, struct bag **bags) {
    for (int i = 0; i < thisBag->arrSize; i++) {
        if (thisBag->contains[i] < 0) {
            continue;
        }

        if (thisBag->contains[i] == searchFor) {
            return true;
        } else if (canContain(searchFor, bags[thisBag->contains[i]], bags)) {
            return true;
        }
    }

    return false;
}

int numContain(struct bag *thisBag, struct bag **bags) {
    int count = 0;

    for (int i = 0; i < thisBag->arrSize; i++) {
        int indexAmt = getAmount(thisBag->amts, i);

        if (indexAmt > 0) {
            count += indexAmt * (1 + numContain(bags[thisBag->contains[i]], bags));
        }
    }

    return count;
}

int part1(struct bag **bags, char *bagNames, int numLines) {
    int count = 0;

    for (int i = 0; i < numLines; i++) {
        if (canContain(getBagIndex("shiny gold", bagNames), bags[i], bags)) {
            count++;
        }
    }

    return count;
}

int part2(struct bag **bags, char *bagNames) {
    return numContain(bags[getBagIndex("shiny gold", bagNames)], bags);
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    int numLines = findLines(inputPath);

	if (numLines == -1) {
        printf("Error: Could not read input file. Quitting\n");
        return -1;
	}

	char *bagNames = getBagNames(inputPath);

	struct bag** bags = readData(numLines, bagNames, inputPath);

    clock_t t; 
    t = clock(); 
    int p1 = part1(bags, bagNames, numLines);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nNumber of bags that can eventually contain a shiny gold bag: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    int p2 = part2(bags, bagNames);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nNumber of bags that a shiny gold bag can contain: %d\nRan in %f seconds\n", p2, t_p2);

	return 0;
}