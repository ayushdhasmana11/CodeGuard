#include "hash_table.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    HashTable table;

    assert(hash_table_init(&table, 10) == 1);

    /* Same fingerprint, two different files. */
    assert(hash_table_insert(
        &table, 845721, "student1.cpp") == 1);

    assert(hash_table_insert(
        &table, 845721, "student3.cpp") == 1);

    /* Different fingerprint. */
    assert(hash_table_insert(
        &table, 845722, "student2.cpp") == 1);

    /* Repeating the same pair should not duplicate it. */
    assert(hash_table_insert(
        &table, 845721, "student1.cpp") == 1);

    assert(table.entry_count == 2);

    const SourceFile *file =
        hash_table_lookup(&table, 845721);

    int found_student1 = 0;
    int found_student3 = 0;
    int count = 0;

    while (file != NULL)
    {
        if (strcmp(file->filename, "student1.cpp") == 0)
            found_student1 = 1;

        if (strcmp(file->filename, "student3.cpp") == 0)
            found_student3 = 1;

        count++;
        file = file->next;
    }

    assert(found_student1);
    assert(found_student3);
    assert(count == 2);

    /* Missing fingerprint must return NULL. */
    assert(hash_table_lookup(&table, 999999) == NULL);

    /* Test a collision: 11 % 10 == 1 and 21 % 10 == 1. */
    assert(hash_table_insert(
        &table, 11, "collision1.c") == 1);

    assert(hash_table_insert(
        &table, 21, "collision2.c") == 1);

    assert(hash_table_lookup(&table, 11) != NULL);
    assert(hash_table_lookup(&table, 21) != NULL);

    /* Invalid input tests */
HashTable invalid_table;
assert(hash_table_init(&invalid_table, 0) == 0);
assert(hash_table_insert(NULL, 100, "student1.cpp") == 0);
assert(hash_table_insert(&table, 100, "") == 0);

/* Multiple collisions in the same bucket (capacity = 10) */
assert(hash_table_insert(&table, 31, "student4.cpp") == 1);
assert(hash_table_insert(&table, 41, "student5.cpp") == 1);
assert(hash_table_insert(&table, 51, "student6.cpp") == 1);

assert(hash_table_lookup(&table, 31) != NULL);
assert(hash_table_lookup(&table, 41) != NULL);
assert(hash_table_lookup(&table, 51) != NULL);

    puts("All hash table tests passed!");

    hash_table_destroy(&table);

    assert(table.buckets == NULL);
    assert(table.capacity == 0);

    puts("Memory cleanup test passed!");

    return 0;

}
