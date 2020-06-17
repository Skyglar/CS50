// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

void trie_free(node *root);
// Represents a trie
node *root;
unsigned int words = 0;

int hash(char word) {
    int prefix = tolower(word) - 'a';
    //set \\` symbol to 26 index of an array
    return prefix == -58 ? N - 1 : prefix;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
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

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        node *current = root;
        // iterate through the word
        for(int i = 0, length = strlen(word); i < length; i++)
        {
            //calculate index for children[] char by char of the word
            int prefix = tolower(word[i]) - 'a';
            //set \\` symbol to 26 index of an array
            if(prefix == -58) prefix = N - 1;

            if(current->children[prefix] == NULL)
            {
                node *new_node = malloc(sizeof(node));
                //is_word = false by default
                new_node->is_word = false;
                //make null children values
                for(int j = 0; j < N; j++)
                {
                    new_node->children[j] = NULL;
                }
                current->children[prefix] = new_node;
            }
        
            current = current->children[prefix];
            
            // insufficient memory
            if (!current)
            {
                return false;
            }
        }
        current->is_word = true;
        words++;

    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return words;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *current = root;
    // iterate through the word
    for(int i = 0, length = strlen(word); i < length; i++)
    {
        //calculate index for children[] char by char of the word
        int prefix = tolower(word[i]) - 'a';
        //set \\` symbol to 26 index of an array
        if(prefix == -58) prefix = N - 1;

        if(current->children[prefix] == NULL)
        {
               return false;
        }
        current = current->children[prefix];   
    }
        
    return current->is_word;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    trie_free(root);
    
    return true;
}

void trie_free(node *current)
{
    if (current == NULL)
    {
        return;
    }

    for (int i = 0; i < N; i++)
    {
        trie_free(current->children[i]);
    }

    free(current);
}


