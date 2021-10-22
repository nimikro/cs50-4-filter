#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed)/3.0);
            image[i][j].rgbtRed = image[i][j].rgbtBlue;
            image[i][j].rgbtGreen = image[i][j].rgbtBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE temp[3];
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width/2; j++)
        {
            temp[0] = image[i][j].rgbtBlue;
            temp[1] = image[i][j].rgbtRed;
            temp[2] = image[i][j].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][width - j - 1].rgbtBlue = temp[0];
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][width - j - 1].rgbtRed = temp[1];
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][width - j - 1].rgbtGreen = temp[2];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

     //create temporary copy of image;
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }

    }

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int sum_blue;
            int sum_green;
            int sum_red;
            float counter;

            sum_blue = sum_green = sum_red = counter = 0;

        //find pixels that are not in image
        //iterate through the column and row, from -1 to +1, using h and w
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w++)
                {
                    //if pixel is outside the image (column)
                    if (i + h < 0 || i + h > (height - 1))
                    {
                        continue;
                    }
                    //if pixel is outside the image (row)
                    if (j + w  < 0 || j + w > (width - 1))
                    {
                        continue;
                    }
                    //sum up the values of all the pixels
                    sum_blue += copy[i + h][j + w].rgbtBlue;
                    sum_red += copy[i + h][j + w].rgbtRed;
                    sum_green += copy[i + h][j + w].rgbtGreen;
                    counter++;
                }
            }

            //calculate new image
            image[i][j].rgbtRed = round(sum_red / counter);
            image[i][j].rgbtGreen = round(sum_green / counter);
            image[i][j].rgbtBlue = round(sum_blue / counter);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int gx[3][3]={{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3]={{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    //create temporary copy of image;
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int sum_blue_x, sum_blue_y;
            int sum_green_x, sum_green_y;
            int sum_red_x, sum_red_y;
            int sum_blue, sum_green, sum_red;

            sum_blue_x = sum_blue_y = sum_green_x = sum_green_y = sum_red_x = sum_red_y = sum_blue = sum_green = sum_red = 0;

        //find pixels that are not in image
        //iterate through the column and row, from -1 to +1, using h and w for first row and column
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w++)
                {
                    //if pixel is outside the image (column)
                    if (i + h < 0 || i + h > (height - 1))
                    {
                        continue;
                    }
                    //if pixel is outside the image (row)
                    if (j + w  < 0 || j + w > (width - 1))
                    {
                        continue;
                    }
                    //calculate Gx and Gy for every pixel
                    sum_blue_x += (copy[i + h][j + w].rgbtBlue)*gx[h+1][w+1];
                    sum_blue_y += (copy[i + h][j + w].rgbtBlue)*gy[h+1][w+1];
                    sum_red_x += (copy[i + h][j + w].rgbtRed)*gx[h+1][w+1];
                    sum_red_y += (copy[i + h][j + w].rgbtRed)*gy[h+1][w+1];
                    sum_green_x += (copy[i + h][j + w].rgbtGreen)*gx[h+1][w+1];
                    sum_green_y += (copy[i + h][j + w].rgbtGreen)*gy[h+1][w+1];
                }
            }

            //round using Sobel formula
            sum_blue = round(sqrt((sum_blue_x * sum_blue_x) + (sum_blue_y * sum_blue_y)));
            sum_green = round(sqrt((sum_green_x * sum_green_x) + (sum_green_y * sum_green_y)));
            sum_red = round(sqrt((sum_red_x * sum_red_x) + (sum_red_y * sum_red_y)));

            //cap at 255
            if(sum_blue > 255)
            {
                sum_blue = 255;
            }
            if(sum_green > 255)
            {
                sum_green = 255;
            }
            if(sum_red > 255)
            {
                sum_red = 255;
            }

            //calculate new image
            image[i][j].rgbtRed = sum_red;
            image[i][j].rgbtGreen = sum_green;
            image[i][j].rgbtBlue = sum_blue;
        }
    }

    return;
}
