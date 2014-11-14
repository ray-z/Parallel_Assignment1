/*
 * CM30225 Parallel Computing
 * Coursework 1
 * Author: Lei Zeng
 * Date: 07 Nov 2014
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_RAND 100   /* maximum random number */

void print2DArr(int *arr, int len);
void copyArr(int *dst, int *src, int len);
void averaging(int *arr, int *temp, int len);

int main(int argc, char *argv[])
{
    int arrLen, precision, numThreads;
    int *randArr, *tempArr;

    /* get command-line arguments */
    arrLen = (argv[1]) ? strtol(argv[1], NULL, 10) : 0;
    precision = (argv[2]) ? strtol(argv[2], NULL, 10) : 0;
    numThreads = (argv[3]) ? strtol(argv[3], NULL, 10) : 0;;

    /* init a array */
    printf("Random generating a 2D array with dimension = %d:\n", arrLen);
    randArr = (int *)malloc(arrLen*arrLen*sizeof(int)); 
    tempArr = (int *)malloc(arrLen*arrLen*sizeof(int)); 
    for(int i = 0; i < arrLen*arrLen; i++)
    {
        randArr[i] = rand() % MAX_RAND;
    }
    //copyArr(tempArr, randArr, arrLen*arrLen);
    print2DArr(randArr, arrLen);
    
    /* do averaging */
    printf("averaging count: 1\n");
    averaging(randArr, tempArr, arrLen);
    print2DArr(randArr, arrLen);
    printf("averaging count: 2\n");
    averaging(randArr, tempArr, arrLen);
    print2DArr(randArr, arrLen);
    printf("averaging count: 3\n");
    averaging(randArr, tempArr, arrLen);
    print2DArr(randArr, arrLen);
    printf("averaging count: 4\n");
    averaging(randArr, tempArr, arrLen);
    print2DArr(randArr, arrLen);



    free(randArr);
    free(tempArr);
    return 0;
}

/*
 * averaging: replacing a value with the average of its four neighbours
 */
void averaging(int *arr, int *temp, int len)
{
    copyArr(temp, arr, len*len);
    for(int r = 1; r < len-1; r++)
    {
        for(int c = 1; c < len-1; c++)
        {
            arr[r*len + c] = (temp[r*len + c - 1] + temp[r*len + c + 1] + 
                    temp[(r-1)*len + c] + temp[(r+1)*len +c]) / 4;
        }
    }
}

/*
 * copyArr: copy array from source to destination 
 */
void copyArr(int *dst, int *src, int len)
{
    while(len--)
    {
        *dst++ = *src++;
    }
}

/*
 * print2DArr: print a readable 2D array
 */
void print2DArr(int *arr, int len)
{
    for(int r = 0; r < len; r++)
    {
        for(int c = 0; c < len; c++)
        {
            //randArr[r][c] = rand() % MAX_RAND;
            //printf("%3d", randArr[r][c]);
            printf("%3d", arr[r*len + c]);
        }
        printf("\n");
    }

}



