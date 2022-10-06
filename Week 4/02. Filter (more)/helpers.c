#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate average of RGB values
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Reflext pixels horizontally
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create copy of original image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Blur image using copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            float counter = 0.0;

            // Check for all 9 pixels around
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (i + k >= 0 && i + k < height && j + l >= 0 && j + l < width)
                    {
                        sumRed += copy[i + k][j + l].rgbtRed;
                        sumGreen += copy[i + k][j + l].rgbtGreen;
                        sumBlue += copy[i + k][j + l].rgbtBlue;
                        counter++;
                    }
                }
            }

            image[i][j].rgbtRed = round(sumRed / counter);
            image[i][j].rgbtGreen = round(sumGreen / counter);
            image[i][j].rgbtBlue = round(sumBlue / counter);
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Create copy of original image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Detect edges using copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRedX = 0;
            int sumGreenX = 0;
            int sumBlueX = 0;
            int sumRedY = 0;
            int sumGreenY = 0;
            int sumBlueY = 0;

            // Check for all 9 pixels around
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (i + k >= 0 && i + k < height && j + l >= 0 && j + l < width)
                    {
                        // Calculate Gx
                        sumRedX += copy[i + k][j + l].rgbtRed * Gx[k + 1][l + 1];
                        sumGreenX += copy[i + k][j + l].rgbtGreen * Gx[k + 1][l + 1];
                        sumBlueX += copy[i + k][j + l].rgbtBlue * Gx[k + 1][l + 1];

                        // Calculate Gy
                        sumRedY += copy[i + k][j + l].rgbtRed * Gy[k + 1][l + 1];
                        sumGreenY += copy[i + k][j + l].rgbtGreen * Gy[k + 1][l + 1];
                        sumBlueY += copy[i + k][j + l].rgbtBlue * Gy[k + 1][l + 1];
                    }
                }
            }

            // Calculate Gx and Gy
            int GxRed = round(sqrt(pow(sumRedX, 2) + pow(sumRedY, 2)));
            int GxGreen = round(sqrt(pow(sumGreenX, 2) + pow(sumGreenY, 2)));
            int GxBlue = round(sqrt(pow(sumBlueX, 2) + pow(sumBlueY, 2)));

            // Cap RGB values at 255
            GxRed = GxRed > 255 ? 255 : GxRed;
            GxGreen = GxGreen > 255 ? 255 : GxGreen;
            GxBlue = GxBlue > 255 ? 255 : GxBlue;

            image[i][j].rgbtRed = GxRed;
            image[i][j].rgbtGreen = GxGreen;
            image[i][j].rgbtBlue = GxBlue;
        }
    }
}
