#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define KEY_SIZE 32  // 32 bytes for a 256-bit key

void generate_and_save_key(const char *filename) {
    unsigned char key[KEY_SIZE];
    int i;

    srand((unsigned int)time(NULL));
    
    for (i = 0; i < KEY_SIZE; i++) {
        key[i] = (unsigned char)(rand() % 256);
    }

    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Failed to open file to write key");
        exit(EXIT_FAILURE);
    }

    if (fwrite(key, 1, KEY_SIZE, file) != KEY_SIZE) {
        fprintf(stderr, "Failed to write key to file\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    printf("Key successfully written to %s\n", filename);
}

int main() {
    generate_and_save_key("key.bin");
    return 0;
}
