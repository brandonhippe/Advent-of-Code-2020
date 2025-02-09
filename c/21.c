#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define defaultInput "../../Inputs/2020_21.txt"
#define dataLine 600


typedef struct food {
    char ingredients[dataLine], allergens[dataLine];
    int aP[5];
} Food;

typedef struct allergen {
    char name[50];
    struct food **foods;
    struct ingredient *ing;
} Allergen;

typedef struct ingredient {
    char name[50];
    struct food **foods;
    struct allergen **pAllergen;
} Ingredient;

struct food *createFood(char *ing, char *all) {
    struct food *f = (struct food*)calloc(1, sizeof(struct food));
    for (int i = 0; i < 5; i++) {
        f->aP[i] = 0;
    }

    strcpy(f->ingredients, ing);
    strcpy(f->allergens, all);

    return f;
}

void printFood(struct food *f) {
    if (f) {
        printf("Address: %p\nIngredients: %s\nKnown Allergens: %s\n\n\n", f, f->ingredients, f->allergens);
    } else {
        printf("***NULL FOOD***\n\n\n");
    }

    return;
}

struct allergen *createAllergen(char *name) {
    struct allergen *a = (struct allergen*)calloc(1, sizeof(struct allergen));
    a->foods = (struct food**)calloc(50, sizeof(struct food*));

    for (int i = 0; i < 50; i++) {
        a->foods[i] = 0;
    }

    a->ing = 0;

    strcpy(a->name, name);

    return a;
}

void printAllergen(struct allergen *a) {
    if (a) {
        printf("Allergen: %s\nFoods: ", a->name);

        int index = 0;
        while (a->foods[index] != 0) {
            if (index != 0) {
                printf(", ");
            }

            printf("%p", a->foods[index]);
            index++;
        }

        if (a->ing) {
            printf("\nIngredient: %s", a->ing->name);
        }
    } else {
        printf("***NULL ALLERGEN***");
    }

    printf("\n\n\n");

    return;
}

struct ingredient *createIngredient(char *name, struct allergen ** allergenArr) {
    struct ingredient *ing = (struct ingredient*)calloc(1, sizeof(struct ingredient));
    ing->foods = (struct food**)calloc(50, sizeof(struct food*));
    ing->pAllergen = (struct allergen**)calloc(10, sizeof(struct allergen*));

    for (int i = 0; i < 10; i++) {
        ing->pAllergen[i] = allergenArr[i];
    }

    strcpy(ing->name, name);

    return ing;
}

void printIngredient(struct ingredient *ing) {
    if (ing) {
        bool first = true;

        printf("Ingredient: %s\nFoods: ", ing->name);
        for (int index = 0; index < 50; index++) {
            struct food *f = ing->foods[index];
            if (!f) {
                continue;
            }

            if (!first) {
                printf(", ");
            }

            first = false;
            printf("%p", f);
            index++;
        }

        first = true;

        printf("\nPossible Allergens: ");
        for (int index = 0; index < 10; index++){
            struct allergen *a = ing->pAllergen[index];
            if (!a) {
                continue;
            }

            if (!first) {
                printf(", ");
            }

            first = false;
            printf("%s", a->name);
            index++;
        }
    } else {
        printf("***NULL INGREDIENT***");
    }

    printf("\n\n\n");

    return;
}

bool inCSV(char *check, char *csv) {
    char *temp = (char*)calloc(strlen(csv), sizeof(char)), *p = csv;
    while (strlen(p) != 0) {
        while (p[0] != ',' && strlen(p) != 0) {
            strncat(temp, p, 1);
            p++;
        }

        if (strcmp(temp, check) == 0) {
            return true;
        }

        if (strlen(p) != 0) {
            p++;
            temp = (char*)calloc(strlen(csv), sizeof(char));
        }
    }

    return false;
}

int numLines(char *fileName) {
	char textRead[dataLine];
	int size = 0;

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return -1;
    }

	while(fgets(textRead, dataLine, inFile)) {
        size++;
	}

	fclose(inFile);

	return size;
}

void readData(struct food **foods, char *fileName) {
	char textRead[dataLine];
	int size = 0;

	// Open the file
	FILE *inFile = fopen(fileName, "r");

	// Check if the file exists or not
    if (inFile == NULL) {
        return;
    }

	while(fgets(textRead, dataLine, inFile)) {
        char *p = strchr(textRead, '(');
        char *q = strtok(p, " ");
        q = strtok(NULL, " ");
        char *all = (char*)calloc(strlen(p), sizeof(char)), *ing = (char*)calloc(dataLine - strlen(p), sizeof(char));

        while (q) {
            int len = strlen(q);
            if (q[len - 1] == '\n') {
                len -= 2;
            }

            if (q[len - 1] == ')') {
                len--;
            }

            strncat(all, q, len);

            q = strtok(NULL, " ");
        }


        p = strtok(textRead, " ");
        bool started = false;

        while (p[0] != '(') {
            if (started) {
                strcat(ing, ",");
            } else {
                started = true;
            }

            strcat(ing, p);
            p = strtok(NULL, " ");
        }

        foods[size] = createFood(ing, all);
        size++;
	}

	fclose(inFile);

	return;
}

char *findAllergens(struct food **foods, int numFoods) {
    char *allergenNames = (char*)calloc(1, sizeof(char));

    for (int i = 0; i < numFoods; i++) {
        struct food *f = foods[i];
        char *fAll = (char*)calloc(strlen(f->allergens), sizeof(char));
        strcpy(fAll, f->allergens);
        char *temp = strtok(fAll, ",");

        while (temp) {
            if (!inCSV(temp, allergenNames)) {
                int pLen = strlen(allergenNames);
                allergenNames = (char*)realloc(allergenNames, strlen(allergenNames) + strlen(temp) + 2);

                if (pLen != 0) {
                    strcat(allergenNames, ",");
                }

                strcat(allergenNames, temp);
            }

            temp = strtok(NULL, ",");
        }
    }


    return allergenNames;
}

void genAllergens(char *allergenNames, struct allergen **allergens, struct food **foods, int numAllergens, int numFoods) {
    int allIndex = 0;
    char *curr = strtok(allergenNames, ",");

    while (curr) {
        struct allergen *a = createAllergen(curr);
        allergens[allIndex] = a;

        int index = 0;
        for (int i = 0; i < numFoods; i++) {
            struct food *f = foods[i];
            char *fAll = (char*)calloc(strlen(f->allergens), sizeof(char));
            strcpy(fAll, f->allergens);

            if (inCSV(curr, fAll)) {
                a->foods[index] = f;
                index++;
            }
        }

        curr = strtok(NULL, ",");
        allIndex++;
    }

    return;
}

char *findIngredients(struct food **foods, int numFoods) {
    char *ingredientNames = (char*)calloc(1, sizeof(char));

    for (int i = 0; i < numFoods; i++) {
        struct food *f = foods[i];
        char *fIng = (char*)calloc(strlen(f->ingredients), sizeof(char));
        strcpy(fIng, f->ingredients);
        char *temp = strtok(fIng, ",");

        while (temp) {
            if (!inCSV(temp, ingredientNames)) {
                int pLen = strlen(ingredientNames);
                ingredientNames = (char*)realloc(ingredientNames, strlen(ingredientNames) + strlen(temp) + 2);

                if (pLen != 0) {
                    strcat(ingredientNames, ",");
                }

                strcat(ingredientNames, temp);
            }

            temp = strtok(NULL, ",");
        }
    }


    return ingredientNames;
}

void genIngredients(char *ingredientNames, struct ingredient **ingredients, struct food **foods, struct allergen **allergens, int numIngredients, int numFoods, int numAllergens) {
    struct allergen **allergenArr = (struct allergen **)calloc(10, sizeof(struct allergen *));
    for (int i = 0; i < numAllergens; i++) {
        allergenArr[i] = allergens[i];
    }

    int ingIndex = 0;
    char *curr = strtok(ingredientNames, ",");

    while (curr) {
        struct ingredient *ing = createIngredient(curr, allergenArr);
        ingredients[ingIndex] = ing;

        int index = 0;
        for (int i = 0; i < numFoods; i++) {
            struct food *f = foods[i];
            char *fIng = (char*)calloc(strlen(f->ingredients), sizeof(char));
            strcpy(fIng, f->ingredients);

            if (inCSV(curr, fIng)) {
                ing->foods[index] = f;
                index++;
            }
        }

        curr = strtok(NULL, ",");
        ingIndex++;
    }

    return;
}

char *sortAllergens(char *allergenNames, int numAllergens) {
    char *sorted = (char*)calloc(strlen(allergenNames), sizeof(char)), *unsorted = (char*)calloc(strlen(allergenNames), sizeof(char));
    strcpy(unsorted, allergenNames);
    int index = 0;


    while (strlen(sorted) < strlen(allergenNames)) {
        char *record = unsorted, *temp = unsorted;
        while (temp[0] != ',' && strlen(temp) != 0) {
            temp++;
        }

        char *end = temp;

        while (strlen(temp) != 0) {
            temp++;

            char *tempEnd = temp;
            while (tempEnd[0] != ',' && strlen(tempEnd) != 0) {
                tempEnd++;
            }

            char *t1 = (char*)calloc(end - record + 1, sizeof(char)), *t2 = (char*)calloc(tempEnd - temp + 1, sizeof(char));
            strncpy(t1, record, end - record);
            strncpy(t2, temp, tempEnd - temp);

            if (strcmp(t1, t2) > 0) {
                record = temp;
                end = tempEnd;
            }

            while (temp[0] != ',' && strlen(temp) != 0) {
                temp++;
            }
        }

        if (index != 0) {
            strcat(sorted, ",");
        }

        strncat(sorted, record, end - record);

        char *next = (char*)calloc(strlen(unsorted), sizeof(char)), *p = unsorted;

        while (strlen(p) != 0) {
            if (p < record || p > end) {
                strncat(next, p, 1);
            }
            p++;
        }

        int nextLen = strlen(next);
        if (next[nextLen - 1] == ',') {
            nextLen--;
        }

        unsorted = (char*)calloc(nextLen + 1, sizeof(char));

        strncpy(unsorted, next, nextLen);
        index++;
    }

    return sorted;
}

int part1(struct food **foods, struct allergen **allergens, struct ingredient **ingredients, char *sortedAllergens, int numAllergens, int numIngredients) {
    for (int i = 0; i < numIngredients; i++) {
        struct ingredient *ing = ingredients[i];
        char *ingName = (char*)calloc(strlen(ing->name), sizeof(char));
        strcpy(ingName, ing->name);

        for (int j = 0; j < numAllergens; j++) {
            if (ing->pAllergen[j] == NULL) {
                continue;
            }

            struct allergen *a = ing->pAllergen[j];
            char *allName = (char*)calloc(strlen(a->name), sizeof(char));
            strcpy(allName, a->name);

            bool inAll = true;
            for (int index = 0; index < 50; index++) {
                struct food *f = a->foods[index];
                if (f && !inCSV(ingName, f->ingredients)) {
                    inAll = false;
                    break;
                }
            }

            if (!inAll) {
                ing->pAllergen[j] = NULL;
            }
        }
    }

    int count = 0;
    for (int i = 0; i < numIngredients; i++) {
        struct ingredient *ing = ingredients[i];

        bool valid = false;
        for (int j = 0; j < 10; j++) {
            if (ing->pAllergen[j] != 0) {
                valid = true;
                break;
            }
        }

        if (!valid) {
            int index = 0;
            for (int j = 0; j < 50; j++) {
                if (ing->foods[j] == 0) {
                    continue;
                }

                count++;
                index++;
            }
        }
    }

    return count;
}

char *part2(struct allergen **allergens, struct ingredient **ingredients, char *sortedAllergens, int numAllergens, int numIngredients) {
    bool done;
    do {
        done = true;

        for (int i = 0; i < numAllergens; i++) {
            struct allergen *a = allergens[i];

            if (a->ing != 0) {
                continue;
            }

            for (int j = 0; j < numIngredients; j++) {
                struct ingredient *pos = ingredients[j];

                bool found = false;
                for (int index = 0; index < 10; index++){
                    if (pos->pAllergen[index] == a) {
                        found = true;
                        break;
                    }
                }

                if (found) {
                    if (a->ing == 0) {
                        a->ing = pos;
                    } else {
                        a->ing = 0;
                        break;
                    }
                }
            }

            if (a->ing) {
                struct ingredient *pos = a->ing;
                for (int index = 0; index < 10; index++) {
                    if (pos->pAllergen[index] != a) {
                        pos->pAllergen[index] = 0;
                    }
                }

                for (int j = 0; j < numIngredients; j++) {
                    struct ingredient *pos_2 = ingredients[j];

                    if (pos_2 == a->ing) {
                        continue;
                    }

                    int index;
                    for (index = 0; index < 10; index++) {
                        if (pos_2->pAllergen[index] == a) {
                            pos->pAllergen[index] = 0;
                            break;
                        }
                    }
                }
            } else {
                done = false;
            }
        }
    } while (!done);

    char *res = (char*)calloc(1, sizeof(char));
    char *p = sortedAllergens, *start = p;
    int index = 0;
    while (strlen(p) != 0) {
        if (p[0] == ',') {
            char *temp = (char*)calloc(p - start + 1, sizeof(char));

            strncpy(temp, start, p - start);

            for (int i = 0; i < numAllergens; i++) {
                struct allergen *a = allergens[i];

                if (strcmp(a->name, temp) == 0) {
                    struct ingredient *i_ = a->ing;

                    res = (char*)realloc(res, strlen(res) + strlen(i_->name) + 2);
                    if (index != 0) {
                        strcat(res, ",");
                    }

                    strcat(res, i_->name);
                    break;
                }
            }

            start = p + 1;
            index++;
        }

        p++;
    }

    return res;
}


int main (int argc, char *argv[]) {
    char *inputPath = defaultInput;
    if (argc > 1) {
        inputPath = argv[1];
    }

    int numFoods = numLines(inputPath);
    struct food **foods = (struct food **)calloc(numFoods, sizeof(struct food *));
    readData(foods, inputPath);

    char *allergenNames = findAllergens(foods, numFoods);

    int numAllergens = 1;
    for (int i = 0; i < strlen(allergenNames); i++) {
        if (allergenNames[i] == ',') {
            numAllergens++;
        }
    }

    char *sortedAllergens = sortAllergens(allergenNames, numAllergens);
    strcat(sortedAllergens, ",");

    struct allergen **allergens = (struct allergen **)calloc(numAllergens, sizeof(struct allergen *));
    genAllergens(allergenNames, allergens, foods, numAllergens, numFoods);

    char *ingredientNames = findIngredients(foods, numFoods);

    int numIngredients = 1;
    for (int i = 0; i < strlen(ingredientNames); i++) {
        if (ingredientNames[i] == ',') {
            numIngredients++;
        }
    }

    struct ingredient **ingredients = (struct ingredient **)calloc(numIngredients, sizeof(struct ingredient *));
    genIngredients(ingredientNames, ingredients, foods, allergens, numIngredients, numFoods, numAllergens);

    clock_t t;
    t = clock(); 
    int p1 = part1(foods, allergens, ingredients, sortedAllergens, numAllergens, numIngredients);
    t = clock() - t; 
    double t_p1 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 1:\nNumber of instances of ingredients that cannot be allergens: %d\nRan in %f seconds\n", p1, t_p1);

    t = clock();
    char *p2 = part2(allergens, ingredients, sortedAllergens, numAllergens, numIngredients);
    t = clock() - t;
    double t_p2 = ((double)t) / CLOCKS_PER_SEC;
    printf("\nPart 2:\nCanonical dangerous ingredient list: %s\nRan in %f seconds\n", p2, t_p2);

    return 0;
}