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
        encode(value, &codeUnits);
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

        read_next_code_unit(fin, &codeUnits);
        if (codeUnits.code[0] != 0)
            if (!fprintf(fout, "%" PRIx32 "\n", decode(&codeUnits)))
                return -1;
    }
    fclose(fin);
    fclose(fout);
    return 0;
}