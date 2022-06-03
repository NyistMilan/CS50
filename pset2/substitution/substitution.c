#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


string getChiper(string key, string plainText);


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    
    for (int j = 0; j < strlen(argv[1]); j++)
    {
        if (!isalpha(argv[1][j]))
        {
            printf("Usage: ./substitution key\n");
            return 1;
        }
    }
    
    for (int x = 0; x < strlen(argv[1]) - 1; x++)
    {
        for (int y = x + 1; y < strlen(argv[1]); y++)
        {
            if (argv[1][x] == argv[1][y])
            {
                printf("Usage: ./substitution key\n");
                return 1;
            }
        }
    }

    string plainText = get_string("plaintext: ");

    string chiperText = getChiper(argv[1], plainText);
    printf("ciphertext: %s\n", chiperText);

    return 0;
}

string getChiper(string key, string plainText)
{
    for (int i = 0; i < strlen(plainText); i++)
    {
        if (isalpha(plainText[i]))
        {

            if (islower(plainText[i]) == 0) //Upper case letter
            {
                plainText[i] = toupper(key[plainText[i] - 65]);
            }
            else if (islower(plainText[i]) != 0) //Lower Case Letter
            {
                plainText[i] = tolower(key[plainText[i] - 97]);
            }
        }

    }
    return plainText;
}