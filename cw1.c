/*
 * CM30225 Parallel Computing
 * Coursework 1
 * Author: Lei Zeng
 * Date: 07 Nov 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define RMAX 100.00   /* maximum random number */

int isEnd = 0;
int waitCount = 0;
int roundCount = 0;
int arrLen, numThreads;
double precision;
double *randArr;
pthread_mutex_t mutex;
pthread_cond_t cv;
pthread_barrier_t barr;

double fRand(double max);
void print2DArr(double *arr, int len);
void copyArr(double *dst, double *src, int len);
void averaging(int *inc);

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
    precision = (argv[2]) ? atof(argv[2]) : RMAX;
    printf("precision: %f\n", precision);
    numThreads = (argv[3]) ? strtol(argv[3], NULL, 10) : 1;


    /* init array */
    randArr = (double *)malloc(arrLen*arrLen*sizeof(double));
    printf("Random generating a 2D array with dimension = %d:\n", arrLen);
    for(int i = 0; i < arrLen*arrLen; i++)
    {
        randArr[i] = fRand(RMAX);
    }
    print2DArr(randArr, arrLen);

    /* init mutex */
    if(pthread_mutex_init(&mutex, NULL))
    {
        printf("Unable to initialize a mutex\n");
        return -1;
    }
   
    /* init condition variable */
    if(pthread_cond_init (&cv, NULL))
    {
        printf("Unable to initialize a condition variable\n");
        return -1;
    }
     

    /* init barrier */
    if(pthread_barrier_init(&barr, NULL, numThreads))
    {
        printf("Unable to initialize a barrier.\n");
        return -1;
    }

    /* create threads */
    pthread_t threads[numThreads];
    for(int i = 0; i < numThreads; ++i)
    {
        int *inc = malloc(sizeof(i));
        *inc = i;
        if(pthread_create(&threads[i], NULL, (void*(*)(void*))averaging, inc))
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


    printf("Result:\n");
    print2DArr(randArr, arrLen);

    /* clean up */
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cv);
    pthread_barrier_destroy(&barr);
    pthread_exit(NULL);
    free(randArr);
    return 0;
}

/*
 * averaging: replacing a value with the average of its four neighbours
 */
void averaging(int *inc)
{
    int thrNum = *inc;
    double temp[arrLen*arrLen];
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

    //printf("Thread %d will average row %d to %d\n", thrNum, row_s, row_e);
    copyArr(temp, randArr, arrLen*arrLen);

    /* 
     * thread 0 will set isEnd to 1 at begining, broadcast once done
     * the rest threads will wait for the signal
     */
    /* start loop to average */
    while(!isEnd)
    {
        pthread_mutex_lock(&mutex);
        waitCount++;
        //printf("Thread %d: waitCount = %d\n", thrNum, waitCount);
        if(waitCount == numThreads)
        {
            isEnd = 1;
            waitCount = 0;
            roundCount++;
            pthread_cond_broadcast(&cv);
            printf("Thread %d is broadcasting signals\n", thrNum);
            printf("Round: %d starts now\n", roundCount);
        }
        else
        {
            printf("Thread %d is waiting for signal\n", thrNum);
            pthread_cond_wait(&cv, &mutex);
        }
        pthread_mutex_unlock(&mutex);


        for(int r = row_s; r <= row_e; r++)
        {
            for(int c = 1; c < col-1; c++)
            {
                double avg = (temp[r*col + c - 1] + temp[r*col + c + 1] + 
                        temp[(r-1)*col + c] + temp[(r+1)*col +c]) / 4; 
                if(fabs(randArr[r*col + c] - avg) >= precision)  isEnd = 0;
                randArr[r*col + c] = avg;
            }
        }
    
        //print2DArr(randArr,col);
        /* Synchronization point */
        printf("Thread %d is done...\n", thrNum);
        int rc = pthread_barrier_wait(&barr);
        if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
        {
            printf("Could not wait on barrier.\n");
            exit(-1);
        }
    
        copyArr(temp, randArr, arrLen*arrLen);
    }
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
            //randArr[r][c] = rand() % RMAX;
            //printf("%3d", randArr[r][c]);
            printf("%3f\t", arr[r*len + c]);
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

