#include "helpers.h"
#include <stdarg.h>
#include <math.h>

// Sum all elements of n RGBTRIPLE instances
RGBTRIPLE sobel(RGBTRIPLE pixels[3][3])
{
    // Initialize the result variable
    RGBTRIPLE result_pixel;

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    int gx_rgbtBlue = 0, gx_rgbtGreen = 0, gx_rgbtRed = 0;
    int gy_rgbtBlue = 0, gy_rgbtGreen = 0, gy_rgbtRed = 0;
    int rgbtBlue = 0, rgbtGreen = 0, rgbtRed = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            gx_rgbtBlue += Gx[i][j] * pixels[i][j].rgbtBlue;
            gx_rgbtGreen += Gx[i][j] * pixels[i][j].rgbtGreen;
            gx_rgbtRed += Gx[i][j] * pixels[i][j].rgbtRed;

            gy_rgbtBlue += Gy[i][j] * pixels[i][j].rgbtBlue;
            gy_rgbtGreen += Gy[i][j] * pixels[i][j].rgbtGreen;
            gy_rgbtRed += Gy[i][j] * pixels[i][j].rgbtRed;
        }
    }

    rgbtBlue = fmin(round(sqrt(pow(gx_rgbtBlue, 2) + pow(gy_rgbtBlue, 2))), 255);
    rgbtGreen = fmin(round(sqrt(pow(gx_rgbtGreen, 2) + pow(gy_rgbtGreen, 2))), 255);
    rgbtRed = fmin(round(sqrt(pow(gx_rgbtRed, 2) + pow(gy_rgbtRed, 2))), 255);

    result_pixel.rgbtBlue = (BYTE) rgbtBlue;
    result_pixel.rgbtGreen = (BYTE) rgbtGreen;
    result_pixel.rgbtRed = (BYTE) rgbtRed;

    return result_pixel;
}

// Compute a RGBTRIPLE instance which elements are the average of n RGBTRIPLE instances
RGBTRIPLE average_pixels(int count, ...)
{
    // Initialize the result variable
    RGBTRIPLE result_pixel;

    float aux_rgbtBlue = 0, aux_rgbtGreen = 0, aux_rgbtRed = 0;

    va_list ap;

    // Requires the last fixed parameter (to get the address)
    va_start(ap, count);
    for (int i = 0; i < count; i++)
    {
        // Increments ap to the next argument.
        RGBTRIPLE current_pixel = va_arg(ap, RGBTRIPLE);

        aux_rgbtBlue += (float) current_pixel.rgbtBlue;
        aux_rgbtGreen += (float) current_pixel.rgbtGreen;
        aux_rgbtRed += (float) current_pixel.rgbtRed;

    }
    va_end(ap);

    result_pixel.rgbtBlue = (BYTE) round(aux_rgbtBlue / count);
    result_pixel.rgbtGreen = (BYTE) round(aux_rgbtGreen / count);
    result_pixel.rgbtRed = (BYTE) round(aux_rgbtRed / count);

    return result_pixel;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // The grayscale element will be the average of all the colors in that pixel
            float gray_value = (float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3;

            // Assign the values to the elements in the struct, so that each color has the same value
            image[i][j].rgbtBlue = (BYTE) round(gray_value);
            image[i][j].rgbtGreen = (BYTE) round(gray_value);
            image[i][j].rgbtRed = (BYTE) round(gray_value);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE reflected_image[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Create reflected image
            reflected_image[i][width - 1 - j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Assign the elements of the reflected image back to the original image
            image[i][j] = reflected_image[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurred_image[height][width];

    // Compute corners
    // The indices "height" and "width" are always followed by a "- 1" operation,
    // because, as the arrays start at 0, the last position is actually "height - 1" and "width - 1"
    blurred_image[0][0] = average_pixels(4, image[0][0], image[0][1], image[1][0], image[1][1]);
    blurred_image[height - 1][0] = average_pixels(4, image[height - 1][0], image[height - 2][0], image[height - 1][1],
                                   image[height - 2][1]);

    blurred_image[0][width - 1] = average_pixels(4, image[0][width - 1], image[0][width - 2], image[1][width - 1], image[1][width - 2]);
    blurred_image[height - 1][width - 1] = average_pixels(4, image[height - 1][width - 1], image[height - 2][width - 1],
                                           image[height - 1][width - 2], image[height - 2][width - 2]);

    // Compute borders
    for (int i = 1; i < height - 1; i++)
    {
        // Compute first column
        blurred_image[i][0] = average_pixels(6, image[i - 1][0], image[i][0], image[i + 1][0], image[i - 1][1], image[i][1],
                                             image[i + 1][1]);

        // Compute last column
        blurred_image[i][width - 1] = average_pixels(6, image[i - 1][width - 1], image[i][width - 1], image[i + 1][width - 1],
                                      image[i - 1][width - 2], image[i][width - 2], image[i + 1][width - 2]);
    }

    for (int i = 1; i < width - 1; i++)
    {
        // Compute first row
        blurred_image[0][i] = average_pixels(6, image[0][i - 1], image[0][i], image[0][i + 1], image[1][i - 1], image[1][i],
                                             image[1][i + 1]);

        // Compute last row
        blurred_image[height - 1][i] = average_pixels(6, image[height - 1][i - 1], image[height - 1][i], image[height - 1][i + 1],
                                       image[height - 2][i - 1], image[height - 2][i], image[height - 2][i + 1]);
    }

    // Compute all other pixels
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            blurred_image[i][j] = average_pixels(9, image[i - 1][j - 1],
                                                 image[i - 1][j],
                                                 image[i - 1][j + 1],
                                                 image[i][j - 1],
                                                 image[i][j],
                                                 image[i][j + 1],
                                                 image[i + 1][j - 1],
                                                 image[i + 1][j],
                                                 image[i + 1][j + 1]);
        }
    }

    // Assign the elements of the blurred image back to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurred_image[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE edge_image[height][width];
    RGBTRIPLE black_pixel = {0, 0, 0};

    // Compute the pixels the form the box around each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Build the pixel box checking corners and border conditions
            RGBTRIPLE sobel_pixels[3][3];
            if (i == 0 || j == 0)
            {
                sobel_pixels[0][0] = black_pixel;
            }
            else
            {
                sobel_pixels[0][0] = image[i - 1][j - 1];
            }

            if (i == 0)
            {
                sobel_pixels[0][1] = black_pixel;
            }
            else
            {
                sobel_pixels[0][1] = image[i - 1][j];
            }

            if (i == 0 || j == width - 1)
            {
                sobel_pixels[0][2] = black_pixel;
            }
            else
            {
                sobel_pixels[0][2] = image[i - 1][j + 1];
            }

            if (j == 0)
            {
                sobel_pixels[1][0] = black_pixel;
            }
            else
            {
                sobel_pixels[1][0] = image[i][j - 1];
            }

            // This is the middle pixel of the box, and it is always the current analyzed pixel, there are no conditions to check
            sobel_pixels[1][1] = image[i][j];

            if (j == width - 1)
            {
                sobel_pixels[1][2] = black_pixel;
            }
            else
            {
                sobel_pixels[1][2] = image[i][j + 1];
            }

            if (i == height - 1 || j == 0)
            {
                sobel_pixels[2][0] = black_pixel;
            }
            else
            {
                sobel_pixels[2][0] = image[i + 1][j - 1];
            }

            if (i == height - 1)
            {
                sobel_pixels[2][1] = black_pixel;
            }
            else
            {
                sobel_pixels[2][1] = image[i + 1][j];
            }

            if (i == height - 1 || j == width - 1)
            {
                sobel_pixels[2][2] = black_pixel;
            }
            else
            {
                sobel_pixels[2][2] = image[i + 1][j + 1];
            }


            edge_image[i][j] = sobel(sobel_pixels);
        }
    }

    // Assign the elements of the produced image back to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = edge_image[i][j];
        }
    }

    return;
}


