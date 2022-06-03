#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int countLetters(string text);
int countWords(string text);
int countSentences(string text);
int colemanIndex(float L, float S);

int main(void)
{
    string text = get_string("Text: ");

    int letters = countLetters(text);
    int sentences = countSentences(text);
    int words = countWords(text);

    // index = 0.0588 * L - 0.296 * S - 15.8
    float L = ((float) letters / (float) words) * 100;
    float S = ((float) sentences / (float) words) * 100;
    int grade = colemanIndex(L, S);

    //printf("%d, %d, %d, %f, %f", letters, sentences, words, L, S);

    if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %d\n", grade);
    }
}

int countLetters(string text)
{
    int counter = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            counter++;
        }
    }
    return counter;
}

int countWords(string text)
{
    int counter = 0;
    char *pointer = strtok(text, " ");

    while (pointer != NULL)
    {
        pointer = strtok(NULL, " ");
        counter++;
    }
    return counter;
}

int countSentences(string text)
{
    int counter = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            counter++;
        }
    }
    return counter;
}

int colemanIndex(float L, float S)
{
    return (int) round(0.0588 * L - 0.296 * S - 15.8);
}