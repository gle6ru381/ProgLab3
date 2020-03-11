#include "coder.h"
#include "command.h"

void print_error()
{
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "coder encode <in-file-name> <out-file-name>\n");
    fprintf(stderr, "coder decode <in-file-name> <out-file-name>\n");
}

int main(int argc, char** argv)
{
    if (argc < 4) {
        print_error();
        return -1;
    }
    if (strcmp(argv[1], "encode")) {
        if (encode_file(argv[2], argv[3])) {
            print_error();
            return -1;
        }
    } else if (strcmp(argv[1], "decode")) {
        if (decode_file(argv[2], argv[3])) {
            print_error();
            return -1;
        }
    } else {
        print_error();
        return -1;
    }
    return 0;
}