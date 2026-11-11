// Unions
// Extended: Memory analysis using offsetof, alignment, and safe tagged union records.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef union {
    int i;
    float f;
    char str[20];
} UnionData;

typedef struct {
    int i;
    float f;
    char str[20];
} StructData;

typedef enum {
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_TEXT
} ValueType;

typedef struct {
    UnionData data;
    ValueType tag;
} TaggedRecord;

void set_int(TaggedRecord *rec, int value) {
    rec->data.i = value;
    rec->tag = VALUE_INT;
}

void set_float(TaggedRecord *rec, float value) {
    rec->data.f = value;
    rec->tag = VALUE_FLOAT;
}

int set_text(TaggedRecord *rec, const char *text) {
    if (text == NULL || strlen(text) >= 20) {
        return 0;
    }
    strcpy(rec->data.str, text);
    rec->tag = VALUE_TEXT;
    return 1;
}

void display_tagged_record(TaggedRecord *rec) {
    switch (rec->tag) {
        case VALUE_INT: {
            printf("[INT]   Value: %d\n", rec->data.i);
            break;
        }
        case VALUE_FLOAT: {
            printf("[FLOAT] Value: %.2f\n", rec->data.f);
            break;
        }
        case VALUE_TEXT: {
            printf("[TEXT]  Value: \"%s\"\n", rec->data.str);
            break;
        }
        default: {
            printf("[UNKNOWN] Invalid type tag.\n");
            break;
        }
    }
}

int main(void) {

    UnionData udata;

    StructData sdata;
    // sdata = (StructData) {10, 220.5, "Struct Programming"};
    sdata.i = 10;
    strcpy(sdata.str, "Struct Programming");
    sdata.f = 220.5f;

    udata.i = 10;
    printf("udata.i: %d\n", udata.i);

    strcpy(udata.str, "Union Programming");
    printf("udata.str: %s\n", udata.str);

    udata.f = 220.5f;
    printf("udata.f: %.2f\n\n", udata.f);

    // Now see the values after overwriting
    printf("After overwriting:\n");
    printf("udata.i: %d\n", udata.i);
    printf("udata.str: %s\n", udata.str);
    printf("udata.f: %.2f\n\n", udata.f);

    // Memory Addresses - Union
    printf("udata.i: %p\n", (void *)&udata.i);
    printf("udata.str: %p\n", (void *)udata.str);
    printf("udata.f: %p\n\n", (void *)&udata.f);

    // Memory Addresses - Struct
    printf("sdata.i: %p\n", (void *)&sdata.i);
    printf("sdata.str: %p\n", (void *)sdata.str);
    printf("sdata.f: %p\n\n", (void *)&sdata.f);

    printf("Size of the Union - %lu\n", sizeof(UnionData));
    printf("Size of the Structure - %lu\n\n", sizeof(StructData));

    // Extended Task: Field offsets and alignment analysis
    printf("=== Field Offsets in StructData (via offsetof) ===\n");
    printf("Offset of sdata.i   : %zu bytes\n", offsetof(StructData, i));
    printf("Offset of sdata.f   : %zu bytes\n", offsetof(StructData, f));
    printf("Offset of sdata.str : %zu bytes\n\n", offsetof(StructData, str));

    printf("Alignment of UnionData  : %zu bytes\n", _Alignof(UnionData));
    printf("Alignment of StructData : %zu bytes\n\n", _Alignof(StructData));

    printf("Explanation:\n");
    printf("In StructData, each member starts at a different byte offset, and the compiler\n");
    printf("inserts padding bytes to satisfy alignment requirements. In UnionData, all members\n");
    printf("share the exact same starting address (%p) because their memory overlaps.\n\n", (void *)&udata);

    printf("=== Bulk Memory Comparison (100 Records) ===\n");
    printf("Total memory for 100 StructData records : %zu bytes\n", 100 * sizeof(StructData));
    printf("Total memory for 100 UnionData records  : %zu bytes\n", 100 * sizeof(UnionData));
    printf("Memory saved by using UnionData         : %zu bytes\n\n",
           (100 * sizeof(StructData)) - (100 * sizeof(UnionData)));

    // Extended Task: Tagged union records demonstration
    printf("=== Tagged Union Records (5 Mixed Records) ===\n");
    TaggedRecord records[5];

    set_int(&records[0], 10);
    set_float(&records[1], 220.50f);
    set_text(&records[2], "Union Programming");
    set_int(&records[3], 99);
    set_text(&records[4], "Safe Tagged Record");

    for (int i = 0; i < 5; i++) {
        printf("Record %d: ", i);
        display_tagged_record(&records[i]);
    }

    printf("\n=== Single Record Type Transitions ===\n");
    TaggedRecord test_item;
    set_text(&test_item, "First Value");
    printf("After set_text  : ");
    display_tagged_record(&test_item);

    set_int(&test_item, 500);
    printf("After set_int   : ");
    display_tagged_record(&test_item);

    set_float(&test_item, 75.25f);
    printf("After set_float : ");
    display_tagged_record(&test_item);

    printf("\n=== Size Comparison ===\n");
    printf("sizeof(UnionData)    : %zu bytes\n", sizeof(UnionData));
    printf("sizeof(StructData)   : %zu bytes\n", sizeof(StructData));
    printf("sizeof(TaggedRecord) : %zu bytes\n\n", sizeof(TaggedRecord));

    printf("=== Observations ===\n");
    printf("1. A union only allocates enough memory for its largest member, saving memory\n");
    printf("   when only one value type is stored at any given time.\n");
    printf("2. Because union members share the same memory, writing to one member overwrites\n");
    printf("   the contents of the other members. They cannot hold independent values simultaneously.\n");
    printf("3. A tagged union (combining a union with an enum) ensures safe access by tracking\n");
    printf("   which member is currently active.\n\n");

    return 0;
}
