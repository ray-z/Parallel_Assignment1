/*
 * CM30225 Parallel Computing
 * Coursework 1
 * Author: Lei Zeng
 * Date: 07 Nov 2014
 */

/* This version does not contain any parallel */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define RMAX 100.00   /* maximum random number */

double fRand(double max);
void print2DArr(double *arr, int len);
void copyArr(double *dst, double *src, int len);
double averaging(double *arr, double *temp, int row, int col, double precision);

int main(int argc, char *argv[])
{
    /* get command-line arguments 
     *
     * default value:
     * square array length: 10
     * precision: max value of the random range, so the loop will run once only
     * number of threads: 1
     */
    int arrLen, numThreads;
    double precision;
    arrLen = (argv[1]) ? strtol(argv[1], NULL, 10) : 10;
    precision = (argv[2]) ? atof(argv[2]) : RMAX;
    numThreads = (argv[3]) ? strtol(argv[3], NULL, 10) : 1;


    /* init array */
    double randArr[arrLen*arrLen];
    double tempArr[arrLen*arrLen];
    printf("Random generating a 2D array with dimension = %d:\n", arrLen);
    for(int i = 0; i < arrLen*arrLen; i++)
    {
        randArr[i] = fRand(RMAX);
    }
    copyArr(tempArr, randArr, arrLen*arrLen);
    print2DArr(randArr, arrLen);


    /* create threads */
    //pthread_t threads[numThreads];

    //for(;;)
    //{
        
   // }





    
    /* do averaging */
    printf("Averaging...\n");
    printf("Result:\n");
    while(!averaging(randArr, tempArr, arrLen, arrLen, precision))
    {
        copyArr(tempArr, randArr, arrLen*arrLen);
    }
    print2DArr(randArr, arrLen);



    return 0;
}

/*
 * averaging: replacing a value with the average of its four neighbours
 * return: number of items whose value >= max after averaging
 */
double averaging(double *arr, double *temp, int row, int col, double max)
{
    //int counter = 0;
    int isValid = 1;
    for(int r = 1; r < row-1; r++)
    {
        for(int c = 1; c < col-1; c++)
        {
            double avg = (temp[r*col + c - 1] + temp[r*col + c + 1] + 
                    temp[(r-1)*col + c] + temp[(r+1)*col +c]) / 4; 
            if(fabs(arr[r*col + c] - avg) >= max)  isValid = 0;
            arr[r*col + c] = avg;
        }
    }
    return isValid;
}

/*
 * copyArr: copy array from source to destination 
 */
void copyArr(double *dst, double *src, int len)
{
    while(len--)
    {
        *dst++ = *src++;
    }
}

/*
 * print2DArr: print a readable 2D array
 */
void print2DArr(double *arr, int len)
{
    for(int r = 0; r < len; r++)
    {
        for(int c = 0; c < len; c++)
        {
            //randArr[r][c] = rand() % MAX_RAND;
            //printf("%3d", randArr[r][c]);
            printf("%f\t", arr[r*len + c]);
        }
        printf("\n");
    }

}

/*
 * fRand: return random double with 2 decimal places
 */
double fRand(double max)
{
    double f = (double)rand() / RAND_MAX;
    return f * max;
}


