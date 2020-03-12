#include "command.h"

int encode_file(const char* in_file, const char* out_file)
{
    FILE* fin = fopen(in_file, "r");
    FILE* fout = fopen(out_file, "wb");
    if (fin == NULL || fout == NULL)
        return -1;
    uint32_t value;
    while (!feof(fin)) {
        if (!fscanf(fin, "%" SCNx32, &value))
            return -1;
        CodeUnits codeUnits;
        if (!encode(value, &codeUnits))
            continue;
        if (!fwrite(codeUnits.code, sizeof(uint8_t), codeUnits.length, fout))
            return -1;
    }
    fclose(fin);
    fclose(fout);
    return 0;
}

int decode_file(const char* in_file, const char* out_file)
{
    FILE* fin = fopen(in_file, "rb");
    FILE* fout = fopen(out_file, "w");
    if (fin == NULL || fout == NULL)
        return -1;
    int i = 0;
    while (!feof(fin)) {
        CodeUnits codeUnits;

        if (!read_next_code_unit(fin, &codeUnits))
            return -1;
        if (codeUnits.code[0] != 0 && codeUnits.length != 0)
            if (!fprintf(fout, "%" PRIx32 "\n", decode(&codeUnits)))
                return -1;
    }
    fclose(fin);
    fclose(fout);
    return 0;
}