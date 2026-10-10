#include "hash_table.h"

#include <stdlib.h>
#include <string.h>

/* Create a dynamically allocated copy of a string. */
static char *copy_string(const char *text)
{
    size_t length = strlen(text) + 1;
    char *copy = malloc(length);

    if (copy == NULL)
        return NULL;

    memcpy(copy, text, length);
    return copy;
}

/* Calculate the bucket index for a fingerprint. */
static size_t hash_function(
    uint64_t fingerprint,
    size_t capacity)
{
    return (size_t)(fingerprint % capacity);
}

/* Initialize the hash table. */
int hash_table_init(HashTable *table, size_t capacity)
{
    if (table == NULL || capacity == 0)
        return 0;

    table->buckets = NULL;
    table->capacity = 0;
    table->entry_count = 0;

    table->buckets = calloc(
        capacity, sizeof(FingerprintEntry *)
    );

    if (table->buckets == NULL)
        return 0;

    table->capacity = capacity;
    return 1;
}

/* Add a filename to an existing fingerprint entry. */
static int add_source_file(
    FingerprintEntry *entry,
    const char *filename)
{
    SourceFile *current = entry->files;

    /* Avoid storing the same filename twice. */
    while (current != NULL)
    {
        if (strcmp(current->filename, filename) == 0)
            return 1;

        current = current->next;
    }

    SourceFile *new_file = malloc(sizeof(SourceFile));

    if (new_file == NULL)
        return 0;

    new_file->filename = copy_string(filename);

    if (new_file->filename == NULL)
    {
        free(new_file);
        return 0;
    }

    new_file->next = entry->files;
    entry->files = new_file;

    return 1;
}

/* Insert a fingerprint and its associated source file. */
int hash_table_insert(
    HashTable *table,
    uint64_t fingerprint,
    const char *filename)
{
    if (table == NULL ||
        table->buckets == NULL ||
        table->capacity == 0 ||
        filename == NULL ||
        filename[0] == '\0')
    {
        return 0;
    }

    size_t index = hash_function(
        fingerprint, table->capacity
    );

    FingerprintEntry *current = table->buckets[index];

    /* Search the bucket for this fingerprint. */
    while (current != NULL)
    {
        if (current->fingerprint == fingerprint)
        {
            return add_source_file(current, filename);
        }

        current = current->next;
    }

    /* Fingerprint is new: allocate its entry. */
    FingerprintEntry *new_entry =
        malloc(sizeof(FingerprintEntry));

    if (new_entry == NULL)
        return 0;

    new_entry->fingerprint = fingerprint;
    new_entry->files = NULL;
    new_entry->next = NULL;

    if (!add_source_file(new_entry, filename))
    {
        free(new_entry);
        return 0;
    }

    /* Insert at the beginning of the bucket's list. */
    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
    table->entry_count++;

    return 1;
}

/* Find all files associated with a fingerprint. */
const SourceFile *hash_table_lookup(
    const HashTable *table,
    uint64_t fingerprint)
{
    if (table == NULL ||
        table->buckets == NULL ||
        table->capacity == 0)
    {
        return NULL;
    }

    size_t index = hash_function(
        fingerprint, table->capacity
    );

    FingerprintEntry *current = table->buckets[index];

    while (current != NULL)
    {
        if (current->fingerprint == fingerprint)
            return current->files;

        current = current->next;
    }

    return NULL;
}

/* Free all memory allocated by the hash table. */
void hash_table_destroy(HashTable *table)
{
    if (table == NULL)
        return;

    if (table->buckets != NULL)
    {
        for (size_t i = 0; i < table->capacity; i++)
        {
            FingerprintEntry *entry = table->buckets[i];

            while (entry != NULL)
            {
                FingerprintEntry *next_entry = entry->next;
                SourceFile *file = entry->files;

                while (file != NULL)
                {
                    SourceFile *next_file = file->next;

                    free(file->filename);
                    free(file);

                    file = next_file;
                }

                free(entry);
                entry = next_entry;
            }
        }

        free(table->buckets);
    }

    table->buckets = NULL;
    table->capacity = 0;
    table->entry_count = 0;

}
