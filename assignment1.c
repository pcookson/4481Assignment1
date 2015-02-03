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
int drawLine(struct PBM_Image *pbmImage, int x0, int x1, int y0, int y1)
{


    int dx;
    int dy;
    int startx;
    int starty;
    int finishx;
    int finishy;


    //if starting x smaller than finish x, reverse start and end so for loop can always be increasing.
    if(x1>=x0)
    {
        startx = x0;
        finishx = x1;
        dx = x1-x0;
    }
    else
    {
        startx = x1;
        finishx = x0;
        dx = x0-x1;
    }

    //if starting y is smaller than finish y, reverse start and end
    if(y1>y0)
    {
        starty = y0;
        finishy = y1;
        dy = y1-y0;
    }
    else
    {
        starty = y1;
        finishy = y0;
        dy = y0-y1;
    }

    int D = 2*dy - dx;



    //Use this variation if picture is a rectangle with x greater than or equal to y
    if(dx >= dy)
    {

        int y = y0;
        int x;
        for(x = startx; x<=finishx; x++)
        {
            if(D > 0)
            {
                if(y0<y1)
                {
                    y = y +1;
                }
                else
                {
                    y = y-1;
                }
                pbmImage->image[y][x] = 1;
                D = D + (2*dy-2*dx);
                printf("x = %d, y = %d\n", x, y);
            }
            else
            {
                pbmImage->image[y][x] = 1;
                printf("x = %d, y = %d\n", x, y);
                D = D + (2*dy);
            }


        }
    }
    //if a rectangle with y greater than x, and the line is drawn in direction of y decreasing
    else if(dx <dy && y0>y1)
    {

        int y;
        int x=x0;
        for(y=y0; y>y1; y--)
        {
            if(D > 0)
            {
                if(x0<x1)
                {
                    x = x +1;
                }
                else
                {
                    x = x-1;
                }
                pbmImage->image[y][x] = 1;
                D = D + (2*dx-2*dy);
                printf("x = %d, y = %d\n", x, y);
            }
            else
            {
                pbmImage->image[y][x] = 1;
                printf("x = %d, y = %d\n", x, y);
                D = D + (2*dx);
            }


        }

    }
    //if picture is rectangle with y greater than x. Line is drawn with x and y increasing
    else
    {

        int x = x0;
        int y;
        for(y = starty; y<=finishy; y++)
        {
            if(D > 0)
            {
                if(x0<x1)
                {
                    x = x +1;
                }
                else
                {
                    x = x-1;
                }
                pbmImage->image[y][x] = 1;
                D = D + (2*dx-2*dy);
                printf("x = %d, y = %d\n", x, y);
            }
            else
            {
                pbmImage->image[y][x] = 1;
                printf("x = %d, y = %d\n", x, y);
                D = D + (2*dx);
            }


        }

    }

    return 0;
}

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


    //draw upper triangle

    startx = col/2;
    finishx = col - col/4;
    starty = row/2;
    finishy = row/4;

    dx = finishx - startx;
    dy = starty - finishy;

    D = 2*dy - dx;

    //if square or horizontal rectangle
    if(col >= row)
    {


        y = starty;
        for(x = startx; x<=finishx; x++)
        {

            if(D > 0)
            {
                acrossx = startx-(x-startx);
                if(color+(row/(row/6)) >= 255)
                {
                    color = 255;
                }
                else
                {
                    color = color+(row/(row/6)) ;// (int)(255/(row/4));


                }
                y = y - 1;
                for(index=acrossx; index<=x; index++)
                {
                    pgmImage->image[y][index] = color;
                    //lower triangle
                    pgmImage->image[row-y][index] = color;
                }

                //right triangle
                for(index=y; index<=(row-y); index++){
                    pgmImage->image[index][x] = color;
                }

                //left triangle
                for(index=y;index<=(row-y);index++){
                    pgmImage->image[index][acrossx] = color;
                }


                D = D + (2*dy-2*dx);
                printf("x = %d, y = %d, ax=%d\n", x, y, acrossx);
            }
            else
            {
                pgmImage->image[y][x] = color;
                acrossx = startx - (x-startx);
                pgmImage->image[y][acrossx] = color;
                //right triangle
                for(index=y; index<=(row-y); index++){
                    pgmImage->image[index][x] = color;
                }
                //left triangle
                for(index=y;index<=(row-y);index++){
                    pgmImage->image[index][acrossx] = color;
                }
                printf("x = %d, y = %d, D = %d\n", x, y, D);
                D = D + (2*dy);
            }


        }
    }
//    else if(col >row)
//    printf("In long rectangle\n");
//    {
//        x = startx;
//        for(y = starty; y>=finishy; y--)
//        {
//            if(D>0)
//            {
//                acrossx = startx - (x - startx);
//                color = color + (int)(255/(row/4));
//                x++;
//                for(index=acrossx; index<=x;index++){
//                    pgmImage->image[y][index] = color;
//                }
//
//                D = D +(2*dx - 2*dy);
//            }else{
//                pgmImage->image[y][x] = color;
//                acrossx = startx - (x-startx);
//                for(index=acrossx; index<=x;index++){
//                    pgmImage->image[y][index] = color;
//                }
//                //pgmImage->image[y][acrossx] = color;
//                printf("x = %d, y = %d, D = %d\n", x, y, D);
//                D = D + (2*dx);
//            }
//        }
//    }


    //draw right triangle




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

int ppm(int rows, int cols, int formatCode, char *imageName, int maxColorValue)
{

    //redSquare
    int row;
    int col;
    int red;
    int green;
    int blue;

    struct PPM_Image ppmImage;
    create_PPM_Image(&ppmImage, cols, rows, maxColorValue);

    //red square
    red =maxColorValue;
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
        if(rows>=255*2)
        {
            if(row %((rows/2)/255)==0)
            {
                if(blue < 255 && green < 255)
                {
                    blue++;
                    green++;
                }
            }
        }
        else
        {
            if(blue+2<255 && green+2<255)
            {
                printf("%d\n", blue);
                blue = blue +2*255/((rows/2));
                green = green + 2*255/((rows/2));
            }

        }
    }

    //green square

    red = maxColorValue;
    green=maxColorValue;
    blue=maxColorValue;
    for(row=0; row<rows/2; row++)
    {

        for(col=cols/3; col<(2*cols)/3; col++)
        {
            ppmImage.image[row][col][RED] = red;
            ppmImage.image[row][col][GREEN] = green;
            ppmImage.image[row][col][BLUE] = blue;
        }
        if(rows>=255*2)
        {
            if(row %((rows/2)/255)==0)
            {
                if(blue > 0 && red > 0)
                {
                    blue--;
                    red--;
                }
            }
        }
        else
        {
            if(blue-2>0 && red-2>0)
            {
                printf("%d\n", blue);
                blue = blue - 2*255/((rows/2));
                red = red - 2*255/((rows/2));
            }

        }
    }

    //blue Square
    red = 0;
    green=0;
    blue=maxColorValue;
    for(row=0; row<rows/2; row++)
    {

        for(col=2*cols/3; col<cols; col++)
        {
            ppmImage.image[row][col][RED] = red;
            ppmImage.image[row][col][GREEN] = green;
            ppmImage.image[row][col][BLUE] = blue;
        }
        if(rows>=255*2)
        {
            if(row %((rows/2)/255)==0)
            {
                if(red < 255 && green < 255)
                {
                    red++;
                    green++;
                }
            }
        }
        else
        {
            if(red+2<255 && green+2<255)
            {
                printf("%d\n", blue);
                red = red +2*255/((rows/2));
                green = green + 2*255/((rows/2));
            }

        }
    }

    //lower left gray
    red = 0;
    green=0;
    blue=0;
    for(row=rows/2; row<rows; row++)
    {

        for(col=0; col<cols/2; col++)
        {
            ppmImage.image[row][col][RED] = red;
            ppmImage.image[row][col][GREEN] = green;
            ppmImage.image[row][col][BLUE] = blue;
        }

        if(rows>=255*2)
        {
            if(row %((rows/2)/255)==0)
            {
                if(red < 255 && green < 255)
                {
                    red++;
                    green++;
                    blue++;
                }
            }
        }
        else
        {
            if(red+2<255 && green+2<255)
            {
                printf("%d\n", blue);
                red = red +2*255/((rows/2));
                green = green + 2*255/((rows/2));
                blue = blue + 2*255/((rows/2));
            }

        }
    }

    //lower right gray
    red = maxColorValue;
    green=maxColorValue;
    blue=maxColorValue;
    for(row=rows/2; row<rows; row++)
    {

        for(col=cols/2; col<cols; col++)
        {
            ppmImage.image[row][col][RED] = red;
            ppmImage.image[row][col][GREEN] = green;
            ppmImage.image[row][col][BLUE] = blue;
        }
        if(rows>=255*2)
        {
            if(row %((rows/2)/255)==0)
            {
                if(red > 0 && green >0)
                {
                    red--;
                    green--;
                    blue--;
                }
            }
        }
        else
        {
            if(red-2>0 && green-2>0)
            {
                printf("%d\n", blue);
                red = red -2*255/((rows/2));
                green = green - 2*255/((rows/2));
                blue = blue - 2*255/((rows/2));
            }

        }
    }


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
    drawLine(&pbmImage, cols/4, (cols-cols/4), rows/4, rows - rows/4);
    drawLine(&pbmImage, cols/4, cols-cols/4, rows-rows/4, rows/4);

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
//
//    int row =255;
//    int column = 255;
//    int indexRow;
//    int indexCol;
//    struct PBM_Image pic1;
//
//    create_PBM_Image(&pic1, row, column);
//
//    for(indexRow=0; indexRow<row; indexRow++)
//    {
//        for(indexCol=0; indexCol<column; indexCol++)
//        {
//            if(indexRow % 10 == 0)
//            {
//                pic1.image[indexRow][indexCol] = 0;
//            }
//            else
//            {
//                pic1.image[indexRow][indexCol] = 1;
//            }
//        }
//    }
//
//    save_PBM_Image(&pic1, "testpbm.pbm", false);
//    free_PBM_Image(&pic1);
    return 0;

}
