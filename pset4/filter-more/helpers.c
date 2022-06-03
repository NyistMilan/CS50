#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float blue = image[i][j].rgbtBlue;
            float green = image[i][j].rgbtGreen;
            float red = image[i][j].rgbtRed;

            int average = round((red + green + blue) / 3);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Duplicating the image
    RGBTRIPLE original[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            original[i][j] = image[i][j];
        }
    }

    //Loop through the pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sumRed = 0;
            float sumBlue = 0;
            float sumGreen = 0;
            int neighbors = 0;

            //Finding the neighbors of a pixel
            for (int y = i - 1; y < i + 2; y++)
            {
                for (int x = j - 1; x < j + 2; x++)
                {
                    if (x >= 0 && y >= 0 && x < width && y < height)
                    {
                        sumRed += original[y][x].rgbtRed;
                        sumBlue += original[y][x].rgbtBlue;
                        sumGreen += original[y][x].rgbtGreen;
                        neighbors++;
                    }
                }
            }

            //Adding blur to the pixel
            image[i][j].rgbtRed = round(sumRed / neighbors);
            image[i][j].rgbtGreen = round(sumGreen / neighbors);
            image[i][j].rgbtBlue = round(sumBlue / neighbors);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Gx and Gy
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    //Duplicating the image
    RGBTRIPLE original[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            original[i][j] = image[i][j];
        }
    }

    //Loop through the pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sumRedGx = 0;
            float sumRedGy = 0;
            float sumGreenGx = 0;
            float sumGreenGy = 0;
            float sumBlueGx = 0;
            float sumBlueGy = 0;

            //Finding the neighbors of a pixel
            for (int y = i - 1; y < i + 2; y++)
            {
                for (int x = j - 1; x < j + 2; x++)
                {
                    if (x >= 0 && y >= 0 && x < width && y < height)
                    {
                        int row = x < 0 ? (x+1) : x;
                        int col = y < 0 ? (y+1) : y;

                        sumRedGx += original[y][x].rgbtRed * Gx[col % 3][row % 3];
                        sumRedGy += original[y][x].rgbtRed * Gy[col % 3][row % 3];

                        sumGreenGx += original[y][x].rgbtGreen * Gx[col % 3][row % 3];
                        sumGreenGy += original[y][x].rgbtGreen * Gy[col % 3][row % 3];

                        sumBlueGx += original[y][x].rgbtBlue * Gx[col % 3][row % 3];
                        sumBlueGy += original[y][x].rgbtBlue * Gy[col % 3][row % 3];
                        //printf("[%d,%d], ", ((y+1) % 3),((x+1) % 3));
                    }
                    /*else
                    {
                        printf("X[%d,%d], ", ((y+1) % 3),((x+1) % 3));
                    }*/
                }
            }

            //Rounding
            int red = round(sqrt(sumRedGx * sumRedGx + sumRedGy * sumRedGy));
            int green = round(sqrt(sumGreenGx * sumGreenGx + sumGreenGy * sumGreenGy));
            int blue = round(sqrt(sumBlueGx * sumBlueGx + sumBlueGy * sumBlueGy));

            //Setting maximums
            if (red > 255)
            {
                red = 255;
            }

            if (green > 255)
            {
                green = 255;
            }

            if (blue > 255)
            {
                blue = 255;
            }

            //Result
            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;
        }
    }
    return;
}
