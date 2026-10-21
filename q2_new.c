
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SUBJECTS 5
#define NAME_SIZE 100

int getInt(void) {
    char input[50];
    char extra;
    int n;

    if (fgets(input, sizeof(input), stdin) == NULL)
        return -1;

    if (sscanf(input, "%d %c", &n, &extra) != 1) {
        printf("Value must be an integer!\n");
        return getInt();
    }

    return n;
}

int isVowel(char c) {
    c = (char)tolower((unsigned char)c);

    return c == 'a' || c == 'e' || c == 'i' ||
           c == 'o' || c == 'u';
}

int getName(char *name) {
    if (fgets(name, NAME_SIZE, stdin) == NULL)
        return -1;

    name[strcspn(name, "\n")] = '\0';

    int flag = 0;

    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha((unsigned char)name[i]) &&
            !isspace((unsigned char)name[i])) {
            printf("Name must contain only letters and spaces.\n");
            printf("Enter name: ");
            return getName(name);
        }

        if (!isspace((unsigned char)name[i]))
            flag = 1;
    }

    if (!flag) {
        printf("Name cannot be empty!\n");
        printf("Enter name: ");
        return getName(name);
    }

    return 1;
}

// Encode Name 
char* encodeName(char *input) {
    char *output = malloc(strlen(input) + 1);

    if (output == NULL)
        return NULL;

    int j = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        char current = (char)tolower((unsigned char)input[i]);

        // Rule 1: Remove consecutive duplicate characters.
        if (j > 0 && output[j - 1] == current)
            continue;

        // Rule 2: Remove the second consecutive vowel.
        if (j > 0 && isVowel(output[j - 1]) &&
            isVowel(current))
            continue;

        output[j++] = current;
    }

    output[j] = '\0';

    return output;
}

// Allocate dynamic 2D matrix 
int** allocateMatrix(int n) {
    int **marks = malloc(n * sizeof(*marks));

    if (marks == NULL)
        return NULL;

    for (int i = 0; i < n; i++) {
        marks[i] = malloc(SUBJECTS * sizeof(*marks[i]));

        if (marks[i] == NULL) {
            for (int j = 0; j < i; j++)
                free(marks[j]);

            free(marks);
            return NULL;
        }
    }

    return marks;
}

void freeMatrix(int **marks, int n) {
    if (marks == NULL)
        return;

    for (int i = 0; i < n; i++)
        free(marks[i]);

    free(marks);
}

// Input marks 
int takeMarks(int *row) {
    for (int i = 0; i < SUBJECTS; i++) {
        printf("Enter Marks %d: ", i + 1);

        int mark = getInt();

        if (mark == -1)
            return -1;

        if (mark < 0 || mark > 100) {
            printf("Marks must lie between 0 and 100.\n");
            i--;
            continue;
        }

        row[i] = mark;
    }

    return 1;
}

int computeTotal(int *row) {
    int total = 0;

    for (int i = 0; i < SUBJECTS; i++)
        total += row[i];

    return total;
}

float computeAvg(int *row) {
    return (float)computeTotal(row) / SUBJECTS;
}

int highestSubject(int *row) {
    int idx = 0;

    for (int i = 1; i < SUBJECTS; i++) {
        if (row[i] > row[idx])
            idx = i;
    }

    return idx;
}


void printResult(char **names, int **marks, int n) {
    for (int i = 0; i < n; i++) {
        printf("%s %d %.2f %d\n",
               names[i],
               computeTotal(marks[i]),
               computeAvg(marks[i]),
               highestSubject(marks[i]));
    }
}

int main() {
    int n;

    printf("Enter number of students: ");
    n = getInt();

    while (n <= 0 || n > 100) {
        if (n == -1)
            return 1;

        printf("Number of students must be between 1 and 100.\n");
        printf("Enter number of students: ");
        n = getInt();
    }

    char **names = malloc(n * sizeof(*names));

    if (names == NULL)
        return 1;

    int **marks = allocateMatrix(n);

    if (marks == NULL) {
        free(names);
        return 1;
    }

    int success = 1;

    for (int i = 0; i < n; i++) {
        char input[NAME_SIZE];

        printf("\nEnter name: ");

        if (getName(input) == -1) {
            success = 0;
            break;
        }

        names[i] = encodeName(input);

        if (names[i] == NULL) {
            success = 0;
            break;
        }

        if (takeMarks(marks[i]) == -1) {
            success = 0;
            break;
        }
    }

    if (success)
        printResult(names, marks, n);
    else
        printf("Error taking input or allocating memory.\n");

    for (int i = 0; i < n; i++)
        free(names[i]);

    free(names);
    freeMatrix(marks, n);

    return success ? 0 : 1;
}
