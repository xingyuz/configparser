#include <stdio.h>

#include "parser.h"

int main(int argc, char const *argv[]) {
    setup_config_info();
    printf("parsing config file%s\n", argv[1]);
    return parse_config_file(argv[1]);
}