#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to calculate the greatest common divisor of two integers
int gcd(int a, int b) {
    int t;
    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Function to check if a number is prime
int is_prime(int num) {
    if (num <= 1) return 0; // return false for numbers less than or equal to 1
    if (num % 2 == 0 && num != 2) return 0; // return false for even numbers except 2
    for (int i = 3; i <= sqrt(num); i += 2) {
        if (num % i == 0) {
            return 0; // return false if num is divisible by any odd number i
        }
    }
    return 1; // return true if num is prime
}

// Function to generate RSA keys
void generate_rsa_keys(int p, int q, int *e, int *d, int *n) {
    *n = p * q; // n is the product of p and q
    int phi = (p - 1) * (q - 1); // Compute the totient function value

    // Select e starting from 2, find an e that is coprime with phi
    *e = 2;
    while (*e < phi) {
        if (gcd(*e, phi) == 1)
            break;
        else
            (*e)++;
    }

    // Calculate d using Extended Euclidean algorithm or simplification for small e
    int k = 1;
    while (1) {
        k = k + phi;
        if (k % *e == 0) {
            *d = k / *e;
            break;
        }
    }
}

int main() {
    int e, d, n, p, q;

    // Loop to get a valid prime number p
    do {
        printf("Enter prime number p: ");
        scanf("%d", &p);
        if (!is_prime(p)) {
            printf("Number p is not a prime number. Please try again.\n");
        }
    } while (!is_prime(p));

    // Loop to get a valid prime number q
    do {
        printf("Enter prime number q: ");
        scanf("%d", &q);
        if (!is_prime(q)) {
            printf("Number q is not a prime number. Please try again.\n");
        }
    } while (!is_prime(q));

    // Generate RSA keys
    generate_rsa_keys(p, q, &e, &d, &n);

    // save the public key
    FILE *fp;
    fp = fopen("public_key.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(fp, "%d %d", e, n);
    fclose(fp);

    // save the private key
    fp = fopen("private_key.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(fp, "%d %d", d, n);
    fclose(fp);

    printf("Keys generated and saved successfully.\n");
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to calculate the greatest common divisor of two integers
int gcd(int a, int b) {
    int t;
    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Function to check if a number is prime
int is_prime(int num) {
    if (num <= 1) return 0; // return false for numbers less than or equal to 1
    if (num % 2 == 0 && num != 2) return 0; // return false for even numbers except 2
    for (int i = 3; i <= sqrt(num); i += 2) {
        if (num % i == 0) {
            return 0; // return false if num is divisible by any odd number i
        }
    }
    return 1; // return true if num is prime
}

// Function to generate RSA keys
void generate_rsa_keys(int p, int q, int *e, int *d, int *n) {
    *n = p * q; // n is the product of p and q
    int phi = (p - 1) * (q - 1); // Compute the totient function value

    // Select e starting from 2, find an e that is coprime with phi
    *e = 2;
    while (*e < phi) {
        if (gcd(*e, phi) == 1)
            break;
        else
            (*e)++;
    }

    // Calculate d using Extended Euclidean algorithm or simplification for small e
    int k = 1;
    while (1) {
        k = k + phi;
        if (k % *e == 0) {
            *d = k / *e;
            break;
        }
    }
}

int main() {
    int e, d, n, p, q;

    // Loop to get a valid prime number p
    do {
        printf("Enter prime number p: ");
        scanf("%d", &p);
        if (!is_prime(p)) {
            printf("Number p is not a prime number. Please try again.\n");
        }
    } while (!is_prime(p));

    // Loop to get a valid prime number q
    do {
        printf("Enter prime number q: ");
        scanf("%d", &q);
        if (!is_prime(q)) {
            printf("Number q is not a prime number. Please try again.\n");
        }
    } while (!is_prime(q));

    // Generate RSA keys
    generate_rsa_keys(p, q, &e, &d, &n);

    // save the public key
    FILE *fp;
    fp = fopen("public_key.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(fp, "%d %d", e, n);
    fclose(fp);

    // save the private key
    fp = fopen("private_key.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(fp, "%d %d", d, n);
    fclose(fp);

    printf("Keys generated and saved successfully.\n");
    return 0;
}
