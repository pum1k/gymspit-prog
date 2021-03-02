#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 20

/* 
 * Typedef: ht_entry / ht_entry_t
 * ------------------------------
 * stores pointers to key and value strings and the next entry 
 * to be used in case of hash collision
 * 
 * struct ht_entry *next
 * - pointer to next entry in the linked list
 * - used in case of collision
 * char *key
 * - pointer to the key of this entry (string)
 * - key is used for inserting, searching and deleting
 * char *value
 * - pointer to the value of this entry (string)
 */
typedef struct ht_entry
{
    struct ht_entry *next;
    char *key;
    char *value;
} ht_entry_t;

/* 
 * Typedef: hashtable / hashtable_t
 * --------------------------------
 * holds data about the hashtable and a list of pointers to the first element
 * of a linked list for each index
 * 
 * int size
 * - length of the pointer array
 * - set by ht_create, should not be changed manually
 * ht_entry_t **table
 * - array of pointers to the first element of the linked lists
 */
typedef struct hashtable
{
    int size;
    ht_entry_t **table;
} hashtable_t;

hashtable_t *ht_create(int size);
void ht_free(hashtable_t *hashtable);
int ht_hash(char str[], int size);
int ht_insert(hashtable_t *hashtable, char key[], char value[]);
char *ht_search(hashtable_t *hashtable, char key[]);
int ht_delete(hashtable_t *hashtable, char key[]);

void print_hashtable(hashtable_t *hashtable);

void strip_end(char str[], char c);

int main(int argc, char const *argv[])
{
    /*
     * Showcase code for the hashtable implementation
     */
    char buffer[64];
    char val_buffer[64];
    char *search_rv;
    int rv;
    hashtable_t *hashtable = ht_create(TABLE_SIZE);
    do
    {
        puts("options: i - insert, s - search, d - delete, p - print, q - quit");
        fgets(buffer, 64, stdin);
        switch (buffer[0])
        {
        case 'q':
            break;
        case 'i':
            printf("key: ");
            fgets(buffer, 64, stdin);
            strip_end(buffer, '\n');
            printf("value: ");
            fgets(val_buffer, 64, stdin);
            strip_end(val_buffer, '\n');
            rv = ht_insert(hashtable, buffer, val_buffer);
            if (rv)
                printf("insertion failed with exit code %d\n", rv);
            break;

        case 's':
            printf("key: ");
            fgets(buffer, 64, stdin);
            strip_end(buffer, '\n');
            search_rv = ht_search(hashtable, buffer);
            if (search_rv != NULL)
            {
                printf("{\"%s\": \"%s\"}\n", buffer, search_rv);
            }
            else
            {
                puts("Key not found!!!");
            }
            break;

        case 'd':
            printf("key: ");
            fgets(buffer, 64, stdin);
            strip_end(buffer, '\n');
            rv = ht_delete(hashtable, buffer);
            if (rv)
                printf("deletion failed with exit code %d\n", rv);
            break;

        case 'p':
            print_hashtable(hashtable);
            break;

        default:
            break;
        }
    } while (buffer[0] != 'q');

    ht_free(hashtable);

    return 0;
}

/* 
 * Function: ht_create
 * -------------------
 * allocate hashtable struct and array of ht_entry struct pointers (heads of 
 * linked lists)
 * 
 * size is the length of the pointer array
 * 
 * input:
 * - int size 
 *   - number indices the newly created table should have
 * 
 * return value:
 * - pointer to hashtable struct 
 * - NULL if there was an error
 */
hashtable_t *ht_create(int size)
{
    hashtable_t *hashtable;

    hashtable = (hashtable_t *)malloc(sizeof(hashtable_t));
    if (hashtable == NULL)
        return NULL;

    hashtable->size = size;

    hashtable->table = (ht_entry_t **)calloc(size, sizeof(ht_entry_t *));
    if (hashtable->table == NULL)
    {
        free(hashtable);
        return NULL;
    }

    return hashtable;
}

/* 
 * Function: ht_free
 * -----------------
 * frees hashtable struct, the array of pointers and all linked list specified
 * by these pointers including its data
 * 
 * input:
 * - hashtable_t *hashtable
 *   - hashtable structure to be freed
 */
void ht_free(hashtable_t *hashtable)
{
    ht_entry_t *current;
    ht_entry_t *next;
    for (int i = 0; i < hashtable->size; i++)
    {
        current = hashtable->table[i];
        while (current != NULL)
        {
            next = current->next;
            free(current->key);
            free(current->value);
            free(current);
            current = next;
        }
    }

    free(hashtable->table);
    free(hashtable);
}

/*
 * Function: ht_hash
 * -----------------
 * calculate FNV-1a hash of a given string, returns hash mod size
 * 
 * input:
 * - char str[]
 *   - string, from which should be the hash calculated
 * - int size
 *   - size of the hashtable we are indexing
 *   - return value calculated as hash % size
 * 
 * return value:
 * - index in the hashtable as int
 */
int ht_hash(char str[], int size)
{
    unsigned int hash = 2166136261;

    for (int i = 0; i < strlen(str); i++)
    {
        hash = (hash ^ str[i]) * 16777619;
    }

    return hash % size;
}

/* 
 * Function: ht_insert
 * -------------------
 * insert a given value to a given key
 * the newly inserted element is added to the start of the linked list
 * if this key already existed, its value will be overwritten
 * 
 * input:
 * - hashtable_t *hashtable 
 *   - hashtable to be used
 * - char key[]
 *   - key, to which you want to write (string)
 * - char value[]
 *   - value to be written to the specified key
 * 
 * return value:
 * - 0 - success
 * - 1 - allocation error
 */
int ht_insert(hashtable_t *hashtable, char key[], char value[])
{
    int index = ht_hash(key, hashtable->size);

    ht_entry_t *entry = hashtable->table[index];
    ht_entry_t *prev = NULL;

    while (entry != NULL)
    {
        if (strcmp(key, entry->key) == 0)
        {
            char *tmp = strdup(value);
            if (tmp == NULL)
                return 1;
            free(entry->value);
            entry->value = tmp;
            return 0;
        }
        else
        {
            prev = entry;
            entry = entry->next;
        }
    }

    entry = (ht_entry_t *)malloc(sizeof(ht_entry_t));
    if (entry == NULL)
        return 1;

    entry->next = NULL;
    entry->key = strdup(key);
    if (entry->key == NULL)
    {
        free(entry);
        return 1;
    }
    entry->value = strdup(value);
    if (entry->value == NULL)
    {
        free(entry->key);
        free(entry);
        return 1;
    }

    entry->next = hashtable->table[index];
    hashtable->table[index] = entry;

    return 0;
}

/* 
 * Function: ht_search
 * -------------------
 * find a given key in the hashtable and return its value
 * 
 * input:
 * - hashtable_t *hashtable 
 *   - hashtable to be used
 * - char key[]
 *   - key to be found (string)
 * 
 * return value:
 * - pointer to the value assigned 
 * - NULL if the key was not found
 */
char *ht_search(hashtable_t *hashtable, char key[])
{
    int index = ht_hash(key, hashtable->size);

    ht_entry_t *entry = hashtable->table[index];

    while (entry != NULL)
    {
        if (strcmp(key, entry->key) == 0)
        {
            return entry->value;
        }
        else
        {
            entry = entry->next;
        }
    }

    return NULL;
}

/* 
 * Function: ht_delete
 * -------------------
 * delete a given key and its value from the hashtable
 * 
 * input:
 * - hashtable_t *hashtable 
 *   - hashtable to be used
 * - char key[]
 *   - key that should be deleted (string)
 * 
 * return value:
 * - 0 - success
 * - 1 - key not found
 */
int ht_delete(hashtable_t *hashtable, char key[])
{
    int index = ht_hash(key, hashtable->size);

    ht_entry_t *entry = hashtable->table[index];
    ht_entry_t *prev = NULL;

    while (entry != NULL)
    {
        if (strcmp(key, entry->key) == 0)
        {
            if (prev != NULL)
            {
                prev->next = entry->next;
            }
            else
            {
                hashtable->table[index] = entry->next;
            }

            free(entry->key);
            free(entry->value);
            free(entry);
            return 0;
        }
        else
        {
            prev = entry;
            entry = entry->next;
        }
    }
    return 1;
}

/* 
 * Function: print_hashtable
 * -------------------------
 * print all entries from the hashtable
 * each entry in in format: ["<key>"] -> "<value>" 
 * where <key> and <value> are replaced with key and value of the entry
 * 
 * input:
 * - hashtable_t *hashtable 
 *   - hashtable to be printed
 */
void print_hashtable(hashtable_t *hashtable)
{
    ht_entry_t *entry;

    for (int i = 0; i < hashtable->size; i++)
    {
        entry = hashtable->table[i];
        printf("%2d |", i);
        while (entry != NULL)
        {
            printf(" -> {\"%s\": \"%s\"}", entry->key, entry->value);
            entry = entry->next;
        }
        printf(" -> NULL\n");
    }
}

/*
 * Function: strip_end
 * -------------------
 * change the prev character of the string to '\0' if it equals
 * the character given, otherwise do nothing
 * 
 * input:
 * - char str[]
 *   - string to be stripped
 * - char c
 *   - character to be stripped
 */
void strip_end(char str[], char c)
{
    int str_len = strlen(str);

    if (str[str_len - 1] == c)
    {
        str[str_len - 1] = '\0';
    }
}
