// Computes the approximate grade level needed to comprehend some text
// using the Coleman-Liau index

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
float cole_liau_index(int letters, int words, int sentences);

int main(void)
{
    // Prompts for the text
    string text = get_string("Text: ");

    float index = cole_liau_index(count_letters(text), count_words(text), count_sentences(text));

    // Print results
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.0f\n", index);
    }

}

// Count the number of letters in a text
int count_letters(string text)
{
    int letters_n = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            letters_n++;
        }
    }

    return letters_n;
}

// Count the number of words in a text
int count_words(string text)
{
    int words_n = 0, in_word_flag = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]) && in_word_flag == 0)
        {
            in_word_flag = 1;
            words_n++;
        }
        else if (isblank(text[i]) && in_word_flag == 1)
        {
            in_word_flag = 0;
        }
    }

    return words_n;
}

// Count the number of sentences in a text
int count_sentences(string text)
{
    int sentences_n = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences_n++;
        }
    }

    return sentences_n;
}

// Calculate Coleman-Liau index
float cole_liau_index(int letters, int words, int sentences)
{
    return 0.0588 * (float) letters / words * 100 - 0.296 * (float) sentences / words * 100 - 15.8;
}