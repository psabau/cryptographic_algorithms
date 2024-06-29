#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to perform modular exponentiation.
// It returns (base^exp) % mod
long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod; // Make base compatible with mod
    while (exp > 0) {
        if (exp % 2 == 1) // If exp is odd, multiply the base with the result
            result = (result * base) % mod;
        exp = exp >> 1; // Divide exp by 2
        base = (base * base) % mod; // Square the base
    }
    return result;
}

// Function to read keys from a file. It reads two integers from the file.
int read_key(char *filename, int *key1, int *key2) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s.\n", filename);
        return -1; // Return -1 if file cannot be opened
    }
    fscanf(file, "%d %d", key1, key2); // Read the keys
    fclose(file);
    return 0; // Return 0 on success
}

// RSA Encryption function: C = M^e mod n
long long rsa_encrypt(long long M, int e, int n) {
    return mod_pow(M, e, n); // Use modular exponentiation
}

// RSA Decryption function: M = C^d mod n
long long rsa_decrypt(long long C, int d, int n) {
    return mod_pow(C, d, n); // Use modular exponentiation
}

char *read_file(const char *filename) {
    char *buffer = NULL;
    size_t size = 0;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s for reading.\n", filename);
        return NULL;
    }

    // Get the size of the file
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);
    
    buffer = malloc(size + 1);
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        fclose(fp);
        return NULL;
    }

    fread(buffer, size, 1, fp);
    buffer[size] = '\0';
    fclose(fp);
    return buffer;
}


void write_file(const char *filename, long long *data, size_t len, int is_encrypted) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file %s for writing.\n", filename);
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (is_encrypted)
            fprintf(fp, "%lld ", data[i]);
        else
            fprintf(fp, "%c", (char)data[i]);
    }

    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: %s <public_key_file> <private_key_file> <input_file> <output_file>\n", argv[0]);
        return 1; // Return error code if incorrect arguments
    }

    int e, n, d;
    char *message;
    long long *encrypted, *decrypted;
    size_t msg_len;

    // Read the public key
    if (read_key(argv[1], &e, &n) != 0) return 1;

    // Read the private key
    if (read_key(argv[2], &d, &n) != 0) return 1;

    // Read the message from the specified file
    message = read_file(argv[3]);
    if (message == NULL) {
        return 1; // Return error code if message could not be read
    }

    msg_len = strlen(message);
    encrypted = malloc(msg_len * sizeof(long long));
    decrypted = malloc(msg_len * sizeof(long long));
    if (encrypted == NULL || decrypted == NULL) {
        printf("Memory allocation failed.\n");
        free(message);
        return 1; // Return error code if memory allocation fails
    }

    // Encrypt the message character by character
    for (size_t i = 0; i < msg_len; i++) {
        encrypted[i] = rsa_encrypt((long long)message[i], e, n);
    }
    write_file("encrypted.txt", encrypted, msg_len, 1);

    // Decrypt the message
    for (size_t i = 0; i < msg_len; i++) {
        decrypted[i] = rsa_decrypt(encrypted[i], d, n);
    }
    write_file(argv[4], decrypted, msg_len, 0);

    // Clean up and free allocated memory
    free(message);
    free(encrypted);
    free(decrypted);

    return 0;
}
