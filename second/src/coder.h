#pragma once
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXLENGTH 4

typedef struct {
    size_t length;
    uint8_t code[MAXLENGTH];
} CodeUnits;

int encode(uint32_t, CodeUnits*);
uint32_t decode(const CodeUnits*);
int read_next_code_unit(FILE*, CodeUnits*);