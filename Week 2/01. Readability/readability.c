#include <cs50.h>
#include <stdio.h>
#include <math.h>

// Function prototypes
int calculate_letters(string text);
int calculate_words(string text);
int calculate_sentences(string text);
int coleman_liau_index(int letters, int words, int sentences);
void print_grade(int grade);

int main(void)
{
    // Calculate each of the three variables
    string text = get_string("Text: ");
    int letters = calculate_letters(text);
    int words = calculate_words(text);
    int sentences = calculate_sentences(text);

    // Calculate the Coleman-Liau index and print grade
    int index = coleman_liau_index(letters, words, sentences);
    print_grade(index);
}

// Calculate number of letters in text
int calculate_letters(string text)
{
    int letters = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            letters++;
        }
    }
    return letters;
}

// Calculate number of words in text
int calculate_words(string text)
{
    int words = 1;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == ' ')
        {
            words++;
        }
    }
    return words;
}

// Calculate number of sentences in text
int calculate_sentences(string text)
{
    int sentences = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}

// Calculate the Coleman-Liau index
// Round the index to the nearest integer
int coleman_liau_index(int letters, int words, int sentences)
{
    float L = (letters / (float)words) * 100;
    float S = (sentences / (float)words) * 100;

    return round(0.0588 * L - 0.296 * S - 15.8);
}

// Print the grade
void print_grade(int grade)
{
    if (grade < 1)
    {
        printf("Before Grade 1\n");
        return;
    }
    if (grade >= 16)
    {
        printf("Grade 16+\n");
        return;
    }

    printf("Grade %i\n", grade);
}
