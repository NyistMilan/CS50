// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
//Average length of words in English times the number of letters available
const unsigned int N = (5 * 26);
int counter = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    //Find hash value
    int hashValue = hash(word);

    //Access the node that has to be traversed
    node *n = table[hashValue];

    //Traversing the linked list
    while (n != NULL)
    {
        //Looking for a match
        if (strcasecmp(word, n->word) == 0)
        {
            return true;
        }
        //Hopping on the next node
        n = n->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function

    //Remainder Hashing with strings
    long sum = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        sum += tolower(word[i]);
    }
    return (sum % N);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dict;
    dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Couldn't open dictionary\n");
        return false;
    }

    char word[LENGTH + 1];

    //Read while there are words in the dictionary
    while (fscanf(dict, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));
        strcpy(n->word, word);

        //Calculate hash value
        int hashValue = hash(n->word);

        //Connecting nodes with equal hash value
        n->next = table[hashValue];

        //Overwrite first node
        table[hashValue] = n;

        counter++;
    }

    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO

    //Go through all the buckets
    for (int i = 0; i < N; i++)
    {
        //Get the next linked list
        node *n = table[i];

        //Free the linked list
        while (n != NULL)
        {
            //Get "original" node
            node *temp = n;

            //Set next node
            n = n->next;

            //free "original" node
            free(temp);
        }
    }

    return true;
}
