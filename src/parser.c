
#include "parser.h"

#include <stdio.h>
#include <string.h>

config_info_t config_info;

extern char *strdup(char *s);

config_element configfile_info[] = {
    {"remote-host", string_e, "The hostname of the server", NULL},
    {"remote-port", integer_e, "The port of the server", NULL},
    {"user", string_e, "The username to use when connecting to the server", NULL},
    {"password", string_e, "The password to use when connecting to the server", NULL},
    {"timeout", real_e, "The timeout to use when connecting to the server", NULL},
};

config_element *find_config_element(char *name, config_element *element) {
    int i;
    for (i = 0; element[i].name; i++) {
        if (strcmp(name, element[i].name) == 0) {
            return &element[i];
        }
    }
    return (config_element *)NULL;
}

config_info_t *setup_config_info_defaults() {
    config_info.host     = strdup("");
    config_info.port     = 0;
    config_info.username = strdup("");
    config_info.password = strdup("");
    config_info.timeout  = 0.0;
    return &config_info;
}

int readline(FILE *fp, char *buff, const int len) {
    char c = '\0';
    int read_bytes, pos;

    buff[len - 1] = ' ';
    if (!buff) {
        fprintf(stderr, "readline: buff is NULL\n");
        return 0;
    } else if (len <= 0) {
        fprintf(stderr, "readline: invalid length\n");
        return 0;
    }

    pos        = 0;
    read_bytes = fread(&c, sizeof(char), 1, fp);
    if (read_bytes == -1) {
        fprintf(stderr, "readline: read error\n");
        return 0;
    }

    while ((c != '\n') && (pos < len) && (read_bytes == 1)) {
        buff[pos++] = c;
        read_bytes  = fread(&c, sizeof(char), 1, fp);
    }

    if (pos < len) {
        buff[pos] = '\0';
    } else {
        buff[len - 1] = '\0';
        fprintf(stderr, "readline: line too long\n");
        return 0;
    }
    return ((pos > 0) || (c == '\n')) ? 1 : 0;
}

extern void setup_config_info() {
    int x = 0;
    /* DO NOT change the order below */
    configfile_info[x++].setting = &config_info.host;
    configfile_info[x++].setting = &config_info.port;
    configfile_info[x++].setting = &config_info.username;
    configfile_info[x++].setting = &config_info.password;
    configfile_info[x++].setting = &config_info.timeout;
}

extern int parse_config_file(const char *filename) {
    config_element *configs;
    char word[BUFSIZE], line[BUFSIZE];
    FILE *fp;
    int lineno = 0;

    if (!filename) {
        fprintf(stderr, "parse_config_file: filename is NULL\n");
        return 1;
    }

    if (!(fp = fopen(filename, "r"))) {
        fprintf(stderr, "parse_config_file: cannot open file %s\n", filename);
        return 1;
    }

    while (readline(fp, line, BUFSIZE) > 0) {
        lineno++;
        if ((strlen(line) < 2) || (line[0] == '#') || (line[0] == ' ')) continue;
        if (sscanf(line, "%s", word) != 1) continue;
        if (!(configs = find_config_element(word, configfile_info))) {
            fprintf(stderr, "parse_config_file: unknown config element %s on line %d\n", word, lineno);
            return 1;
        }
        switch (configs->type) {
            case integer_e:
                if (sscanf(line, "%s %d", word, (int *)configs->setting) != 2) {
                    fprintf(stderr, "parse_config_file: invalid integer value for %s on line %d\n", word, lineno);
                    return 1;
                }
                printf("\033[34m%s\033[0m = %d\n", word, *(int *)configs->setting);
                break;
            case real_e:
                if (sscanf(line, "%s %lf", word, (double *)configs->setting) != 2) {
                    fprintf(stderr, "parse_config_file: invalid real value for %s on line %d\n", word, lineno);
                    return 1;
                }
                printf("\033[33m%s\033[0m = %f\n", word, *(double *)configs->setting);
                break;
            case string_e:
                if (sscanf(line, "%s %s", word, (char *)configs->setting) != 2) {
                    fprintf(stderr, "parse_config_file: invalid string value for %s on line %d\n", word, lineno);
                    return 1;
                }
                printf("\033[32m%s\033[0m = %s\n", word, (char *)configs->setting);
                break;
            default:
                fprintf(stderr, "parse_config_file: unknown config element type %d on line %d\n", configs->type,
                        lineno);
                return 1;
        }
    }
    fclose(fp);
    return 0;
}