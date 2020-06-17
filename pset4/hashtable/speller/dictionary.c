// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

bool add(char *word);
void tolowerword(char *word);
void unloadLinkedList(node *first);

// Represents a hash table
node *hashtable[N];
int count = 0;

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];
    bool added = false;
    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        added = add(word);
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return added;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return count;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char lowerWord[LENGTH];
    strcpy(lowerWord, word);
    tolowerword(lowerWord);

    int index = hash(word);

    if(hashtable[index] == NULL)
    {
        return false;
    }
    else
    {
        for(node *current = hashtable[index]; current != NULL; current = current->next)
        {
            if(strcmp(current->word, lowerWord) == 0)
            {
                return true;
            }
        }
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for(int i = 0; i < N; i++)
    {
        if(hashtable[i] == NULL)
        {
            continue;
        }
        unloadLinkedList(hashtable[i]);
        hashtable[i] = NULL;
    }

    if(hashtable[0] == NULL)
    {
        return true;
    }
    return false;
}

bool add(char *word)
{
    int index = hash(word);
    if(hashtable[index] == NULL)
    {
        node *newnode = malloc(sizeof(node));
        if(newnode == NULL)
        {
            unload();
            return false;
        }

        strcpy(newnode->word, word);
        newnode->next = NULL;
        hashtable[index] = newnode;
        count++;
        return true;
    }
    else
    {
        for(node *current = hashtable[index]; current != NULL; current = current->next)
        {
            if(current->next == NULL)
            {
                node *newnode = malloc(sizeof(node));
                if(newnode == NULL)
                {
                    unload();
                    return false;
                }
                strcpy(newnode->word, word);
                newnode->next = NULL;
                current->next = newnode;
                count++;

                return true;
            }
        }
    }
    return false;
}

void tolowerword(char *word)
{
    for(int i = 0; word[i] != '\0' && i < LENGTH; i++)
    {
        if(word[i] >= 'a')
        {
            continue;
        }
        word[i] = tolower(word[i]);
    }
}

void recursionUnloadLinkedList(node *first)
{
    while(first != NULL)
    {
        if(first->next == NULL)
        {
            free(first);
            return;
        }
        else
        {
            recursionUnloadLinkedList(first->next);
            first->next = NULL;
        }
    }
}

void unloadLinkedList(node *first)
{
    node *current = first;
    while(current != NULL)
    {
        node *temp = current;
        current = current->next;
        free(temp);
    }
}