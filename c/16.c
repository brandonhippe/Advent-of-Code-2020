#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define defaultInput "../../Inputs/2020_16.txt"
#define dataLine 150
#define nameLen 25

typedef struct field {
    char name[nameLen];
    int min[2], max[2], structSize;
    bool posFieldNums[];
} Field;

struct field *createField(struct field *f, int numFields) {
    f = malloc(sizeof(*f) + sizeof(bool) * numFields);
    f->structSize = sizeof(*f) + sizeof(bool) * numFields;

    for (int i = 0; i < numFields; i++) {
        f->posFieldNums[i] = true;
    }

    return f;
}

void printField(struct field *f, bool printPos, int numFields) {
    printf("Field named %s: %d-%d or %d-%d\n", f->name, f->min[0], f->max[0], f->min[1], f->max[1]);
    if (printPos) {
        for (int i = 0; i < numFields; i++) {
            if (f->posFieldNums[i]) {
                printf("Field named %s is valid in position #%d\n", f->name, i+1);
            }
        }
        printf("\n\n");
    }
    return;
}

int findAmts(int *arr, char *fileName) {
    int numLines = 0, arrIndex = 0;
	char textRead[dataLine];

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

	while(fgets(textRead, dataLine, inFile)) {
        if (textRead[0] == '\n') {
            if (arrIndex >  0) {
                numLines--;
            }
            arr[arrIndex] = numLines;
            arrIndex++;
            numLines = 0;
        } else {
            ++numLines;
        }
	}

    arr[arrIndex] = numLines -1;
	fclose(inFile);

	return 1;
}

void updatePos(struct field **fieldPtrs, int *vals, int numFields) {
    for (int i = 0; i < numFields; i++) {
        struct field *f = fieldPtrs[i];
        for (int j = 0; j < numFields; j++) {
            if (!((vals[j] >= f->min[0] && vals[j] <= f->max[0]) || (vals[j] >= f->min[1] && vals[j] <= f->max[1]))) {
                f->posFieldNums[j] = false;
            }
        }
    }
    return;
}

void eliminateOptions(struct field **fieldPtrs, int numFields) {
    bool eliminated;
    do {
        for (int i = 0; i < numFields; i++) {
            int pos = -1;
            struct field *f = fieldPtrs[i];
            for (int j = 0; j < numFields; j++) {
                if (f->posFieldNums[j]) {
                    if (pos >= 0) {
                        pos = -1;
                        break;
                    } else {
                        pos = j;
                    }
                }
            }

            if (pos >= 0) {
                for (int j = 0; j < numFields; j++) {
                    if (j == i) {
                        continue;
                    }
                    struct field *rem = fieldPtrs[j];
                    rem->posFieldNums[pos] = false;
                }
            }
        }

        eliminated = true;

        for (int i = 0; i < numFields; i++) {
            int count = 0;
            struct field *f = fieldPtrs[i];
            for (int j = 0; j < numFields; j++) {
                count += (f->posFieldNums[j]) ? 1 : 0;
            }

            if (count > 1) {
                eliminated = false;
                break;
            }
        }

    } while (!eliminated);

    return;
}

int readData(int *amts, bool *tickets, struct field **fieldPtrs, int *myTicket, char *fileName) {
    int count = 0, arrIndex = 0, index = 0, lineNum = 0;
	char textRead[dataLine], *p;
	struct field *fields;

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

	while(fgets(textRead, dataLine, inFile)) {
        if (textRead[0] == '\n') {
            arrIndex++;
        } else if (arrIndex == 0) {
            fields = fieldPtrs[index];
            p = strtok(textRead, ":");
            strcpy(fields->name, p);

            p = strtok(NULL, "-");
            fields->min[0] = atoi(p);
            p = strtok(NULL, " ");
            fields->max[0] = atoi(p);
            p = strtok(NULL, " ");

            p = strtok(NULL, "-");
            fields->min[1] = atoi(p);
            p = strtok(NULL, " ");
            fields->max[1] = atoi(p);

            index++;
        } else if (arrIndex == 2) {
            if (isdigit(textRead[0]) == 0) {
                continue;
            }

            p = strtok(textRead, ",");

            bool valid = true;

            int vals[amts[0]], j = 0;
            while (p) {
                vals[j] = atoi(p);

                bool found = false;
                for (int i = 0; i < amts[0]; i++) {
                    fields = fieldPtrs[i];
                    if ((vals[j] >= fields->min[0] && vals[j] <= fields->max[0]) || (vals[j] >= fields->min[1] && vals[j] <= fields->max[1])) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    valid = false;
                    count += vals[j];
                }

                p = strtok(NULL, ",");
                j++;
            }

            if (valid) {
                int numFields = amts[0];
                updatePos(fieldPtrs, &vals[0], numFields);
            }

            tickets[lineNum] = valid;
            lineNum++;
        } else {
            if (isdigit(textRead[0]) == 0) {
                continue;
            }

            p = strtok(textRead, ",");
            int i = 0;
            while (p) {
                myTicket[i] = atoi(p);
                i++;
                p = strtok(NULL, ",");
            }
        }
	}

	fclose(inFile);

    return count;
}

int part1(int *amts, bool *valid, struct field **fields, int *ticket, char *fileName) {

    for (int i = 0; i < amts[0]; i++) {
        struct field *temp = NULL;
        fields[i] = createField(temp, amts[0]);
    }

    return readData(amts, valid, fields, ticket, fileName);
}

unsigned long long int part2(int *amts, bool *valid, struct field **fields, int *ticket) {
    eliminateOptions(fields, amts[0]);

    unsigned long long int answer = 1;
    for (int i = 0; i < amts[0]; i++) {
        struct field *f = fields[i];
        char temp[nameLen];
        strcpy(temp, f->name);
        if (strncmp(temp, "departure ", 10) == 0) {
            int pos = 0;
            while (!f->posFieldNums[pos]) {
                pos++;
            }

            answer *= ticket[pos];
        }
    }

    return answer;
}


int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    int *amts = (int *)calloc(3, sizeof(int));
    findAmts(amts, inputPath);

    bool *valid = (bool *)calloc(amts[2], sizeof(bool));
    struct field **fields = (struct field **)calloc(amts[0], sizeof(struct field *));
    int *ticket = (int *)calloc(amts[0], sizeof(int));

    clock_t t; 
    t = clock(); 
    int p1 = part1(amts, valid, fields, ticket, inputPath);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nTicket scanning error rate: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    unsigned long long int p2 = part2(amts, valid, &fields[0], ticket);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nProduct of Departure Fields: %llu\nRan in %f seconds\n", p2, t_p2);

    return 0;
}