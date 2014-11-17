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
int arrLen, precision, numThreads;
int *randArr;
pthread_barrier_t barr;
//pthread_mutex_t mutex;


void print2DArr(int *arr, int len);
void copyArr(int *dst, int *src, int len);
void averaging(int thrNum);
void averaging_noP(int row, int col);

int main(int argc, char *argv[])
{
    /* get command-line arguments 
     *
     * default value:
     * square array length: 10
     * precision: max value of the random range, so the loop will run once only
     * number of threads: 1
     */
    //int arrLen, precision, numThreads;
    arrLen = (argv[1]) ? strtol(argv[1], NULL, 10) : 10;
    precision = (argv[2]) ? strtol(argv[2], NULL, 10) : MAX_RAND;
    numThreads = (argv[3]) ? strtol(argv[3], NULL, 10) : 1;


    /* init array */
    randArr = (int *)malloc(arrLen*arrLen*sizeof(int));
    printf("Random generating a 2D array with dimension = %d:\n", arrLen);
    for(int i = 0; i < arrLen*arrLen; i++)
    {
        randArr[i] = rand() % MAX_RAND;
    }
    print2DArr(randArr, arrLen);


    /* init barrier */
    if(pthread_barrier_init(&barr, NULL, numThreads))
    {
        printf("Could not create a barrier.\n");
        return -1;
    }

    /* create threads */
    pthread_t threads[numThreads];
    for(int i = 0; i < numThreads; ++i)
    {
        if(pthread_create(&threads[i], NULL, (void*(*)(void*))averaging, &i))
        {
            printf("Could not create thread: %d\n", i);
            return -1;
        }

    }

    for(int i = 0; i< numThreads; ++i)
    {
        if(pthread_join(threads[i], NULL))
        {
            printf("Could not join thread: %d\n", i);
            return -1;
        }
    }






    
//    /* do averaging */
//    printf("Averaging...\n");
//    printf("Result:\n");
//    while(!isEnd)
//    {
//        averaging_noP(arrLen, arrLen);
//        
//    }
//    print2DArr(randArr, arrLen);


    free(randArr);
    return 0;
}

/*
 * averaging: replacing a value with the average of its four neighbours
 */
void averaging(int thrNum)
{
    int temp[arrLen*arrLen];
    int row_s, row_e;
    int col = arrLen;
    int n = (arrLen-2)/numThreads;
    if(thrNum == numThreads - 1)
    {
        row_s = thrNum * n + 1;
        row_e = arrLen - 2;
    }
    else
    {
        row_s = thrNum * n + 1;
        row_e = row_s + n - 1;
    }

    copyArr(temp, randArr, arrLen*arrLen);

    /* start loop to averaging */
    while(!isEnd)
    {
        for(int r = row_s; r <= row_e; r++)
        {
            for(int c = 1; c < col-1; c++)
            {
                int avg = (temp[r*col + c - 1] + temp[r*col + c + 1] + 
                        temp[(r-1)*col + c] + temp[(r+1)*col +c]) / 4; 
                if((randArr[r*col + c] = avg) >= precision)  isEnd = 0;
            }
        }
    
        /* Synchronization point */
        printf("Thread %d is waiting...", thrNum);
        int rc = pthread_barrier_wait(&barr);
        if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
        {
            printf("Could not wait on barrier.\n");
            exit(-1);
        }
    
        copyArr(temp, randArr, arrLen*arrLen);

        
    }

    
    





    /*
    isEnd = 1;

    */
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

