#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Function prototype
bool is_valid_key(int argc, string argv[]);
bool only_digits(string s);
char rotate(char c, int n);

int main(int argc, string argv[])
{
    // Check for valid key
    if (!is_valid_key(argc, argv))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Get key and plaintext to encrypt
    int key = atoi(argv[1]);
    string plaintext = get_string("plaintext: ");

    // Encrypt plaintext and print ciphertext
    printf("ciphertext: ");
    for (int i = 0; plaintext[i] != '\0'; i++)
    {
        printf("%c", rotate(plaintext[i], key));
    }
    printf("\n");
}

// Validate user input
bool is_valid_key(int argc, string argv[])
{
    return argc == 2 && argv[1] > 0 && only_digits(argv[1]);
}

// Check if string contains only digits
bool only_digits(string s)
{
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

// Rotate char to the right by n
char rotate(char c, int n)
{
    if (isupper(c))
    {
        return (c - 'A' + n) % 26 + 'A';
    }
    if (islower(c))
    {
        return (c - 'a' + n) % 26 + 'a';
    }

    return c;
}
