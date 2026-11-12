#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum to track the type of data stored
typedef enum {
    TYPE_EMPTY,
    INT,
    FLOAT,
    CHAR,
    STRING
} DataType;

// Union to store different types of data
typedef union {
    int int_value;
    float float_value;
    char char_value;
    char *string_value;
} Data;

// Struct to encapsulate the data and its type
typedef struct {
    Data data;
    DataType type;
} Container;

// Function declarations
void container_init(Container *c);
void container_clear(Container *c);
void free_container(Container *c);
const char *container_type_name(DataType type);
int set_int(Container *c, int value);
int set_float(Container *c, float value);
int set_char(Container *c, char value);
int set_string(Container *c, const char *value);
int get_int(const Container *c, int *out_value);
int get_float(const Container *c, float *out_value);
int get_char(const Container *c, char *out_value);
int get_string(const Container *c, const char **out_value);
int container_copy(Container *dst, const Container *src);
void container_print(const Container *c);

int main(void) {
    Container c;
    container_init(&c);

    int test_int;
    float test_float;
    char test_char;
    const char *test_str;

    set_int(&c, 10);
    get_int(&c, &test_int);
    printf("Int: %d\n", test_int);

    set_float(&c, 3.14f);
    get_float(&c, &test_float);
    printf("Float: %.2f\n", test_float);

    set_char(&c, 'A');
    get_char(&c, &test_char);
    printf("Char: %c\n", test_char);

    set_string(&c, "Hello, World!");
    get_string(&c, &test_str);
    printf("String: %s\n", test_str);

    // Free any allocated memory
    free_container(&c);

    printf("\n=== Heterogeneous Container Array (6 items) ===\n");
    Container items[6];

    for (int i = 0; i < 6; i++) {
        container_init(&items[i]);
    }

    set_int(&items[0], 10);
    set_float(&items[1], 3.14f);
    set_char(&items[2], 'A');
    set_string(&items[3], "Hello, World!");
    set_int(&items[4], 0);
    set_string(&items[5], "Data Structures");

    for (int i = 0; i < 6; i++) {
        printf("Container [%d] | Type: %-6s | Value: ", i, container_type_name(items[i].type));
        container_print(&items[i]);
    }

    printf("\n=== Type-Safe Getter Tests ===\n");
    int int_val;
    if (get_int(&items[0], &int_val) == 0) {
        printf("Item 0 (int)   : %d\n", int_val);
    }

    float float_val;
    if (get_float(&items[1], &float_val) == 0) {
        printf("Item 1 (float) : %.2f\n", float_val);
    }

    char char_val;
    if (get_char(&items[2], &char_val) == 0) {
        printf("Item 2 (char)  : %c\n", char_val);
    }

    const char *str_val;
    if (get_string(&items[3], &str_val) == 0) {
        printf("Item 3 (string): %s\n", str_val);
    }

    if (get_int(&items[1], &int_val) != 0) {
        printf("Item 1 is float; calling get_int returned error code as expected.\n");
    }

    printf("\n=== Sequential Type Transitions on Single Container ===\n");
    Container trans;
    container_init(&trans);

    set_string(&trans, "First String");
    printf("State 1 (STRING): ");
    container_print(&trans);

    set_int(&trans, 999);
    printf("State 2 (INT)   : ");
    container_print(&trans);

    set_string(&trans, "Second String");
    printf("State 3 (STRING): ");
    container_print(&trans);

    set_float(&trans, 123.45f);
    printf("State 4 (FLOAT) : ");
    container_print(&trans);

    free_container(&trans);
    printf("State 5 (CLEAR) : ");
    container_print(&trans);

    printf("\n=== Deep Copy Test ===\n");
    Container src;
    Container dst;
    container_init(&src);
    container_init(&dst);

    set_string(&src, "Original Shared Text");
    container_copy(&dst, &src);

    printf("Source Container: ");
    container_print(&src);
    printf("Target Container: ");
    container_print(&dst);

    set_int(&src, 777);
    printf("Source after changing to INT: ");
    container_print(&src);
    printf("Target after Source changed : ");
    container_print(&dst);

    container_copy(&dst, &dst);
    printf("Target after self-copy      : ");
    container_print(&dst);

    for (int i = 0; i < 6; i++) {
        free_container(&items[i]);
    }
    free_container(&trans);
    free_container(&src);
    free_container(&dst);

    printf("\nAll allocated memory cleaned up successfully.\n");
    return 0;
}

// Function definitions
void container_init(Container *c) {
    if (c == NULL) {
        return;
    }
    c->type = TYPE_EMPTY;
    c->data.string_value = NULL;
}

void container_clear(Container *c) {
    if (c == NULL) {
        return;
    }
    if (c->type == STRING && c->data.string_value != NULL) {
        free(c->data.string_value);
        c->data.string_value = NULL;
    }
    c->type = TYPE_EMPTY;
}

void free_container(Container *c) {
    container_clear(c);
}

const char *container_type_name(DataType type) {
    switch (type) {
        case TYPE_EMPTY: return "EMPTY";
        case INT:        return "INT";
        case FLOAT:      return "FLOAT";
        case CHAR:       return "CHAR";
        case STRING:     return "STRING";
        default:         return "UNKNOWN";
    }
}

int set_int(Container *c, int value) {
    if (c == NULL) {
        return -1;
    }
    container_clear(c);
    c->data.int_value = value;
    c->type = INT;
    return 0;
}

int set_float(Container *c, float value) {
    if (c == NULL) {
        return -1;
    }
    container_clear(c);
    c->data.float_value = value;
    c->type = FLOAT;
    return 0;
}

int set_char(Container *c, char value) {
    if (c == NULL) {
        return -1;
    }
    container_clear(c);
    c->data.char_value = value;
    c->type = CHAR;
    return 0;
}

int set_string(Container *c, const char *value) {
    if (c == NULL || value == NULL) {
        return -1;
    }

    int len = strlen(value);
    char *copy = (char *)malloc(len + 1);
    if (copy == NULL) {
        return -1;
    }
    strcpy(copy, value);

    // Free previous string if any
    container_clear(c);
    c->data.string_value = copy;
    c->type = STRING;
    return 0;
}

int get_int(const Container *c, int *out_value) {
    if (c == NULL || out_value == NULL || c->type != INT) {
        return -1;
    }
    *out_value = c->data.int_value;
    return 0;
}

int get_float(const Container *c, float *out_value) {
    if (c == NULL || out_value == NULL || c->type != FLOAT) {
        return -1;
    }
    *out_value = c->data.float_value;
    return 0;
}

int get_char(const Container *c, char *out_value) {
    if (c == NULL || out_value == NULL || c->type != CHAR) {
        return -1;
    }
    *out_value = c->data.char_value;
    return 0;
}

int get_string(const Container *c, const char **out_value) {
    if (c == NULL || out_value == NULL || c->type != STRING) {
        return -1;
    }
    *out_value = c->data.string_value;
    return 0;
}

int container_copy(Container *dst, const Container *src) {
    if (dst == NULL || src == NULL) {
        return -1;
    }
    if (dst == src) {
        return 0;
    }

    if (src->type == STRING) {
        return set_string(dst, src->data.string_value);
    }

    container_clear(dst);
    dst->type = src->type;
    dst->data = src->data;
    return 0;
}

void container_print(const Container *c) {
    if (c == NULL) {
        printf("(null)\n");
        return;
    }

    switch (c->type) {
        case TYPE_EMPTY: {
            printf("(empty)\n");
            break;
        }
        case INT: {
            printf("%d\n", c->data.int_value);
            break;
        }
        case FLOAT: {
            printf("%.2f\n", c->data.float_value);
            break;
        }
        case CHAR: {
            printf("'%c'\n", c->data.char_value);
            break;
        }
        case STRING: {
            printf("\"%s\"\n", c->data.string_value);
            break;
        }
        default: {
            printf("(unknown)\n");
            break;
        }
    }
}
