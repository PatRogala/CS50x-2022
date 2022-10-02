#include <cs50.h>
#include <stdio.h>
#include <ctype.h>

// Function prototypes
bool are_valid_arguments(int argc, string argv[]);

int main(int argc, string argv[])
{
    if (!are_valid_arguments(argc, argv))
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string plaintext = get_string("plaintext: ");
    printf("ciphertext: ");
    for (int i = 0; plaintext[i] != '\0'; i++)
    {
        if (!isalpha(plaintext[i]))
        {
            printf("%c", plaintext[i]);
            continue;
        }

        int index = plaintext[i] - (isupper(plaintext[i]) ? 'A' : 'a');
        printf("%c", isupper(plaintext[i]) ? toupper(argv[1][index]) : tolower(argv[1][index]));
    }
    printf("\n");
}

// Check if the arguments are valid
bool are_valid_arguments(int argc, string argv[])
{
    // Check if the number of arguments is 2
    if (argc != 2)
    {
        return false;
    }

    // Check if the key is 26 characters long
    if (strlen(argv[1]) != 26)
    {
        return false;
    }

    // Check if the key contains only alphabetic characters
    for (int i = 0; i < 26; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            return false;
        }
    }

    // Check if the key contains each letter exactly once
    for (int i = 0; i < 26; i++)
    {
        for (int j = i + 1; j < 26; j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                return false;
            }
        }
    }

    return true;
}
