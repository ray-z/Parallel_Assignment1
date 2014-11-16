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
int averaging(int *arr, int *temp, int row, int col, int precision);

int main(int argc, char *argv[])
{
    int arrLen, precision, numThreads;
    int *randArr, *tempArr;

    /* get command-line arguments */
    arrLen = (argv[1]) ? strtol(argv[1], NULL, 10) : 0;
    precision = (argv[2]) ? strtol(argv[2], NULL, 10) : 0;
    numThreads = (argv[3]) ? strtol(argv[3], NULL, 10) : 0;

    /* init a array */
    printf("Random generating a 2D array with dimension = %d:\n", arrLen);
    randArr = (int *)malloc(arrLen*arrLen*sizeof(int)); 
    tempArr = (int *)malloc(arrLen*arrLen*sizeof(int)); 
    for(int i = 0; i < arrLen*arrLen; i++)
    {
        randArr[i] = rand() % MAX_RAND;
    }
    copyArr(tempArr, randArr, arrLen*arrLen);

    //copyArr(tempArr, randArr, arrLen*arrLen);
    print2DArr(randArr, arrLen);
    
    /* do averaging */
    printf("Averaging...\n");
    printf("Result:\n");
    while(averaging(randArr, tempArr, arrLen, arrLen, precision))
    {
        copyArr(tempArr, randArr, arrLen*arrLen);
    }
    print2DArr(randArr, arrLen);



    free(randArr);
    free(tempArr);
    return 0;
}

/*
 * averaging: replacing a value with the average of its four neighbours
 * return: number of items whose value >= max after averaging
 */
int averaging(int *arr, int *temp, int row, int col, int max)
{
    int counter = 0;
    for(int r = 1; r < row-1; r++)
    {
        for(int c = 1; c < col-1; c++)
        {
            int avg = (temp[r*col + c - 1] + temp[r*col + c + 1] + 
                    temp[(r-1)*col + c] + temp[(r+1)*col +c]) / 4; 
            if((arr[r*col + c] = avg) >= max)  counter++;
            /*printf("result=%d, precision=%d, counter=%d\n", 
                    result, precision, counter);*/
        }
    }
    return counter;
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



