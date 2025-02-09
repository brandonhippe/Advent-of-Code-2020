#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define defaultInput "../../Inputs/2020_13.txt"
#define dataLine 200

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

int readData(int *busses, int *bus_times, char *fileName) {
    int lineNum = 0, start, index = 0, bus_time = 0;
	char textRead[dataLine], *p;

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

	while(fgets(textRead, dataLine, inFile)) {
        if (lineNum == 0) {
            start = atoi(textRead);
        } else {
            p = strtok(textRead, ",");

            while (p) {
                if (isdigit(p[0]) != 0) {
                    busses[index] = atoi(p);
                    bus_times[index] = bus_time;
                    index++;
                }

                bus_time++;
                p = strtok(NULL, ",");
            }

        }

        lineNum++;
	}

	fclose(inFile);

    return start;
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

bool all(bool *arr, int len) {
    for (int i = 0; i < len; i++) {
        if (!arr[i]) {
            return false;
        }
    }

    return true;
}

int part1(int start, int numBusses, int *busses) {
    int minWaited = 0;
    while (true) {
        for (int i = 0; i < numBusses; i++) {
            if ((start + minWaited) % busses[i] == 0) {
                return busses[i] * minWaited;
            }
        }

        minWaited++;
    }

    return -1;
}

long long part2(int numBusses, int *busses, int *bus_times) {
    long long timestamp = 0, step = busses[0];
    bool *found = (bool*)calloc(numBusses, sizeof(bool));
    found[0] = true;

    while (!all(found, numBusses)) {
        for (int i = 1; i < numBusses; i++) {
            if ((timestamp + bus_times[i]) % busses[i] == 0 && !found[i]) {
                step *= busses[i];
                found[i] = true;
            }
        }

        timestamp += step;
    }

    return timestamp - step;
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    int numBusses;

	char textRead[dataLine];

	// Open the file
	FILE *inFile = fopen(inputPath, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

	while(fgets(textRead, dataLine, inFile)) {
        numBusses = numInstances(textRead, ',') + 1 - numInstances(textRead, 'x');
	}

	fclose(inFile);

	int busses[numBusses], bus_times[numBusses];
    int start = readData(&busses[0], &bus_times[0], inputPath);

    clock_t t; 
    t = clock(); 
    int p1 = part1(start, numBusses, &busses[0]);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nID * Wait time: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    long long p2 = part2(numBusses, &busses[0], &bus_times[0]);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nTimestamp: %lld\nRan in %f seconds\n", p2, t_p2);

	return 0;
}