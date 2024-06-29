#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_random_key(unsigned char *key, int len) {
    srand(time(NULL));
    for (int i = 0; i < len; i++) {
        key[i] = rand() % 256;
    }
}

void write_to_file(unsigned char *data, int len, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    fwrite(data, sizeof(unsigned char), len, file);
    fclose(file);
}

int main() {
    unsigned char key[16];

    generate_random_key(key, 16);

    write_to_file(key, 16, "initial_key.bin");

    printf("Key generation complete.\n");

    return 0;
}