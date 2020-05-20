// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 17576;
//const unsigned int N = 676;

// Hash table
node *table[N];

void init(void)
{
    for (int i = 0; i < N; i++)
    {
        table[i] = malloc(sizeof(node));
        table[i] -> next = NULL;
    }
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Change case to lower case
    int word_len = strlen(word);
    char lower_word[word_len + 1];
    for (int i = 0; i <= word_len; i++)
    {
        lower_word[i] = tolower(word[i]);
    }

    node *trav = table[hash(word)];

    while (trav -> next != NULL)
    {
        trav = trav -> next;
        if (strcmp(lower_word, trav -> word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // 'A' = 65 (dec - ascii)
    // Check if the first two letters are alphanumeric
    int first_letter = (isalpha(word[0])) ? toupper(word[0]) : 'A';
    int second_letter;
    int third_letter;

    // If the word ends in its 2nd character, set second_letter and third_letter to 'A'
    if (word[1] == 0 || !isalpha(word[1]))
    {
        second_letter = 'A';
        third_letter = 'A';
    }
    else
    {
        // If the word doesn't end on the second character, set its value to the second letter of the word
        // and determine the thid letter
        second_letter = toupper(word[1]);
        third_letter = (isalpha(word[2])) ? toupper(word[2]) : 'A';
    }
    return (first_letter - (int) 'A') * 26 * 26 + (second_letter - (int) 'A') * 26 + (third_letter - (int) 'A');
    //return (first_letter - (int) 'A') * 26 + (second_letter - (int) 'A');
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    init();
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");

    // Check if opening the file was succesful
    if (file == NULL)
    {
        printf("Opening the dictionary file returned a NULL pointer\n");
        return false;
    }

    // Bucket index
    int word_index = 0;
    char word[LENGTH + 1];

    for (int i = 0; i < LENGTH + 1; i++)
    {
        word[i] = '\0';
    }

    // Read one character at a time to build the word
    for (int c = fgetc(file); c != EOF; c = fgetc(file))
    {
        if (c == '\n')
        {
            // The word got to an end, hash it to find in which bucket it must go
            int bucket_index = hash(word);

            // Allocate memory for the next node in this bucket and initialize it
            node *new_node = malloc(sizeof(node));
            new_node -> next = table[bucket_index] -> next;
            strcpy(new_node -> word, word);

            // Add new_node to the bucket chain
            table[bucket_index] -> next = new_node;

            // Get ready for the next word
            for (int i = 0; i < word_index; i++)
            {
                word[i] = '\0';
            }
            word_index = 0;
            continue;
        }
        else
        {
            word[word_index] = c;
            word_index++;
        }
    }

    // Check if there was an error
    if (ferror(file))
    {
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int n_words = 0;
    for (int i = 0; i < N; i++)
    {
        node *trav = table[i];
        while (trav -> next != NULL)
        {
            trav = trav -> next;
            n_words++;
        }
    }
    return n_words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        do
        {
            node *tmp = table[i] -> next;
            free(table[i]);
            table[i] = tmp;
        }
        while (table[i] != NULL);
    }
    return true;
}
