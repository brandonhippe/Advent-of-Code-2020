#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define defaultInput "../../Inputs/2020_19.txt"
#define dataLine 200


typedef struct rule {
    int num, len[2], pipe;
    struct rule *subRules[6];
    char text[dataLine];
    bool selfReferential;
} Rule;

void removeChar(char *str, char charToRemove) {
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != charToRemove) dst++;
    }
    *dst = '\0';
}

int countChar(char *str, char search) {
    int i = 0, count = 0;
    while (i < strlen(str)) {
        count += (str[i] == search) ? 1 : 0;
        i++;
    }

    return count;
}

struct rule *createRule(int ruleNum, char *otherRules) {
    struct rule *r = (struct rule *)calloc(1, sizeof(struct rule));
    if (r == NULL) {
        fprintf(stderr, "Memory allocation failed for rule %d\n", ruleNum);
        exit(EXIT_FAILURE);
    }
    r->num = ruleNum;
    r->selfReferential = false;
    int len = strlen(otherRules) - 1;
    removeChar(otherRules, '"');
    strncpy(r->text, otherRules, len);

    for (int i = 0; i < 6; i++) {
        r->subRules[i] = 0;
        r->len[i % 2] = 0;
    }
    r->pipe = 5;

    char *p = strchr(otherRules, '|');
    if (p) {
        p = strtok(otherRules, "|");
        r->pipe = countChar(p, ' ');
    }

    return r;
}

int getLength(struct rule *r) {
    if (r == NULL) {
        fprintf(stderr, "getLength called with NULL rule\n");
        return -1;
    }
    
    if (r->len[0] == 0 && r->len[1] == 0) {
        if (isalpha(r->text[0]) != 0) {
            r->len[0] = 1;
            r->len[1] = 1;
        } else {
            int index = 0;
            struct rule *r1 = r->subRules[index];
            while (r1) {
                if (r1 != r) {
                    int temp = getLength(r1);
                    if (temp == -1) {
                        r->len[0] = -1;
                        r->len[1] = -1;
                    } else if (r->pipe == 5) {
                        r->len[0] += temp;
                        r->len[1] += temp;
                    } else {
                        r->len[index >= r->pipe ? 1 : 0] += temp;
                    }
                }

                index++;
                r1 = r->subRules[index];
            }
        }
    }

    if (r->len[0] != r->len[1] || r->selfReferential) {
        return -1;
    }

    return r->len[0];
}

bool fits_p1(struct rule *r, char *check) {
    if (r == NULL) {
        fprintf(stderr, "fits_p1 called with NULL rule\n");
        return false;
    }
    if (r->len[0] != strlen(check)) {
        return false;
    } else if (isalpha(r->text[0]) != 0) {
        return check[0] == r->text[0];
    } else {
        int i = 0, charIndex = 0;
        struct rule *r1 = r->subRules[i];
        bool reset = false, fit = true;
        while (r1) {
            int length;
            char *temp, *p = check + (charIndex * sizeof(char));

            length = r1->len[i >= r->pipe ? 1 : 0];

            temp = (char*)calloc(length + 1, sizeof(char));
            if (temp == NULL) {
                fprintf(stderr, "Memory allocation failed in fits_p1\n");
                exit(EXIT_FAILURE);
            }
            strncpy(temp, p, length);

            if (!fits_p1(r1, temp)) {
                if (i < r->pipe && r->pipe != 5) {
                    i = r->pipe - 1;
                    fit = false;
                } else {
                    free(temp);
                    return false;
                }
            }

            charIndex += length;
            i++;
            r1 = r->subRules[i];

            if (i >= r->pipe && !reset) {
                if (fit) {
                    free(temp);
                    return true;
                } else {
                    charIndex = 0;
                    reset = true;
                }
            }
            free(temp);
        }
    }

    return true;
}

bool fits_p2(struct rule *r, char *check) {
    int l[2] = {strlen(check), r->len[0]};
    if (l[0] < l[1] || (!r->selfReferential && l[0] != l[1])) {
        return false;
    }

    if (isalpha(r->text[0]) != 0) {
        return check[0] == r->text[0];
    } else {
        int i = 0, charIndex = 0;
        struct rule *r1 = r->subRules[i];
        bool reset = false, fit = true;
        while (r1) {
            int length;
            char *temp, *p = check + (charIndex * sizeof(char));

            if (r1 == r) {
                struct rule *next = r->subRules[i + 1];

                length = strlen(p) - ((next) ? next->len[0] : 0);
            } else if (i == r->pipe - 1) {
                length = strlen(p);
            }  else {
                length = r1->len[i >= r->pipe ? 1 : 0];
            }

            temp = (char*)calloc(length + 1, sizeof(char));
            strncpy(temp, p, length);

            if (!fits_p2(r1, temp)) {
                if (i < r->pipe && r->pipe != 5) {
                    i = r->pipe - 1;
                    fit = false;
                } else {
                    return false;
                }
            }

            charIndex += length;
            i++;
            r1 = r->subRules[i];

            if (i >= r->pipe && !reset) {
                if (fit) {
                    return true;
                } else {
                    charIndex = 0;
                    reset = true;
                }
            }
        }
    }

    return true;
}

bool fitsR0(struct rule *r, char *check) {
    struct rule *s0 = r->subRules[0];
    struct rule *s1 = r->subRules[1];

    if (s0 == NULL || s1 == NULL) {
        return false;
    }

    for (int i = s0->len[0]; i < strlen(check); i += s0->len[0]) {
        if ((strlen(check) - i) % s1->len[0] == 0) {
            char *p0 = (char*)calloc(i + 1, sizeof(char));
            char *p1 = (char*)calloc((strlen(check) - i) + 1, sizeof(char));

            strncpy(p0, check, i);
            strncpy(p1, check + i, strlen(check) - i);

            bool fits = fits_p2(s0, p0) && fits_p2(s1, p1);

            free(p0);
            free(p1);

            if (fits) {
                return true;
            }
        }
    }

    return false;
}

void setSubRules(struct rule *r, int numRules, struct rule **rules, int i, char *ruleText) {
    char *p = strtok(ruleText, " ");
    int index = 0;

    while (p) {
        if (p[0] != '|') {
            int subRule = atoi(p);

            for (int j = 0; j < numRules; j++) {
                struct rule *r1 = rules[j];
                if (r1->num == subRule) {
                    if (i == j) {
                        r->selfReferential = true;
                    }

                    r->subRules[index] = r1;
                    index++;
                    break;
                }
            }
        }

        p = strtok(NULL, " ");
    }
}


int part1(int numRules, struct rule **rules, char *fileName) {
    struct rule *r0;

	for (int i = 0; i < numRules; i++) {
        struct rule *r = rules[i];
        if (r->num == 0) {
            r0 = r;
            break;
        }
	}

	getLength(r0);

	for (int i = 0; i < numRules; i++) {
        struct rule *temp = rules[i];
        if (temp->len[0] == -1) {
            return -1;
        }
	}

	FILE *inFile = fopen(fileName, "r");
    bool inRules = true;
    char *textRead = (char*)calloc(dataLine, sizeof(char));
    int count = 0;

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

	while(fgets(textRead, dataLine, inFile)) {
        if (textRead[0] == '\n') {
            inRules = false;
        } else if (!inRules) {
            textRead[strlen(textRead) - 1] = '\0';
            bool stringFits = fits_p1(r0, textRead);
            count += stringFits ? 1 : 0;
        }

        free(textRead);
        textRead = (char*)calloc(dataLine, sizeof(char));
	}

	fclose(inFile);

	return count;
}

int part2(int numRules, struct rule **rules, char *fileName) {
    struct rule *r0 = NULL;

    for (int i = 0; i < numRules; i++) {
        struct rule *r = rules[i];
        if (r->num == 0) {
            r0 = r;
        } else if (r->num == 8) {
            char rule_str[] = "42 | 42 8";
            char *temp = (char*)calloc(strlen(rule_str) + 1, sizeof(char));
            strcpy(temp, rule_str);
            setSubRules(r, numRules, rules, i, temp);

            char *p = strchr(rule_str, '|');
            if (p) {
                p = strtok(rule_str, "|");
                r->pipe = countChar(p, ' ');
            }
        } else if (r->num == 11) {
            char rule_str[] = "42 31 | 42 11 31";
            char *temp = (char*)calloc(strlen(rule_str) + 1, sizeof(char));
            strcpy(temp, rule_str);
            setSubRules(r, numRules, rules, i, temp);

            char *p = strchr(rule_str, '|');
            if (p) {
                p = strtok(rule_str, "|");
                r->pipe = countChar(p, ' ');
            }
        }
    }

    if (r0 == NULL) {
        fprintf(stderr, "Rule 0 not found\n");
        return -1;
    }

    getLength(r0);

    for (int i = 0; i < numRules; i++) {
        struct rule *temp = rules[i];
        if (temp->len[0] == -1 && temp->num != 0) {
            return -1;
        }
    }

    FILE *inFile = fopen(fileName, "r");
    bool inRules = true;
    char *textRead = (char*)calloc(dataLine, sizeof(char));
    int count = 0;

    // Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

    while(fgets(textRead, dataLine, inFile)) {
        if (textRead[0] == '\n') {
            inRules = false;
        } else if (!inRules) {
            textRead[strlen(textRead) - 1] = 0;
            bool stringFits = fitsR0(r0, textRead);
            count += stringFits ? 1 : 0;
        }

        free(textRead);
        textRead = (char*)calloc(dataLine, sizeof(char));
    }

    fclose(inFile);

    return count;
}

int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    int numRules = 0;
    bool inRules = true;
    char *textRead = (char*)calloc(dataLine, sizeof(char));
    // Open the file
	FILE *inFile = fopen(inputPath, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

	while(fgets(textRead, dataLine, inFile)) {
        if (textRead[0] == '\n') {
            inRules = false;
        }

        if (inRules) {
            numRules++;
        }

        free(textRead);
        textRead = (char*)calloc(dataLine, sizeof(char));
	}

	fclose(inFile);

	struct rule **rules = (struct rule **)calloc(numRules, sizeof(struct rule *));


    inFile = fopen(inputPath, "r");
    inRules = true;
    textRead = (char*)calloc(dataLine, sizeof(char));
    int index = 0;

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

	while(fgets(textRead, dataLine, inFile)) {
        if (textRead[0] == '\n') {
            inRules = false;
        }

        if (inRules) {
            char *contains = strchr(textRead, ' ');
            contains += sizeof(char);
            int ruleNum = round(strtof(textRead, NULL));

            rules[index] = createRule(ruleNum, contains);
            index++;
        }

        free(textRead);
        textRead = (char*)calloc(dataLine, sizeof(char));
	}

	fclose(inFile);

	for (int i = 0; i < numRules; i++) {
        struct rule *r = rules[i];
        char *temp = (char*)calloc(strlen(r->text) + 1, sizeof(char));
        strcpy(temp, r->text);

        setSubRules(r, numRules, rules, i, temp);
	}

    clock_t t;
    t = clock(); 
    int p1 = part1(numRules, rules, inputPath);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nMessages that match rule 0: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    int p2 = part2(numRules, rules, inputPath);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nMessages that match rule 0: %d\nRan in %f seconds\n", p2, t_p2);

    return 0;
}