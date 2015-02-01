#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "libpnm.h"

#define PBM 1
#define PGM 2
#define PPM 3


int drawLine(struct PBM_Image *pbmImage, int x0, int x1, int y0, int y1)
{

    //Bresenhams Line Algorithm
    int dx;
    int dy;
    int startx;
    int starty;
    int finishx;
    int finishy;



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

    double D = 2*dy - dx;




    if(dx >= dy)
    {
        //normal left to right, x,y increasing, square, or rectangle,dx is larger than dy
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
    else
    {
        //normal left to right,  x,y increasing, rectangle, dy is larger than dx
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



int pbm(int rows, int cols, int format, char *imageName)
{
    int colIndex;
    int rowIndex;

    struct PBM_Image pbmImage;

    create_PBM_Image(&pbmImage, cols, rows);

    //first for loop
    for(colIndex=0; colIndex<cols; colIndex++)
    {
        for(rowIndex=0; rowIndex<rows; rowIndex++)
        {

            pbmImage.image[rowIndex][colIndex] = 1;
        }
    }



    //middle
    for(colIndex=cols/4; colIndex<cols - cols/4; colIndex++)
    {
        for(rowIndex=rows/4; rowIndex<rows - rows/4; rowIndex++)
        {

            pbmImage.image[rowIndex][colIndex] = 0;
        }
    }

    //drawLine(&pbmImage, cols/4, (cols-cols/4), rows/4, rows - rows/4);
    drawLine(&pbmImage, cols/4, cols-cols/4, rows-rows/4, rows/4);

    save_PBM_Image(&pbmImage, imageName, false);
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
