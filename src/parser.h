#ifndef __PARSER_H
#define __PARSER_H
#define BUFSIZE 1000

typedef enum { integer_e = 1, real_e = 2, string_e = 3, unknown_type_e = -1 } type_t;

typedef struct {
    char* host;
    unsigned int port;
    char* username;
    char* password;
    double timeout;
} config_info_t;

typedef struct {
    char* name;    /* Printable name of this setting */
    type_t type;   /* Integer, string or double? */
    char* doc;     /* What does this setting do? */
    void* setting; /* The actual data affected by this setting */
} config_element;

extern void setup_config_info();
extern int parse_config_file(const char* filename);
#endif /* __PARSER_H */