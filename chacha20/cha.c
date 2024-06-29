#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROUNDS 20

// Rotate left: Performs a bitwise left rotation, ensuring bits wrap around.
static uint32_t rotl(uint32_t value, int shift) {
    return (value << shift) | (value >> (32 - shift));
}

// Update the state: This function processes a block of the ChaCha20 algorithm using the given state and produces a block of keystream.
static void chacha20_block(uint32_t state[16], uint8_t keystream[64]) {
    uint32_t x[16];
    int i;

    // Copy the current state to a local array for manipulation.
    memcpy(x, state, sizeof(x));

    // Perform 20 rounds of the ChaCha20 block function, updating the state array.
    for (i = 0; i < ROUNDS; i += 2) {
        // Odd round: Mixing columns and diagonals.
        x[0] += x[4]; x[12] = rotl(x[12] ^ x[0], 16);
        x[8] += x[12]; x[4] = rotl(x[4] ^ x[8], 12);
        x[0] += x[4]; x[12] = rotl(x[12] ^ x[0], 8);
        x[8] += x[12]; x[4] = rotl(x[4] ^ x[8], 7);

        // Even round: Similar operations with different indices.
        x[1] += x[5]; x[13] = rotl(x[13] ^ x[1], 16);
        x[9] += x[13]; x[5] = rotl(x[5] ^ x[9], 12);
        x[1] += x[5]; x[13] = rotl(x[13] ^ x[1], 8);
        x[9] += x[13]; x[5] = rotl(x[5] ^ x[9], 7);
    }

    // Add the modified block back to the original state and output the result in keystream array.
    for (i = 0; i < 16; ++i) {
        uint32_t result = x[i] + state[i];
        keystream[4*i+0] = (result >> 0) & 0xff;
        keystream[4*i+1] = (result >> 8) & 0xff;
        keystream[4*i+2] = (result >> 16) & 0xff;
        keystream[4*i+3] = (result >> 24) & 0xff;
    }
}

// Initializes the ChaCha20 state: Sets up the state array with the constants, key, and nonce.
void chacha20_init(uint32_t state[16], const uint8_t key[32], const uint8_t nonce[12]) {
    static const char *constants = "expand 32-byte k"; // Constants for ChaCha20
    int i;

    // Load constants and key into the state array.
    state[0] = *(const uint32_t *)(constants + 0);
    state[1] = *(const uint32_t *)(constants + 4);
    state[2] = *(const uint32_t *)(constants + 8);
    state[3] = *(const uint32_t *)(constants + 12);
    for (i = 0; i < 8; i++) {
        state[4 + i] = *(const uint32_t *)(key + 4 * i);
    }

    // Set initial values for block counter and nonce.
    state[12] = 0;
    state[13] = 0;
    state[14] = *(const uint32_t *)(nonce + 0);
    state[15] = *(const uint32_t *)(nonce + 4);
}

// Encrypts or decrypts data in place.
void chacha20_encrypt(uint8_t *data, size_t data_len, const uint8_t key[32], const uint8_t nonce[12]) {
    uint32_t state[16];
    uint8_t keystream[64];
    size_t i, j;

    chacha20_init(state, key, nonce);

    for (i = 0; i < data_len; i += 64) {
        chacha20_block(state, keystream);
        for (j = 0; j < 64 && i + j < data_len; ++j) {
            data[i + j] ^= keystream[j];  // XOR data with the generated keystream.
        }

        // Handle counter increment and overflow detection.
        if (state[12] == 0xffffffff) {
            state[12] = 0;
            if (++state[13] == 0) {
                fprintf(stderr, "Nonce overflow\n");
                exit(EXIT_FAILURE);
            }
        } else {
            state[12]++;
        }
    }
}



uint8_t *read_file(const char *filename, size_t *size) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Failed to open file");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);

    uint8_t *data = malloc(*size);
    if (!data) {
        perror("Failed to allocate memory");
        fclose(f);
        return NULL;
    }

    if (fread(data, 1, *size, f) != *size) {
        perror("Failed to read file");
        fclose(f);
        free(data);
        return NULL;
    }

    fclose(f);
    return data;
}

// Function to write a buffer to a file.
void write_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Failed to open file");
        return;
    }

    if (fwrite(data, 1, size, f) != size) {
        perror("Failed to write file");
    }

    fclose(f);
}


int main(int argc, char **argv) {
    int encrypt_mode = -1; // -1 means uninitialized, 1 for encrypt, 0 for decrypt.
    char *key_filename = NULL;
    char *nonce_filename = NULL;
    char *input_filename = NULL;
    char *output_filename = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0) {
            encrypt_mode = 1;
        } else if (strcmp(argv[i], "-d") == 0) {
            encrypt_mode = 0;
        } else if (strcmp(argv[i], "-k") == 0 && i + 1 < argc) {
            key_filename = argv[++i];
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            nonce_filename = argv[++i];
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            input_filename = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_filename = argv[++i];
        } else {
            fprintf(stderr, "Unknown or incomplete argument: %s\n", argv[i]);
            return 1;
        }
    }

    if (encrypt_mode == -1 || !key_filename || !nonce_filename || !input_filename || !output_filename) {
        fprintf(stderr, "Usage: %s -e|-d -k <key file> -n <nonce file> -i <input file> -o <output file>\n", argv[0]);
        return 1;
    }

    size_t key_size, nonce_size, data_size;
    uint8_t *key = read_file(key_filename, &key_size);
    if (!key || key_size != 32) {
        fprintf(stderr, "Invalid key size\n");
        return 1;
    }

    uint8_t *nonce = read_file(nonce_filename, &nonce_size);
    if (!nonce || nonce_size != 12) {
        fprintf(stderr, "Invalid nonce size\n");
        return 1;
    }

    uint8_t *data = read_file(input_filename, &data_size);
    if (!data) {
        return 1;
    }

    chacha20_encrypt(data, data_size, key, nonce);

    write_file(output_filename, data, data_size);

    free(data);
    free(key);
    free(nonce);

    return 0;
}