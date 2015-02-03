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

    if(cols>=rows)
    {
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
    else
    {
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



    //if square or horizontal rectangle
    if(col >= row)
    {
        D = 2*dy - dx;

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
                printf("x = %d, y = %d, ax=%d\n", x, y, acrossx);
            }
            else
            {
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
                printf("x = %d, y = %d, D = %d\n", x, y, D);
                D = D + (2*dy);
            }


        }
    }
    else
    {
        D = 2*dx - dy;

        x = startx;
        for(y = starty; y>=finishy; y--)
        {

            if(D > 0)
            {
                acrossx = startx-(x-startx);
                if(color+7 >= 255)
                {
                    color = 255;
                }
                else
                {
                    color = color+7 ;// (int)(255/(row/4));


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
                pgmImage->image[y][x] = color;
                acrossx = startx - (x-startx);
                pgmImage->image[y][acrossx] = color;

                for(index=acrossx; index<=x; index++)
                {
                    pgmImage->image[y][index] = color;
                    //lower triangle
                    pgmImage->image[row-y][index] = color;
                }

//                //right triangle
//                for(index=y; index<=(row-y); index++)
//                {
//                    pgmImage->image[index][x] = color;
//                }
//                //left triangle
//                for(index=y; index<=(row-y); index++)
//                {
//                    pgmImage->image[index][acrossx] = color;
//                }
                //printf("x = %d, y = %d, D = %d\n", x, y, D);
                D = D + (2*dx);
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
