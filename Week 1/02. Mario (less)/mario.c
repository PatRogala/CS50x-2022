// Print mario pyramid
#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;

    // Prompt user for valid height
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Print pyramid
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < height - i - 1; j++)
        {
            printf(" ");
        }
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }
        printf("\n");
    }
}
