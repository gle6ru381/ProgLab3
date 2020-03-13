#include "coder.h"

int encode(uint32_t value, CodeUnits* codeUnits)
{
    if (value < 128) {
        codeUnits->length = 1;
        uint8_t* byte = &value;
        codeUnits->code[0] = *byte;
    } else if (value < 2048) {
        codeUnits->length = 2;
        uint8_t byte1 = value % 64;
        uint8_t byte2 = (value - byte1) / 64;
        codeUnits->code[0] = byte2 | 0xC0;
        codeUnits->code[1] = byte1 | 0x80;
    } else if (value < 65536) {
        codeUnits->length = 3;
        uint8_t byte1 = value % 64;
        uint8_t byte2 = ((value - byte1) / 64) % 64;
        uint8_t byte3 = (value - byte1 - (byte2 * 64)) / (64 * 64);
        codeUnits->code[0] = byte3 | 0xE0;
        codeUnits->code[1] = byte2 | 0x80;
        codeUnits->code[2] = byte1 | 0x80;
    } else if (value < 1114112) {
        codeUnits->length = 4;
        uint8_t byte1 = value % 64;
        uint8_t byte2 = ((value - byte1) / 64) % 64;
        uint8_t byte3 = ((value - byte1 - (byte2 * 64)) / (64 * 64)) % 64;
        uint8_t byte4 = (value - byte1 - (byte2 * 64) - (byte3 * 64 * 64))
                / (64 * 64 * 64);
        codeUnits->code[0] = byte4 | 0xF0;
        codeUnits->code[1] = byte3 | 0x80;
        codeUnits->code[2] = byte2 | 0x80;
        codeUnits->code[3] = byte1 | 0x80;
    }
    return 0;
}

uint32_t decode(const CodeUnits* codeUnits)
{
    uint32_t result = 0;
    if (codeUnits->length == 1) {
        result = codeUnits->code[0];
    } else if (codeUnits->length == 2) {
        result = codeUnits->code[1] & 0x7f;
        result += (codeUnits->code[0] & 0x3f) * 64;
    } else if (codeUnits->length == 3) {
        result = (codeUnits->code[2] & 0x7f);
        result += (codeUnits->code[1] & 0x7f) * 64;
        result += (codeUnits->code[0] & 0x0f) * 64 * 64;
    } else if (codeUnits->length == 4) {
        result = (codeUnits->code[3] & 0x7f);
        result += (codeUnits->code[2] & 0x7f) * 64;
        result += (codeUnits->code[1] & 0x7f) * 64 * 64;
        result += (codeUnits->code[0] & 0x07) * 64 * 64 * 64;
    }
    return result;
}

int read_next_code_unit(FILE* fin, CodeUnits* codeUnits)
{
    uint8_t byte;
    fread(&byte, sizeof(uint8_t), 1, fin);
    if ((byte & 0xF0) == 0xF0) {
        codeUnits->length = 4;
    } else if ((byte & 0xE0) == 0xE0) {
        codeUnits->length = 3;
    } else if ((byte & 0xC0) == 0xC0) {
        codeUnits->length = 2;
    } else if ((byte | 0x80) != byte) {
        codeUnits->length = 1;
    } else {
        codeUnits->length = 0;
        return 0;
    }

    codeUnits->code[0] = byte;
    if (codeUnits->length != 1) {
        if (!fread(codeUnits->code + 1,
                   sizeof(uint8_t),
                   codeUnits->length - 1,
                   fin))
            return -1;
        for (int i = 1; i < codeUnits->length; i++) {
            if (((codeUnits->code[i] | 0x40) == codeUnits->code[i])
                || (codeUnits->code[i] | 0x80) != codeUnits->code[i]) {
                codeUnits->length = 0;
                return 0;
            }
        }
    }
    return 0;
}
