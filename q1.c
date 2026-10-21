
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 3
#define SIZE 100

struct Polynomial {
    int coeff[MAX + 1];
    int degree;
};

/* Read polynomial string */
int parsePolynomial(char s[], struct Polynomial *p)
{
    char *q = s;

    for (int i = 0; i <= MAX; i++)
        p->coeff[i] = 0;

    while (*q) {
        while (isspace((unsigned char)*q)) q++;
        if (*q == '\0' || *q == '\n') break;

        int sign = 1, c = 0, e = 0, hasC = 0;

        if (*q == '-') { sign = -1; q++; }
        else if (*q == '+') q++;

        while (isdigit((unsigned char)*q)) {
            c = c * 10 + (*q - '0');
            hasC = 1;
            q++;
        }

        if (*q == 'x' || *q == 'X') {
            q++;
            if (!hasC) c = 1;
            e = 1;

            if (*q == '^') {
                q++;
                if (!isdigit((unsigned char)*q)) return 0;
                e = 0;
                while (isdigit((unsigned char)*q)) {
                    e = e * 10 + (*q - '0');
                    q++;
                }
            }
        } else if (!hasC) {
            return 0;
        }

        if (e > MAX) return 0;
        p->coeff[e] += sign * c;

        while (isspace((unsigned char)*q)) q++;
        if (*q && *q != '\n' && *q != '+' && *q != '-')
            return 0;
    }

    p->degree = MAX;
    while (p->degree > 0 && p->coeff[p->degree] == 0)
        p->degree--;

    return 1;
}

/* Display polynomial */
void printPolynomial(struct Polynomial p)
{
    int first = 1;

    for (int i = p.degree; i >= 0; i--) {
        int c = p.coeff[i];
        if (c == 0) continue;

        if (!first)
            printf(c > 0 ? " + " : " - ");
        else if (c < 0)
            printf("-");

        int value = abs(c);

        if (i == 0 || value != 1) printf("%d", value);
        if (i > 0) {
            printf("x");
            if (i > 1) printf("^%d", i);
        }

        first = 0;
    }

    if (first) printf("0");
    printf("\n");
}

/* Addition */
struct Polynomial addPolynomial(struct Polynomial a,
                                struct Polynomial b)
{
    struct Polynomial r = {0};

    for (int i = 0; i <= MAX; i++)
        r.coeff[i] = a.coeff[i] + b.coeff[i];

    r.degree = MAX;
    while (r.degree > 0 && r.coeff[r.degree] == 0)
        r.degree--;

    return r;
}

/* Subtraction */
struct Polynomial subtractPolynomial(struct Polynomial a,
                                     struct Polynomial b)
{
    struct Polynomial r = {0};

    for (int i = 0; i <= MAX; i++)
        r.coeff[i] = a.coeff[i] - b.coeff[i];

    r.degree = MAX;
    while (r.degree > 0 && r.coeff[r.degree] == 0)
        r.degree--;

    return r;
}

/* Multiplication */
void multiplyPolynomial(struct Polynomial a, struct Polynomial b)
{
    int r[2 * MAX + 1] = {0};

    for (int i = 0; i <= a.degree; i++)
        for (int j = 0; j <= b.degree; j++)
            r[i + j] += a.coeff[i] * b.coeff[j];

    printf("Result: ");

    int first = 1;
    for (int i = 2 * MAX; i >= 0; i--) {
        int c = r[i];
        if (c == 0) continue;

        if (!first)
            printf(c > 0 ? " + " : " - ");
        else if (c < 0)
            printf("-");

        int value = abs(c);

        if (i == 0 || value != 1) printf("%d", value);
        if (i > 0) {
            printf("x");
            if (i > 1) printf("^%d", i);
        }

        first = 0;
    }

    if (first) printf("0");
    printf("\n");
}

/* Division: displays quotient and remainder coefficients */
void dividePolynomial(struct Polynomial a, struct Polynomial b)
{
    double rem[2 * MAX + 1] = {0};
    double quo[2 * MAX + 1] = {0};

    if (b.degree == 0 && b.coeff[0] == 0) {
        printf("Cannot divide by zero polynomial.\n");
        return;
    }

    for (int i = 0; i <= a.degree; i++)
        rem[i] = a.coeff[i];

    for (int i = a.degree; i >= b.degree; i--) {
        quo[i - b.degree] = rem[i] / b.coeff[b.degree];

        for (int j = 0; j <= b.degree; j++)
            rem[i - b.degree + j] -=
                quo[i - b.degree] * b.coeff[j];
    }

    printf("Quotient coefficients (low power first): ");
    for (int i = 0; i <= MAX; i++)
        printf("%.2f ", quo[i]);

    printf("\nRemainder coefficients (low power first): ");
    for (int i = 0; i <= MAX; i++)
        printf("%.2f ", rem[i]);

    printf("\n");
}

/* Find integer roots */
void findFactors(struct Polynomial p)
{
    int found = 0;

    for (int x = -100; x <= 100; x++) {
        long long value = 0;

        for (int i = p.degree; i >= 0; i--)
            value = value * x + p.coeff[i];

        if (value == 0) {
            printf("Root: %d, factor: (x %c %d)\n",
                   x, x >= 0 ? '-' : '+', abs(x));
            found = 1;
        }
    }

    if (!found)
        printf("No integer roots found.\n");
}

/* Main menu */
int main(void)
{
    int choice;
    char s[SIZE];
    struct Polynomial a, b, result;

    while (1) {
        printf("\n1. Addition\n");
        printf("2. Subtraction\n");
        printf("3. Division\n");
        printf("4. Multiplication\n");
        printf("5. Finding factors\n");
        printf("0. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
            break;

        getchar();

        if (choice == 0) break;

        if (choice < 1 || choice > 5) {
            printf("Invalid choice!\n");
            continue;
        }

        printf("Enter first polynomial: ");
        if (!fgets(s, SIZE, stdin) || !parsePolynomial(s, &a)) {
            printf("Invalid polynomial!\n");
            continue;
        }

        if (choice == 5) {
            findFactors(a);
            continue;
        }

        printf("Enter second polynomial: ");
        if (!fgets(s, SIZE, stdin) || !parsePolynomial(s, &b)) {
            printf("Invalid polynomial!\n");
            continue;
        }

        switch (choice) {
            case 1:
                result = addPolynomial(a, b);
                printf("Result: ");
                printPolynomial(result);
                break;

            case 2:
                result = subtractPolynomial(a, b);
                printf("Result: ");
                printPolynomial(result);
                break;

            case 3:
                dividePolynomial(a, b);
                break;

            case 4:
                multiplyPolynomial(a, b);
                break;
        }
    }

    return 0;
}
