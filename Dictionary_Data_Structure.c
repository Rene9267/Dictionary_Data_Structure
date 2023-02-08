#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "Sets_Data_Structure.c"

typedef struct dictionary_node dictionary_node_t;

typedef struct dictionary_node
{
    dictionary_node_t *next;
    void *key;
    void *value;
    size_t key_len;
} dictionary_node_t;

struct dictionary_table
{
    dictionary_node_t **nodes;
    size_t hashmap_size;
    int num_collision;
};

void Print_Dictionary(struct dictionary_table *table)
{
    dictionary_node_t **current_node = table->nodes;
    size_t index = 0;
    dictionary_node_t *head_node;
    for (index = 0; index < table->hashmap_size; index++)
    {
        if ((head_node = current_node[index]))
        {
            printf("Index [%llu]:\n", index);
            dictionary_node_t *current_node = head_node;
            while (current_node)
            {
                printf("\tThe Key is [%s], the value is [%s]\n", (char *)current_node->key, (char *)current_node->value);
                current_node = (dictionary_node_t *)current_node->next;
            }
        }
        else
            printf("Index [%llu]:\n", index);
    }
}

int Dictionary_Realloc(struct dictionary_table *table, void *key)
{
    size_t hash = djb33x_hash(key);
    size_t index = hash % table->hashmap_size;

    dictionary_node_t **dictionary_node_temp = calloc(table->hashmap_size, sizeof(dictionary_node_t *));
    memcpy(dictionary_node_temp, table->nodes, table->hashmap_size * sizeof(dictionary_node_t *));
    int hashmap_size_original = table->hashmap_size;
    table->hashmap_size += 3;
    table->nodes = realloc(table->nodes, (sizeof(dictionary_node_t *) * (table->hashmap_size)));
    if (!table->nodes)
    {
        printf("---- !!Fail The realloc of table!! ----\n");
        return -1;
    }
    memset(table->nodes, 0, table->hashmap_size * sizeof(dictionary_node_t *));
    printf("---- !!Node Reallocated!! ----\n");
    
    for (size_t i = 0; i < hashmap_size_original; i++)
    {
        dictionary_node_t *current_node = dictionary_node_temp[i];
        while (current_node)
        {
            printf("%s\n", (char *)current_node->key);
            dictionary_node_t *dictionary_node = (dictionary_node_t *)set_insert((Set_Table_t *)table, current_node->key, strlen(current_node->key));
            if (!dictionary_node)
                return -1;
            dictionary_node->value = current_node->value;
            current_node = current_node->next;
        }
    }

    free(dictionary_node_temp);
    table->num_collision = 0;
    return 0;
}

dictionary_node_t *Dictionary_insert(struct dictionary_table *table, void *key, void *value, const size_t key_len)
{
    dictionary_node_t *dictionary_node = (dictionary_node_t *)set_insert((Set_Table_t *)table, key, key_len);
    if (!dictionary_node)
        return NULL;
    dictionary_node->value = value;

#pragma region Collision Detected
    if (table->num_collision > 0)
    {
        printf("Now i'll Realloc\n");
        Dictionary_Realloc(table, dictionary_node->key);

        Print_Dictionary(table);
        printf("------------------END REALLOC------------------\n");
    }
#pragma endregion

    return dictionary_node;
}

int main(int argc, char const *argv[])
{
    struct dictionary_table *dictionary_table = (struct dictionary_table *)set_table_new(2);

    Dictionary_insert(dictionary_table, "Hello", "Spank", 5);
    Dictionary_insert(dictionary_table, "Hello World", "Die", 11);
    Dictionary_insert(dictionary_table, "This is me", "Poato", 10);
    Dictionary_insert(dictionary_table, "New", "Coffie", 3);
    Dictionary_insert(dictionary_table, "Hello Friend", "Amigo", 11);
    Dictionary_insert(dictionary_table, "Hello Man", "Pino", 10);

    Dictionary_insert(dictionary_table, "Computer", "Science", 8);
    Dictionary_insert(dictionary_table, "Hello Moto", "Rola", 10);
    Dictionary_insert(dictionary_table, "Hello Logi", "tech", 10);
    Dictionary_insert(dictionary_table, "Hello Kitty", "Pink", 11);
    Dictionary_insert(dictionary_table, "Hello Pen", "Table", 9);

    Search((struct set_table *)dictionary_table, "This is me", -1);

    Remove_Sets((Set_Table_t *)dictionary_table, "Hello");
    Remove_Sets((Set_Table_t *)dictionary_table, "Hello Friend");
    Print_Dictionary(dictionary_table);
}
