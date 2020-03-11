#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

size_t encode_varint(int32_t value, uint8_t* buf)
{
    assert(buf != NULL);
    uint8_t varint[16];
    uint32_t pos = sizeof(varint) - 1;
    varint[pos] = value & 127;

    while (value >>= 7)
        varint[--pos] = 128 | (--value & 127);
    if (buf)
        memcpy(buf, varint + pos, sizeof(varint) - pos);
    return sizeof(varint) - pos;
}

uint32_t decode_varint(const uint8_t** bufp)
{
    const uint8_t* cur = *bufp;
    uint8_t byte = *cur++;
    uint32_t value = byte & 0x7f;
    while (byte >= 0x80) {
        value += 1;
        byte = *cur++;
        value = (value << 7) + (byte & 127);
    }
    *bufp = cur;
    return value;
}

uint32_t generate_number()
{
    const int r = rand();
    const int p = r % 100;
    if (p < 90)
        return r % 128;
    if (p < 95)
        return r % 16384;
    if (p < 99)
        return r % 268435455;
    return r % 268435455;
}

int main()
{
    srand(time(NULL));

    uint8_t buf[4];
    size_t sizeBuf[20];

    FILE* UnComp = fopen("Uncompressed.dat", "wb");
    FILE* Comp = fopen("Compressed.dat", "wb");
    if (UnComp == NULL || Comp == NULL)
        exit(-3);

    for (int i = 0; i < 1000000; i++) {
        uint32_t value = generate_number();
        fwrite(&value, sizeof(uint32_t), 1, UnComp);
        size_t size = encode_varint(value, buf);
        fwrite(buf, sizeof(uint8_t), size, Comp);
        if (i < 20)
            sizeBuf[i] = size;
    }

    fclose(UnComp);
    fclose(Comp);
    UnComp = fopen("Uncompressed.dat", "rb");
    Comp = fopen("Compressed.dat", "rb");

    for (int i = 0; i < 20; i++) {
        uint32_t valueUncomp = 0;
        uint32_t valueComp = 0;

        fread(&valueUncomp,
              sizeof(uint8_t),
              sizeof(uint32_t) / sizeof(uint8_t),
              UnComp);
        fread(&valueComp, sizeof(uint8_t), sizeBuf[i], Comp);

        uint8_t* val = &valueComp;
        valueComp = decode_varint(&val);
        printf("%" PRIu32 "\t", valueUncomp);
        printf("%" PRIu32 "\n", valueComp);
    }
}