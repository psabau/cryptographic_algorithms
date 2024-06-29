#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NONCE_SIZE 12  // 12 bytes for the nonce, 96 bits

void generate_and_save_nonce(const char *filename) {
    unsigned char nonce[NONCE_SIZE];
    int i;

    // random number generator with current time
    srand((unsigned int)time(NULL));

    // Generate random bytes for the nonce
    for (i = 0; i < NONCE_SIZE; i++) {
        nonce[i] = (unsigned char)(rand() % 256);
    }

    // Open the file to write the nonce
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Failed to open file to write nonce");
        exit(EXIT_FAILURE);
    }

    // Write the nonce to the file
    if (fwrite(nonce, 1, NONCE_SIZE, file) != NONCE_SIZE) {
        fprintf(stderr, "Failed to write nonce to file\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    printf("Nonce successfully written to %s\n", filename);
}

int main() {
    generate_and_save_nonce("nonce.bin");
    return 0;
}
