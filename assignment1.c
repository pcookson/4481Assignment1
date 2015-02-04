#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "libpnm.h"

#define PBM 1
#define PGM 2
#define PPM 3

/*
* Drawline takes inputs:
* Image struct, start and end of y and x coordinates coresponding to rows and columns
* Uses input information to plot a line from start to finish
* Uses Bresenhams Line Algorithm to plot line
*/
int drawLine(struct PBM_Image *pbmImage, int cols, int rows)
{

    int dcol;
    int drow;
    int startcol;
    int startrow;
    int finishcol;
    int finishrow;




    startcol = cols/2;
    finishcol = cols - cols/4;
    startrow = rows/2;
    finishrow = rows/4;

    //use this algorithm if horizontal rectangle or square
    if(cols>=rows)
    {
        //starting parameters
        dcol = finishcol-startcol;
        drow = startrow - finishrow;
        int D = 2*drow - dcol;
        int y = startrow;
        int x;

        for(x = startcol; x<=finishcol; x++)
        {

            if(D > 0)
            {
                y--;
                //line from center to top right
                pbmImage->image[y][x] = 1;
                //line from center to lower left
                pbmImage->image[rows-y][x] = 1;

                //line from center to top left
                pbmImage->image[y][startcol-(x-startcol)] = 1;
                //linfe from center to lower left
                pbmImage->image[rows-y][startcol-(x-startcol)] = 1;


                D = D + (2*drow-2*dcol);

            }
            else
            {
                pbmImage->image[y][x] = 1;
                pbmImage->image[rows-y][x] = 1;
                //line from center to top left
                pbmImage->image[y][startcol-(x-startcol)] = 1;
                //linfe from center to lower left
                pbmImage->image[rows-y][startcol-(x-startcol)] = 1;

                D = D + (2*drow);
            }


        }
    }
    //else the rectangle is vertical based
    else
    {
        //starting parameters
        dcol = finishcol-startcol;
        drow = startrow - finishrow;
        int D = 2*dcol - drow;
        int x = startcol;
        int y;

        for(y = startrow; y>=finishrow; y--)
        {

            if(D > 0)
            {
                x++;
                //line from center to top right
                pbmImage->image[y][x] = 1;
                //line from center to lower left
                pbmImage->image[rows-y][x] = 1;

                //line from center to top left
                pbmImage->image[y][startcol-(x-startcol)] = 1;
                //linfe from center to lower left
                pbmImage->image[rows-y][startcol-(x-startcol)] = 1;


                D = D + (2*dcol-2*drow);

            }
            else
            {
                pbmImage->image[y][x] = 1;
                pbmImage->image[rows-y][x] = 1;
                //line from center to top left
                pbmImage->image[y][startcol-(x-startcol)] = 1;
                //linfe from center to lower left
                pbmImage->image[rows-y][startcol-(x-startcol)] = 1;

                D = D + (2*dcol);
            }


        }
    }

    return 0;
}

/*
* draw the faded inner part of the pgm image
* Starts from the center and constructs the 'upper' triangle.
* It then mirrors it to the other triangles in the center and fills in
* the 'gaps' between the two points with the gray value at that location
* This uses the same basic algorithm as in drawLine but with some added features
* for filling in the white rectangle
*/
int drawFadedCenter(struct PGM_Image *pgmImage, int row, int col, int maxGrayValue)
{
    int x;
    int y;
    int dx;
    int dy;
    int startx;
    int finishx;
    int starty;
    int finishy;
    int D;
    int color = 0;
    int acrossx;
    int index;

    //perLine is used as a way to determine how many rows/columns should be the same color if cols, rows > maxGrayValue
    //if cols, rows < maxGrayValue, then perLine is used to determine the gray 'jump' at each line
    int perLine;



    //starting parameters
    startx = col/2;
    finishx = col - col/4;
    starty = row/2;
    finishy = row/4;

    dx = finishx - startx;
    dy = starty - finishy;



    //if square or horizontal rectangle
    if(col >= row)
    {
        D = 2*dy - dx;

        y = starty;

        //determine perLine usage
        if(col/4 >= maxGrayValue)
        {
            //perLine is how many lines in a row have the same color
            perLine = (col/4)/maxGrayValue;
        }
        else
        {
            //perLine is how many color values to 'jump' at each line
            perLine = (int)ceil((double)maxGrayValue/((double)col/(double)4));
        }
        for(x = startx; x<=finishx; x++)
        {

            if(D > 0)
            {
                acrossx = startx-(x-startx);

                if(col/4 >= maxGrayValue)
                {
                    //only increase color if x/4, the inner rectangle height/width is divisible by perLine value
                    if((x/4)%perLine == 0)
                    {
                        color++;
                        printf("%d\n", color);
                    }
                }
                else
                {

                    color = color + perLine;

                }

                //safety check on color value
                if(color >= maxGrayValue)
                {

                    color = maxGrayValue;
                }


                y = y - 1;

                //fill in values across columns
                for(index=acrossx; index<=x; index++)
                {
                    //upper triangle
                    pgmImage->image[y][index] = color;
                    //lower triangle
                    pgmImage->image[row-y][index] = color;
                }

                //right triangle
                for(index=y; index<=(row-y); index++)
                {
                    pgmImage->image[index][x] = color;
                }

                //left triangle
                for(index=y; index<=(row-y); index++)
                {
                    pgmImage->image[index][acrossx] = color;
                }


                D = D + (2*dy-2*dx);
                //printf("x = %d, y = %d, ax=%d\n", x, y, acrossx);
            }
            //D <=0
            else
            {
                if(col/4 >= maxGrayValue)
                {

                    if((x/4)%perLine == 0)
                    {
                        color++;
                    }
                }
                else
                {

                    color = color + perLine;

                }


                if(color >= maxGrayValue)
                {

                    color = maxGrayValue;
                }

                pgmImage->image[y][x] = color;
                acrossx = startx - (x-startx);
                pgmImage->image[y][acrossx] = color;

                //right triangle
                for(index=y; index<=(row-y); index++)
                {
                    pgmImage->image[index][x] = color;
                }
                //left triangle
                for(index=y; index<=(row-y); index++)
                {
                    pgmImage->image[index][acrossx] = color;
                }
                //printf("x = %d, y = %d, D = %d\n", x, y, D);
                D = D + (2*dy);
            }


        }
    }

    //if long vertical rectangle
    // Same algorithm as above only row and column values swapped
    else
    {
        D = 2*dx - dy;
        if(row/4 >= maxGrayValue)
        {
            perLine = (row/4)/maxGrayValue;
        }
        else
        {
            perLine = (int)ceil((double)maxGrayValue/((double)row/(double)4));
        }

        x = startx;
        for(y = starty; y>=finishy; y--)
        {

            if(D > 0)
            {
                acrossx = startx-(x-startx);

                if(row/4 >= maxGrayValue)
                {

                    if((y/4)%perLine == 0)
                    {
                        color++;
                    }
                }
                else
                {

                    color = color + perLine;

                }


                if(color >= maxGrayValue)
                {

                    color = maxGrayValue;
                }




                for(index=acrossx; index<=x; index++)
                {
                    pgmImage->image[y][index] = color;
                    //lower triangle
                    pgmImage->image[row-y][index] = color;
                }

                //right triangle
                for(index=y; index<=(row-y); index++)
                {
                    pgmImage->image[index][x] = color;
                }

                //left triangle
                for(index=y; index<=(row-y); index++)
                {
                    pgmImage->image[index][acrossx] = color;
                }

                x++;
                D = D + (2*dx-2*dy);
                //printf("x = %d, y = %d, ax=%d\n", x, y, acrossx);
            }
            else
            {
                if(row/4 >= maxGrayValue)
                {

                    if((y/4)%perLine == 0)
                    {
                        color++;
                    }
                }
                else
                {

                    color = color + perLine;

                }


                if(color >= maxGrayValue)
                {

                    color = maxGrayValue;
                }
                pgmImage->image[y][x] = color;
                acrossx = startx - (x-startx);
                pgmImage->image[y][acrossx] = color;

                for(index=acrossx; index<=x; index++)
                {
                    pgmImage->image[y][index] = color;
                    //lower triangle
                    pgmImage->image[row-y][index] = color;
                }


                D = D + (2*dx);
            }


        }
    }




    return 0;
}

int pgm(int rows, int cols, int formatCode, int maxGrayValue, char *imageName)
{
    int colIndex;
    int rowIndex;

    struct PGM_Image pgmImage;

    create_PGM_Image(&pgmImage, cols, rows, maxGrayValue);

    //first for loop, makes everything black
    for(colIndex=0; colIndex<cols; colIndex++)
    {
        for(rowIndex=0; rowIndex<rows; rowIndex++)
        {

            pgmImage.image[rowIndex][colIndex] = 0;
        }
    }



    //draw white middle rectangle
    for(colIndex=cols/4; colIndex<cols - cols/4; colIndex++)
    {
        for(rowIndex=rows/4; rowIndex<rows - rows/4; rowIndex++)
        {

            pgmImage.image[rowIndex][colIndex] = maxGrayValue;
        }
    }

    drawFadedCenter(&pgmImage, rows, cols, maxGrayValue);
    save_PGM_Image(&pgmImage, imageName, formatCode);
    free_PGM_Image(&pgmImage);
    return 0;


}

/*
* creates a ppm image as shown in the assignment.
* uses same perLine algorithm to fill color as in pgm
*/
int ppm(int rows, int cols, int formatCode, char *imageName, int maxColorValue)
{


    int row;
    int col;
    int red;
    int green;
    int blue;
    int perLine;

    struct PPM_Image ppmImage;
    create_PPM_Image(&ppmImage, cols, rows, maxColorValue);

    //red square
    if(rows/2 >= maxColorValue)
    {
        perLine = (rows/2)/maxColorValue;
    }
    else
    {
        perLine = (int)ceil((double)maxColorValue/((double)rows/(double)4));
    }

    red = maxColorValue;
    green = 0;
    blue=0;
    for(row=0; row<rows/2; row++)
    {

        for(col=0; col<cols/3; col++)
        {
            ppmImage.image[row][col][RED] = red;
            ppmImage.image[row][col][GREEN] = green;
            ppmImage.image[row][col][BLUE] = blue;
        }

        if(rows/2 >= maxColorValue)
        {

            if((row/4)%perLine == 0)
            {
                blue++;
                green++;
            }
        }
        else
        {

            blue = blue + perLine;
            green = green+ perLine;

        }


        if(green >= maxColorValue)
        {

            blue = maxColorValue;
            green = maxColorValue;
        }

    }

    //green square

    red = maxColorValue;
    green=maxColorValue;
    blue=maxColorValue;

    if(rows/2 >= maxColorValue)
    {
        perLine = (rows/2)/maxColorValue;
    }
    else
    {
        perLine = (int)ceil((double)maxColorValue/((double)rows/(double)4));
    }


    for(row=0; row<rows/2; row++)
    {

        for(col=cols/3; col<(2*cols)/3; col++)
        {
            ppmImage.image[row][col][RED] = red;
            ppmImage.image[row][col][GREEN] = green;
            ppmImage.image[row][col][BLUE] = blue;
        }

      if(rows/2 >= maxColorValue)
        {

            if((row/4)%perLine == 0)
            {
                blue--;
                red--;
            }
        }
        else
        {

            blue = blue - perLine;
            red = red- perLine;

        }


        if(red < 0)
        {

            blue = 0;
            red = 0;
        }

    }

    //blue Square
    red = 0;
    green=0;
    blue=maxColorValue;

    if(rows/2 >= maxColorValue)
    {
        perLine = (rows/2)/maxColorValue;
    }
    else
    {
        perLine = (int)ceil((double)maxColorValue/((double)rows/(double)4));
    }


    for(row=0; row<rows/2; row++)
    {

        for(col=2*cols/3; col<cols; col++)
        {
            ppmImage.image[row][col][RED] = red;
            ppmImage.image[row][col][GREEN] = green;
            ppmImage.image[row][col][BLUE] = blue;
        }

     if(rows/2 >= maxColorValue)
        {

            if((row/4)%perLine == 0)
            {
                red++;
                green++;
            }
        }
        else
        {

            red = red + perLine;
            green = green+ perLine;

        }


        if(green >= maxColorValue)
        {

            red = maxColorValue;
            green = maxColorValue;
        }

    }

    //lower left gray
    red = 0;
    green=0;
    blue=0;

    if(rows/2 >= maxColorValue)
    {
        perLine = (rows/2)/maxColorValue;
    }
    else
    {
        perLine = (int)ceil((double)maxColorValue/((double)rows/(double)4));
    }


    for(row=rows/2; row<rows; row++)
    {

        for(col=0; col<cols/2; col++)
        {
            ppmImage.image[row][col][RED] = red;
            ppmImage.image[row][col][GREEN] = green;
            ppmImage.image[row][col][BLUE] = blue;
        }

    if(rows/2 >= maxColorValue)
        {

            if((row/4)%perLine == 0)
            {
                red++;
                blue++;
                green++;
            }
        }
        else
        {
            red = red + perLine;
            blue = blue + perLine;
            green = green+ perLine;

        }


        if(green >= maxColorValue)
        {
            red = maxColorValue;
            blue = maxColorValue;
            green = maxColorValue;
        }

    }

    //lower right gray
    red = maxColorValue;
    green=maxColorValue;
    blue=maxColorValue;

    if(rows/2 >= maxColorValue)
    {
        perLine = (rows/2)/maxColorValue;
    }
    else
    {
        perLine = (int)ceil((double)maxColorValue/((double)rows/(double)4));
    }


    for(row=rows/2; row<rows; row++)
    {

        for(col=cols/2; col<cols; col++)
        {
            ppmImage.image[row][col][RED] = red;
            ppmImage.image[row][col][GREEN] = green;
            ppmImage.image[row][col][BLUE] = blue;
        }
       if(rows/2 >= maxColorValue)
        {

            if((row/4)%perLine == 0)
            {
                red--;
                blue--;
                green--;
            }
        }
        else
        {
            red = red - perLine;
            blue = blue - perLine;
            green = green - perLine;

        }


        if(green < 0)
        {
            red = 0;
            blue = 0;
            green = 0;
        }
    }
    struct PGM_Image pgmImage1;
    create_PGM_Image(&pgmImage1, cols, rows, 255);
    copy_PPM_to_PGM(&ppmImage,&pgmImage1, RED);
    save_PGM_Image(&pgmImage1, "pgmImageRed.pgm", 0);
    free_PGM_Image(&pgmImage1);

    struct PGM_Image pgmImage2;
    create_PGM_Image(&pgmImage2, cols, rows, 255);
    copy_PPM_to_PGM(&ppmImage,&pgmImage2, GREEN);
    save_PGM_Image(&pgmImage2, "pgmImageGreen.pgm", 0);
    free_PGM_Image(&pgmImage2);

    struct PGM_Image pgmImage3;
    create_PGM_Image(&pgmImage3, cols, rows, 255);
    copy_PPM_to_PGM(&ppmImage,&pgmImage3, BLUE);
    save_PGM_Image(&pgmImage3, "pgmImageBlue.pgm", 0);
    free_PGM_Image(&pgmImage3);



    save_PPM_Image(&ppmImage, imageName, formatCode);
    free_PPM_Image(&ppmImage);

    return 0;


}

//plots a pbm image. First quarter of rows is black, white in the center of picture measuring
//half height and half width of picture. Bottom quarter of rows is black. Uses two drawline calls
// to draw an x spanning the white center
int pbm(int rows, int cols, int formatCode, char *imageName)
{
    int colIndex;
    int rowIndex;

    struct PBM_Image pbmImage;

    create_PBM_Image(&pbmImage, cols, rows);

    //first for loop, makes everything black
    for(colIndex=0; colIndex<cols; colIndex++)
    {
        for(rowIndex=0; rowIndex<rows; rowIndex++)
        {

            pbmImage.image[rowIndex][colIndex] = 1;
        }
    }



    //draw white middle rectangle
    for(colIndex=cols/4; colIndex<cols - cols/4; colIndex++)
    {
        for(rowIndex=rows/4; rowIndex<rows - rows/4; rowIndex++)
        {

            pbmImage.image[rowIndex][colIndex] = 0;
        }
    }

    //draw lines
    drawLine(&pbmImage, cols, rows);
    //drawLine(&pbmImage, cols/4, cols-cols/4, rows-rows/4, rows/4);

    save_PBM_Image(&pbmImage, imageName, formatCode);
    free_PBM_Image(&pbmImage);


    return 0;
}


int main(int argc, char *argv[])
{
    /*
    * Takes in 5 inputs
    * type code: 1 - pbm, 2 - pgm, 3 - ppm
    * if pbm || pgm
    * width - div by 4
    * else width - div  by 6
    * height - div by 4
    * image output name
    * image format code: 0 - ascii, 1 - raw
    */

    int pictureType;
    int width;
    int height;
    char *imageName;
    int formatCode;


    if(argc!=6)
    {
        perror("Invalid command line inputs");
        exit(1);
    }



    if(atoi(argv[1]) !=PBM && atoi(argv[1]) !=PGM && atoi(argv[1]) != PPM)
    {
        perror("Invalid picture type");
        return 1;
    }
    else
    {
        pictureType = atoi(argv[1]);
    }



    if(pictureType ==PBM || pictureType == PGM)
    {
        if(atoi(argv[2]) % 4 != 0)
        {
            perror("Width must be divisible by 4");
            return 1;
        }
        else
        {
            width = atoi(argv[2]);
            height = atoi(argv[3]);
        }
    }
    else
    {
        if(atoi(argv[2]) % 6 !=0)
        {
            perror("Width my be divisible by 6");
            return 1;
        }
        else
        {
            width = atoi(argv[2]);
            height = atoi(argv[3]);
        }
    }


    imageName = strdup(argv[4]);

    if(atoi(argv[5])!=0 && atoi(argv[5])!=1)
    {
        perror("format must be 0 or 1");
        return 1;
    }
    else
    {
        formatCode = atoi(argv[5]);
    }

    if(pictureType == PBM)
    {
        pbm(height, width, formatCode, imageName);
    }
    else if(pictureType == PGM)
    {
        pgm(height, width, formatCode, 255, imageName);
    }
    else
    {
        ppm(height, width, formatCode, imageName, 255);
    }

    return 0;
}
