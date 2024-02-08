/**
 * This file is the .c file for Programming Assignment #3: Pthreads
 * This was written by Michael Piscione
*/

// NOTE THAT THIS FILE CONTAINS A MAIN FUNCTION,
// NO SPECIFICATION NOT TO INCLUDE MAIN.
// ALSO NOTE THAT THIS WORK IS HEAVILY INFLUENCED FROM CODE IN pthreads-example-simpler.c by Jason Hibbeler

// Include dependencies
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Define global constants
#define NUMVALS (1024 * 1024)
#define NUMTHREADS 4

// Declare global struct
typedef struct {
    int startIndex;  // Start index at which to start looking
    int endIndex;  // End index at which to stop looking
    float threshold;  // Threshold value
    int count;  // Result: #values greater than threshold
} CountInfo;

// Declare global buffer
float g_vals[NUMVALS];

// Define prand function
// Pseudo-random number generator given by Jason Hibbeler
int prand() {
  static int p = 1;
  const int a = 105491;
  const int b = 733;
  const int c = 6619;
  const int base = 100;
  int val;
  p = p*a + b;
  p = p % c;
  val = p % base;
  return val;
}

/**
 * doCount counts the number of values in a specified region in a global array that are greater than a 
 * threshold value. Intended to be used with threads.
 * Parameters: void *param, the data to be loaded in for a specific thread.
 * Return: void * the number of values greater than a threshold in a specified region.
*/
void *doCount(void *param) {
    // Declare local variables
    CountInfo *data;
    int i, count;

    // Cast param to CountInfo pointer and store it in data
    data = (CountInfo *) param;

    // Set threshold to 0.5
    data->threshold = 0.5;
    
    // Print helpful text about this thread
    printf("I am doCount; will count for the range %d to %d\n",
            data->startIndex, data->endIndex);

    // Count number of values greater than the threshold
    for (i = data->startIndex; i <= data->endIndex; ++i) {
        if (g_vals[i] > data->threshold)
        count += 1;
    }

    // Store value of count in data's count
    data->count = count;

    // Print helpful information about count of this thread
    printf("my count is %d\n", data->count);

    // Exit the thread
    pthread_exit(NULL);
}

int main() {
    // Make array of NUMTHREADS structs
    CountInfo tdata[NUMTHREADS];

    // Make array of NUMTHREADS pthread_t variables to hold each thread id that pthread_create gives
    pthread_t tids[NUMTHREADS];

    // Initialize local variables
    int count;
    int i, pos, chunkSize;

    // Initialize global buffer
    int i1, i2;
    float f1, f2;
    for (i = 0; i < NUMVALS; ++i) {
        i1 = prand(100);
        i2 = prand(100);
        f1 = i1 / 100.0;
        f2 = i2 / 100.0;
        g_vals[i] = f1 / (1.0 + f2);
    }

    // Set up bounds for the threads
    chunkSize = NUMVALS / NUMTHREADS;
    pos = 0;
    for (i = 0; i < NUMTHREADS; ++i) {
        tdata[i].startIndex = pos;
        tdata[i].endIndex = tdata[i].startIndex + chunkSize;
        pos = pos + chunkSize + 1;
    }

    // Adjust last region if necessary (NUMVALS unenven multiple of NUMTHREADS)
    if (tdata[NUMTHREADS - 1].endIndex < NUMVALS - 1)
        tdata[NUMTHREADS - 1].endIndex = NUMVALS - 1;
    else if (tdata[NUMTHREADS-1].endIndex > NUMVALS - 1)
        tdata[NUMTHREADS - 1].endIndex = NUMVALS - 1;

    // Create child threads
    for (i = 0; i < NUMTHREADS; ++i)
        pthread_create(&tids[i], NULL, doCount, &tdata[i]);

    // Wait for the child threads to terminate
    for (i = 0; i < NUMTHREADS; ++i)
        pthread_join(tids[i], NULL);

    // Gather data from the individual results
    count = tdata[0].count;
    for (i = 1; i < NUMTHREADS; ++i) {
        count += tdata[i].count;
    }

    // Print the overall count
    printf("overall count is %d\n", count);

    // Return 0 upon successful execution
    return 0;
    
}
