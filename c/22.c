#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define defaultInput "../../Inputs/2020_22.txt"
#define dataLine 20
#define size 50


typedef struct hand {
    int player, deck[size + 1], cards;
} Hand;

struct hand *createHand(int p) {
    struct hand *h = (struct hand*)calloc(1, sizeof(struct hand));
    h->player = p;

    for (int i = 0; i <= size; i++) {
        h->deck[i] = 0;
    }

    h->cards = 0;

    return h;
}

void printHand(struct hand *h) {
    printf("Player %d's deck: ", h->player);

    for (int index = size; index >= 0 && index + h->cards > size; index--) {
        if (index != size) {
            printf(", ");
        }

        printf("%d", h->deck[index]);

        index--;
    }

    printf("\n");

    return;
}

void readData(struct hand **players, char *fileName) {
    int player = 0, index = size;
	char textRead[dataLine];
	struct hand *h;

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return;
    }

	while(fgets(textRead, dataLine, inFile)) {
        if (textRead[0] == 'P') {
            h = players[player];
            player++;
            index = size;
        } else if (textRead[0] != '\n') {
            int num = atoi(textRead);
            if (num == 0) {
                continue;
            }
            
            h->deck[index] = num;
            h->cards++;
            index--;
        }
	}

	fclose(inFile);

	return;
}

int popStack(int *arr) {
    int popped;
    for (int i = size; i >= 0; i--) {
        if (i == size) {
            popped = arr[i];
        } else {
            arr[i + 1] = arr[i];
            arr[i] = 0;
        }
    }

    return popped;
}

char *decksToStr(struct hand **players) {
    char *str = (char*)calloc(1, sizeof(char));

    for (int j = 0; j < 2; j++) {
        struct hand *h = players[j];
        char *t = (char*)calloc(10, sizeof(char));
        sprintf(t, "%s: ", (j == 0) ? "A" : "B");
        str = realloc(str, (strlen(str) + strlen(t) + 2) * sizeof(char));
        strcat(str, t);

        for (int i = size; i >= 0 && i + h->cards > size; i--) {
            char *temp = (char*)calloc(ceil(log(h->deck[i] + 1)) + 1, sizeof(char));
            sprintf(temp, "%d", h->deck[i]);

            str = realloc(str, (strlen(str) + strlen(temp) + 2) * sizeof(char));

            strcat(str, temp);
            strcat(str, ",");
            free(temp);
        }
    }

    return str;
}


char *inHist(char *pHist, struct hand **players, bool *result) {
    *(result) = false;

    char *curr = decksToStr(players), *tHist = (char*)calloc(strlen(pHist) + 2, sizeof(char)), *p;
    strcpy(tHist, pHist);

    p = strtok(tHist, "A");

    while (p) {
        char *temp = (char*)calloc(strlen(p) + 2, sizeof(char));
        strcat(temp, "A");
        strcat(temp, p);

        if (strncmp(temp, curr, strlen(curr)) == 0) {
            *(result) = true;
            break;
        }

        p = strtok(NULL, "A");
        free(temp);
    }

    char *newHist = (char*)calloc(strlen(pHist) + strlen(curr) + 2, sizeof(char));
    strcpy(newHist, pHist);
    strcat(newHist, curr);

    free(curr), free(tHist), free(pHist);

    return newHist;
}

int newGame(struct hand **players) {
    char *hist = (char*)calloc(1, sizeof(char));

    while (true) {
        int num[2];

        bool found;
        hist = inHist(hist, players, &found);

        if (found) {
            free(hist);
            return 0;
        }

        int *cards = (int*)calloc(2, sizeof(int));
        for (int i = 0; i < 2; i++) {
            struct hand *h = players[i];
            num[i] = popStack(h->deck);
            h->cards--;
            cards[i] = h->cards;
        }

        int winner;
        if (cards[0] >= num[0] && cards[1] >= num[1]) {
            // Recursion
            struct hand **newPlayers = (struct hand **)calloc(2, sizeof(struct hand *));
            for (int i = 0; i < 2; i++) {
                newPlayers[i] = createHand(i + 1);
            }

            for (int i = 0; i < 2; i++) {
                struct hand *oldH = players[i], *newH = newPlayers[i];

                for (int j = size; j > size - num[i]; j--) {
                    newH->deck[j] = oldH->deck[j];
                    newH->cards++;
                }
            }

            winner = newGame(newPlayers);

            free(newPlayers);
        } else {
            // Standard
            winner = (num[0] > num[1]) ? 0 : 1;
        }

        struct hand *h = players[winner];

        int index = size - h->cards;

        h->deck[index] = num[winner];
        h->deck[index - 1] = num[(winner + 1) % 2];
        h->cards += 2;

        for (int i = 0; i < 2; i++) {
            struct hand *h = players[i];
            if (h->cards == 0) {
                int winner = (i + 1) % 2;
                free(hist);
                return winner;
            }
        }
    }
}

int part1(char *fileName) {
    struct hand **players = (struct hand**)calloc(2, sizeof(struct hand*));
    for (int i = 1; i <= 2; i++) {
        players[i - 1] = createHand(i);
    }

    readData(players, fileName);

    bool finished = false;
    while (!finished) {
        int num[2];

        for (int i = 0; i < 2; i++) {
            struct hand *h = players[i];
            num[i] = popStack(h->deck);
            h->cards--;
        }

        int winner = (num[0] > num[1]) ? 0 : 1;

        struct hand *h = players[winner];

        int index = size - h->cards;

        h->deck[index] = num[winner];
        h->deck[index - 1] = num[(winner + 1) % 2];
        h->cards += 2;

        for (int i = 0; i < 2; i++) {
            struct hand *h = players[i];
            if (h->cards == 0) {
                finished = true;
                break;
            }
        }
    }

    int count = 0;
    for (int i = 0; i < 2; i++) {
        struct hand *h = players[i];
        int mult = 1;

        for (int j = size - h->cards + 1; j <= size; j++) {
            count += mult * h->deck[j];
            mult++;
        }
    }

    return count;
}

int part2(char *fileName) {
    struct hand **players = (struct hand**)calloc(2, sizeof(struct hand*));
    for (int i = 1; i <= 2; i++) {
        players[i - 1] = createHand(i);
    }

    readData(players, fileName);
    newGame(players);

    int count = 0;
    for (int i = 0; i < 2; i++) {
        struct hand *h = players[i];
        int mult = 1;

        for (int j = size - h->cards + 1; j <= size; j++) {
            count += mult * h->deck[j];
            mult++;
        }
    }

    return count;
}


int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    clock_t t;
    t = clock(); 
    int p1 = part1(inputPath);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nWinner's Score: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock(); 
    int p2 = part2(inputPath);
    t = clock() - t; 
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nWinner's Score: %d\nRan in %f seconds\n", p2, t_p2);

    return 0;
}