#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdint.h>

/* A source file associated with a fingerprint */
typedef struct SourceFile {
    char *filename;
    struct SourceFile *next;
} SourceFile;

/* One unique fingerprint stored in the table */
typedef struct FingerprintEntry {
    uint64_t fingerprint;
    SourceFile *files;
    struct FingerprintEntry *next;
} FingerprintEntry;

/* The hash table */
typedef struct {
    FingerprintEntry **buckets;
    size_t capacity;
    size_t entry_count;
} HashTable;

/* Initialize the table */
int hash_table_init(HashTable *table, size_t capacity);

/* Add a fingerprint and its source file */
int hash_table_insert(
    HashTable *table,
    uint64_t fingerprint,
    const char *filename
);

/* Find all source files associated with a fingerprint */
const SourceFile *hash_table_lookup(
    const HashTable *table,
    uint64_t fingerprint
);

/* Release all allocated memory */
void hash_table_destroy(HashTable *table);

#endif
