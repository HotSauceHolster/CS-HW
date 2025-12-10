#include "helpers.h"
#include <math.h>

RGBTRIPLE blurredPix(int y, int x, int height, int width, RGBTRIPLE image[height][width]);
RGBTRIPLE sobelPix(int y, int x, int height, int width, RGBTRIPLE image[height][width]);
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Calculate average RGB value and assign to each color channel
            float avgVal =
                round((image[h][w].rgbtBlue + image[h][w].rgbtGreen + image[h][w].rgbtRed) / 3.0);
            image[h][w].rgbtBlue = (int) avgVal;
            image[h][w].rgbtGreen = (int) avgVal;
            image[h][w].rgbtRed = (int) avgVal;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];
    // Read original image from right to left into tempImage
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            tempImage[h][w] = image[h][width - 1 - w];
        }
    }
    // Copy reversed tempImage into image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = tempImage[h][w];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];
    // Get average values of all adjacent pixels
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            tempImage[h][w] = blurredPix(h, w, height, width, image);
        }
    }
    // Copy blurred tempImage into image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = tempImage[h][w];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];
    // Get sobel values of all adjacent pixels
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            tempImage[h][w] = sobelPix(h, w, height, width, image);
        }
    }
    // Copy highlighted edges tempImage into image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = tempImage[h][w];
        }
    }
    return;
}

RGBTRIPLE blurredPix(int y, int x, int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixel[1][1];
    float blue = 0.0;
    float green = 0.0;
    float red = 0.0;

    if (y == 0)
    {
        // Top left corner case of 4 pixels
        if (x == 0)
        {
            for (int h = 0; h < y + 2; h++)
            {
                for (int w = 0; w < x + 2; w++)
                {
                    blue += image[h][w].rgbtBlue;
                    green += image[h][w].rgbtGreen;
                    red += image[h][w].rgbtRed;
                }
            }
            blue = blue / 4.0;
            green = green / 4.0;
            red = red / 4.0;
        }
        // Top right corner case of 4 pixels
        else if (x == width - 1)
        {
            for (int h = 0; h < y + 2; h++)
            {
                for (int w = -1; w < 1; w++)
                {
                    blue += image[h][x + w].rgbtBlue;
                    green += image[h][x + w].rgbtGreen;
                    red += image[h][x + w].rgbtRed;
                }
            }
            blue = blue / 4.0;
            green = green / 4.0;
            red = red / 4.0;
        }
        // Top edge between corners with 6 pixels
        else
        {
            for (int h = 0; h < y + 2; h++)
            {
                for (int w = -1; w < 2; w++)
                {
                    blue += image[h][x + w].rgbtBlue;
                    green += image[h][x + w].rgbtGreen;
                    red += image[h][x + w].rgbtRed;
                }
            }
            blue = blue / 6.0;
            green = green / 6.0;
            red = red / 6.0;
        }
    }
    else if (y == height - 1)
    {
        // Bottom left corner case with 4 pixels
        if (x == 0)
        {
            for (int h = -1; h < 1; h++)
            {
                for (int w = 0; w < x + 2; w++)
                {
                    blue += image[y + h][w].rgbtBlue;
                    green += image[y + h][w].rgbtGreen;
                    red += image[y + h][w].rgbtRed;
                }
            }
            blue = blue / 4.0;
            green = green / 4.0;
            red = red / 4.0;
        }
        // Bottom right corner case with 4 pixels
        else if (x == width - 1)
        {
            for (int h = -1; h < 1; h++)
            {
                for (int w = -1; w < 1; w++)
                {
                    blue += image[y + h][x + w].rgbtBlue;
                    green += image[y + h][x + w].rgbtGreen;
                    red += image[y + h][x + w].rgbtRed;
                }
            }
            blue = blue / 4.0;
            green = green / 4.0;
            red = red / 4.0;
        }
        // Bottom edge between corners with 6 pixels
        else
        {
            for (int h = -1; h < 1; h++)
            {
                for (int w = -1; w < 2; w++)
                {
                    blue += image[y + h][x + w].rgbtBlue;
                    green += image[y + h][x + w].rgbtGreen;
                    red += image[y + h][x + w].rgbtRed;
                }
            }
            blue = blue / 6.0;
            green = green / 6.0;
            red = red / 6.0;
        }
    }
    else
    {
        // Left edge between corners with 6 pixels
        if (x == 0)
        {
            for (int h = -1; h < 2; h++)
            {
                for (int w = 0; w < x + 2; w++)
                {
                    blue += image[y + h][w].rgbtBlue;
                    green += image[y + h][w].rgbtGreen;
                    red += image[y + h][w].rgbtRed;
                }
            }
            blue = blue / 6.0;
            green = green / 6.0;
            red = red / 6.0;
        }
        // Right edge between corners with 6 pixels
        else if (x == width - 1)
        {
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 1; w++)
                {
                    blue += image[y + h][x + w].rgbtBlue;
                    green += image[y + h][x + w].rgbtGreen;
                    red += image[y + h][x + w].rgbtRed;
                }
            }
            blue = blue / 6.0;
            green = green / 6.0;
            red = red / 6.0;
        }
        // Anywhere in picture not touching an edge or corner with 9 pixels
        else
        {
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w++)
                {
                    blue += image[y + h][x + w].rgbtBlue;
                    green += image[y + h][x + w].rgbtGreen;
                    red += image[y + h][x + w].rgbtRed;
                }
            }
            blue = blue / 9.0;
            green = green / 9.0;
            red = red / 9.0;
        }
    }
    // Put the averaged color values into the returning pixel
    pixel[0][0].rgbtBlue = round(blue);
    pixel[0][0].rgbtGreen = round(green);
    pixel[0][0].rgbtRed = round(red);
    // Return averaged (blurred) pixel
    return pixel[0][0];
}

RGBTRIPLE sobelPix(int y, int x, int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixel[1][1];

    float blue = 0.0;
    float green = 0.0;
    float red = 0.0;

    float dxblue = 0.0;
    float dxgreen = 0.0;
    float dxred = 0.0;

    float dyblue = 0.0;
    float dygreen = 0.0;
    float dyred = 0.0;
    // For each color channel, loop through changes in x direction, changes in y direction
    // then take the sum of the square of both and get the square root of it
    if (y == 0)
    {
        // Top left
        if (x == 0)
        {
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w += 2)
                {
                    if (w == 1)
                    {
                        if (h == 0)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * 2.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * 2.0);
                            dxred += (image[y + h][x + w].rgbtRed * 2.0);
                        }
                        else if (h == 1)
                        {
                            dxblue += (float) image[y + h][x + w].rgbtBlue;
                            dxgreen += (float) image[y + h][x + w].rgbtGreen;
                            dxred += (float) image[y + h][x + w].rgbtRed;
                        }
                    }
                }
            }
            for (int w = -1; w < 2; w++)
            {
                for (int h = -1; h < 2; h += 2)
                {
                    if (h == 1)
                    {
                        if (w == 0)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * 2.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * 2.0);
                            dyred += (image[y + h][x + w].rgbtRed * 2.0);
                        }
                        else if (w == 1)
                        {
                            dyblue += (float) image[y + h][x + w].rgbtBlue;
                            dygreen += (float) image[y + h][x + w].rgbtGreen;
                            dyred += (float) image[y + h][x + w].rgbtRed;
                        }
                    }
                }
            }
        }
        // Top right
        else if (x == width - 1)
        {
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w += 2)
                {
                    if (w == -1)
                    {
                        if (h == 0)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * -2.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * -2.0);
                            dxred += (image[y + h][x + w].rgbtRed * -2.0);
                        }
                        else if (h == 1)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * -1.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * -1.0);
                            dxred += (image[y + h][x + w].rgbtRed * -1.0);
                        }
                    }
                }
            }
            for (int w = -1; w < 2; w++)
            {
                for (int h = -1; h < 2; h += 2)
                {
                    if (h == 1)
                    {
                        if (w == -1)
                        {
                            dyblue += (float) image[y + h][x + w].rgbtBlue;
                            dygreen += (float) image[y + h][x + w].rgbtGreen;
                            dyred += (float) image[y + h][x + w].rgbtRed;
                        }
                        else if (w == 0)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * 2.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * 2.0);
                            dyred += (image[y + h][x + w].rgbtRed * 2.0);
                        }
                    }
                }
            }
        }
        // Top edge, not corner adjacent
        else
        {
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w += 2)
                {
                    if (w == -1)
                    {
                        if (h == 0)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * -2.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * -2.0);
                            dxred += (image[y + h][x + w].rgbtRed * -2.0);
                        }
                        else if (h == 1)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * -1.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * -1.0);
                            dxred += (image[y + h][x + w].rgbtRed * -1.0);
                        }
                    }
                    else if (w == 1)
                    {
                        if (h == 0)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * 2.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * 2.0);
                            dxred += (image[y + h][x + w].rgbtRed * 2.0);
                        }
                        else if (h == 1)
                        {
                            dxblue += (float) image[y + h][x + w].rgbtBlue;
                            dxgreen += (float) image[y + h][x + w].rgbtGreen;
                            dxred += (float) image[y + h][x + w].rgbtRed;
                        }
                    }
                }
            }
            for (int w = -1; w < 2; w++)
            {
                for (int h = -1; h < 2; h += 2)
                {
                    if (h == 1)
                    {
                        if (w == -1 || w == 1)
                        {
                            dyblue += image[y + h][x + w].rgbtBlue;
                            dygreen += image[y + h][x + w].rgbtGreen;
                            dyred += image[y + h][x + w].rgbtRed;
                        }
                        else if (w == 0)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * 2.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * 2.0);
                            dyred += (image[y + h][x + w].rgbtRed * 2.0);
                        }
                    }
                }
            }
        }
    }
    else if (y == height - 1)
    {
        // Bottom left corner
        if (x == 0)
        {
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w += 2)
                {
                    if (w == 1)
                    {
                        if (h == 0)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * 2.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * 2.0);
                            dxred += (image[y + h][x + w].rgbtRed * 2.0);
                        }
                        else if (h == -1)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * 1.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * 1.0);
                            dxred += (image[y + h][x + w].rgbtRed * 1.0);
                        }
                    }
                }
            }
            for (int w = -1; w < 2; w++)
            {
                for (int h = -1; h < 2; h += 2)
                {
                    if (h == -1)
                    {
                        if (w == 0)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * -2.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * -2.0);
                            dyred += (image[y + h][x + w].rgbtRed * -2.0);
                        }
                        else if (w == 1)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * -1.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * -1.0);
                            dyred += (image[y + h][x + w].rgbtRed * -1.0);
                        }
                    }
                }
            }
        }
        // Bottom right corner
        else if (x == width - 1)
        {
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w += 2)
                {
                    if (w == -1)
                    {
                        if (h == 0)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * -2.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * -2.0);
                            dxred += (image[y + h][x + w].rgbtRed * -2.0);
                        }
                        else if (h == -1)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * -1.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * -1.0);
                            dxred += (image[y + h][x + w].rgbtRed * -1.0);
                        }
                    }
                }
            }
            for (int w = -1; w < 2; w++)
            {
                for (int h = -1; h < 2; h += 2)
                {
                    if (h == -1)
                    {
                        if (w == 0)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * -2.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * -2.0);
                            dyred += (image[y + h][x + w].rgbtRed * -2.0);
                        }
                        else if (w == -1)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * -1.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * -1.0);
                            dyred += (image[y + h][x + w].rgbtRed * -1.0);
                        }
                    }
                }
            }
        }
        // Bottom edge, not corner adjacent
        else
        {
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w += 2)
                {
                    if (w == -1)
                    {
                        if (h == 0)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * -2.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * -2.0);
                            dxred += (image[y + h][x + w].rgbtRed * -2.0);
                        }
                        else if (h == -1)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * -1.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * -1.0);
                            dxred += (image[y + h][x + w].rgbtRed * -1.0);
                        }
                    }
                    else if (w == 1)
                    {
                        if (h == 0)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * 2.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * 2.0);
                            dxred += (image[y + h][x + w].rgbtRed * 2.0);
                        }
                        else if (h == -1)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * 1.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * 1.0);
                            dxred += (image[y + h][x + w].rgbtRed * 1.0);
                        }
                    }
                }
            }
            for (int w = -1; w < 2; w++)
            {
                for (int h = -1; h < 2; h += 2)
                {
                    if (h == -1)
                    {
                        if (w == 0)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * -2.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * -2.0);
                            dyred += (image[y + h][x + w].rgbtRed * -2.0);
                        }
                        else if (w == -1 || w == 1)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * -1.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * -1.0);
                            dyred += (image[y + h][x + w].rgbtRed * -1.0);
                        }
                    }
                }
            }
        }
    }
    else
    {
        // Left edge, not corner adjacent
        if (x == 0)
        {
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w += 2)
                {
                    if (w == 1)
                    {
                        if (h == 0)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * 2.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * 2.0);
                            dxred += (image[y + h][x + w].rgbtRed * 2.0);
                        }
                        else if (h == -1 || h == 1)
                        {
                            dxblue += (float) image[y + h][x + w].rgbtBlue;
                            dxgreen += (float) image[y + h][x + w].rgbtGreen;
                            dxred += (float) image[y + h][x + w].rgbtRed;
                        }
                    }
                }
            }
            for (int w = -1; w < 2; w++)
            {
                for (int h = -1; h < 2; h += 2)
                {
                    if (h == 1)
                    {
                        if (w == 0)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * 2.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * 2.0);
                            dyred += (image[y + h][x + w].rgbtRed * 2.0);
                        }
                        else if (w == 1)
                        {
                            dyblue += (float) image[y + h][x + w].rgbtBlue;
                            dygreen += (float) image[y + h][x + w].rgbtGreen;
                            dyred += (float) image[y + h][x + w].rgbtRed;
                        }
                    }
                    else if (h == -1)
                    {
                        if (w == 0)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * -2.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * -2.0);
                            dyred += (image[y + h][x + w].rgbtRed * -2.0);
                        }
                        else if (w == 1)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * -1.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * -1.0);
                            dyred += (image[y + h][x + w].rgbtRed * -1.0);
                        }
                    }
                }
            }
        }
        // Right edge, not corner adjacent
        else if (x == width - 1)
        {
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w += 2)
                {
                    if (w == -1)
                    {
                        if (h == 0)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * -2.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * -2.0);
                            dxred += (image[y + h][x + w].rgbtRed * -2.0);
                        }
                        else if (h == -1 || h == 1)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * -1.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * -1.0);
                            dxred += (image[y + h][x + w].rgbtRed * -1.0);
                        }
                    }
                }
            }
            for (int w = -1; w < 2; w++)
            {
                for (int h = -1; h < 2; h += 2)
                {
                    if (h == 1)
                    {
                        if (w == -1)
                        {
                            dyblue += (float) image[y + h][x + w].rgbtBlue;
                            dygreen += (float) image[y + h][x + w].rgbtGreen;
                            dyred += (float) image[y + h][x + w].rgbtRed;
                        }
                        else if (w == 0)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * 2.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * 2.0);
                            dyred += (image[y + h][x + w].rgbtRed * 2.0);
                        }
                    }
                    else if (h == -1)
                    {
                        if (w == -1)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * -1.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * -1.0);
                            dyred += (image[y + h][x + w].rgbtRed * -1.0);
                        }
                        else if (w == 0)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * -2.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * -2.0);
                            dyred += (image[y + h][x + w].rgbtRed * -2.0);
                        }
                    }
                }
            }
        }
        // Not adjacent to any corners or edges
        else
        {
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w += 2)
                {
                    if (w == -1)
                    {
                        if (h == 0)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * -2.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * -2.0);
                            dxred += (image[y + h][x + w].rgbtRed * -2.0);
                        }
                        else if (h == -1 || h == 1)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * -1.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * -1.0);
                            dxred += (image[y + h][x + w].rgbtRed * -1.0);
                        }
                    }
                    else if (w == 1)
                    {
                        if (h == 0)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * 2.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * 2.0);
                            dxred += (image[y + h][x + w].rgbtRed * 2.0);
                        }
                        else if (h == -1 || h == 1)
                        {
                            dxblue += (image[y + h][x + w].rgbtBlue * 1.0);
                            dxgreen += (image[y + h][x + w].rgbtGreen * 1.0);
                            dxred += (image[y + h][x + w].rgbtRed * 1.0);
                        }
                    }
                }
            }
            for (int w = -1; w < 2; w++)
            {
                for (int h = -1; h < 2; h += 2)
                {
                    if (h == 1)
                    {
                        if (w == -1 || w == 1)
                        {
                            dyblue += (float) image[y + h][x + w].rgbtBlue;
                            dygreen += (float) image[y + h][x + w].rgbtGreen;
                            dyred += (float) image[y + h][x + w].rgbtRed;
                        }
                        else if (w == 0)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * 2.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * 2.0);
                            dyred += (image[y + h][x + w].rgbtRed * 2.0);
                        }
                    }
                    else if (h == -1)
                    {
                        if (w == -1 || w == 1)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * -1.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * -1.0);
                            dyred += (image[y + h][x + w].rgbtRed * -1.0);
                        }
                        else if (w == 0)
                        {
                            dyblue += (image[y + h][x + w].rgbtBlue * -2.0);
                            dygreen += (image[y + h][x + w].rgbtGreen * -2.0);
                            dyred += (image[y + h][x + w].rgbtRed * -2.0);
                        }
                    }
                }
            }
        }
    }
    // Perform sobel function on each color channel, then check if value is out of range
    blue = round(sqrt((dxblue * dxblue) + (dyblue * dyblue)));
    if (blue > 255)
    {
        blue = 255;
    }
    green = round(sqrt((dxgreen * dxgreen) + (dygreen * dygreen)));
    if (green > 255)
    {
        green = 255;
    }
    red = round(sqrt((dxred * dxred) + (dyred * dyred)));
    if (red > 255)
    {
        red = 255;
    }
    // Input sobel color values to return pixel and return it
    pixel[0][0].rgbtBlue = blue;
    pixel[0][0].rgbtGreen = green;
    pixel[0][0].rgbtRed = red;
    return pixel[0][0];
}
