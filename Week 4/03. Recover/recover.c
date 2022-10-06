#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Open memory card and check if it is opened
    FILE *input = fopen(argv[1], "r");

    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Create buffer to store data
    unsigned char buffer[512];
    int count = 0;
    char filename[8];
    FILE *img = NULL;

    // Iterate over file for each 512 bytes to find JPEGs
    while (fread(buffer, 512, 1, input))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (img != NULL)
            {
                fclose(img);
            }

            sprintf(filename, "%03i.jpg", count);
            img = fopen(filename, "w");
            count++;
        }

        if (img != NULL)
        {
            fwrite(buffer, 512, 1, img);
        }
    }

    // Close files to prevent memory leaks
    fclose(img);
    fclose(input);
}
