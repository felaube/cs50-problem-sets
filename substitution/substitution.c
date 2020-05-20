// Cryptograph a message replacing all character in the original message by
// a corresponding character in the key
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

string encrypt(string plaintext, string key);

int main(int argc, string argv[])
{
    // Check if the correct number of parameters were given in the initialization
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    int key_length = strlen(argv[1]);

    // Check if 26 characters were given as the "key" parameter
    if (key_length != 26)
    {
        printf("The key must contain 26 characters\n");
        return 1;
    }

    for (int i = 0; i < key_length; i++)
    {
        // Check if the charachter in the key is alphabetic
        if (isalpha(argv[1][i]) == 0)
        {
            printf("All characters in the key must be alphabetic\n");
            return 1;
        }
        // Check if there's any repeated character in the key
        for (int j = i + 1; j < key_length; j++)
        {
            if (argv[1][i] == argv[1][j] || argv[1][i] == argv[1][j] - 32 || argv[1][i] == argv[1][j] + 32)
            {
                printf("Character %c is repeated in the key in the positions %i and %i\n", argv[1][i], i + 1, j + 1);
                return 1;
            }
        }
    }

    // Prompt the user for the message and encrypt it
    string plaintext = get_string("plaintext: ");
    printf("ciphertext: %s\n", encrypt(plaintext, argv[1]));
}

// Encrypt message according to the key
string encrypt(string plaintext, string key)
{
    string ciphertext = plaintext;
    // Iterate through the text replacing corresponding characters
    for (int i = 0; i < strlen(ciphertext); i++)
    {
        if islower(ciphertext[i])
        {
            ciphertext[i] = tolower(key[(int) ciphertext[i] - 97]);
        }
        else if isupper(ciphertext[i])
        {
            ciphertext[i] = toupper(key[(int) ciphertext[i] - 65]);
        }
        // If the character is neither lower or upper, it means it is not an alphabetic character,
        // in this case, it will not be replaced
    }

    return ciphertext;
}