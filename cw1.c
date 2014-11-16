/*
 * CM30225 Parallel Computing
 * Coursework 1
 * Author: Lei Zeng
 * Date: 07 Nov 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_RAND 100   /* maximum random number */

int isEnd = 0;
pthread_barrier_t barrier;

void print2DArr(int *arr, int len);
void copyArr(int *dst, int *src, int len);
void averaging(int *arr, int row, int col, int precision);

int main(int argc, char *argv[])
{
    /* get command-line arguments 
     *
     * default value:
     * square array length: 10
     * precision: max value of the random range, so the loop will run once only
     * number of threads: 1
     */
    int arrLen, precision, numThreads;
    arrLen = (argv[1]) ? strtol(argv[1], NULL, 10) : 10;
    precision = (argv[2]) ? strtol(argv[2], NULL, 10) : MAX_RAND;
    numThreads = (argv[3]) ? strtol(argv[3], NULL, 10) : 1;


    /* init array */
    int randArr[arrLen*arrLen];
    //int tempArr[arrLen*arrLen];
    printf("Random generating a 2D array with dimension = %d:\n", arrLen);
    for(int i = 0; i < arrLen*arrLen; i++)
    {
        randArr[i] = rand() % MAX_RAND;
    }
    //copyArr(tempArr, randArr, arrLen*arrLen);
    print2DArr(randArr, arrLen);


    /* create threads */
    pthread_t threads[numThreads];

    //for(;;)
    //{
        
   // }





    
    /* do averaging */
    printf("Averaging...\n");
    printf("Result:\n");
    while(!isEnd)
    {
        averaging(randArr, arrLen, arrLen, precision);
        
    }
    print2DArr(randArr, arrLen);



    return 0;
}

/*
 * averaging: replacing a value with the average of its four neighbours
 * return: number of items whose value >= max after averaging
 */
void averaging(int *arr, int row, int col, int max)
{
    int temp[row*col];
    copyArr(temp, arr, row*col);

    isEnd = 1;

    for(int r = 1; r < row-1; r++)
    {
        for(int c = 1; c < col-1; c++)
        {
            int avg = (temp[r*col + c - 1] + temp[r*col + c + 1] + 
                    temp[(r-1)*col + c] + temp[(r+1)*col +c]) / 4; 
            if((arr[r*col + c] = avg) >= max)  isEnd = 0;
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



