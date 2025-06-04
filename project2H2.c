#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

int *AllocateArray(int N)
{
    srand((unsigned)time(NULL)); 
    int *array = (int *)malloc(N * sizeof(int));
    if (array == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++) {
        array[i] = rand() % (10 * N); 
    }
    return array;
}

void DeallocateArray(int *A)
{
    free(A);
}

int compareInts(const void *a, const void *b) {
    const int *ia = (const int *)a;
    const int *ib = (const int *)b;
    return *ia - *ib; // This will sort in ascending order
}

void SortArray(int N, int *A) {
    qsort(A, N, sizeof(int), compareInts);
}

int main()
{
    int sizes[8] = { 1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000 };

/* For fun:
 *  int sizes[11] = { 1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000,
 *                    256000, 512000, 1024000 }; 
 */

    for (int i = 0 ; i < 8 ; i++)
    {
        double alloc_time = 0., sort_time = 0., dealloc_time = 0.;
        struct timeval startTime;
        struct timeval endTime;

        /* Call the three functions in a sequence. Also use
         * gettimeofday calls surrounding each function and set the 
         * corresponding variable (alloc_time, sort_time, dealloc_time).
         */

        // Allocation timing
        gettimeofday(&startTime, NULL);
        int *array = AllocateArray(sizes[i]);
        gettimeofday(&endTime, NULL);
        alloc_time = (endTime.tv_sec - startTime.tv_sec) * 1000.0;     
        alloc_time += (endTime.tv_usec - startTime.tv_usec) / 1000.0;   

        // Sorting timing
        gettimeofday(&startTime, NULL);
        SortArray(sizes[i], array);
        gettimeofday(&endTime, NULL);
        sort_time = (endTime.tv_sec - startTime.tv_sec) * 1000.0;     
        sort_time += (endTime.tv_usec - startTime.tv_usec) / 1000.0;   

        // Deallocation timing
        gettimeofday(&startTime, NULL);
        DeallocateArray(array);
        gettimeofday(&endTime, NULL);
        dealloc_time = (endTime.tv_sec - startTime.tv_sec) * 1000.0;      
        dealloc_time += (endTime.tv_usec - startTime.tv_usec) / 1000.0;   

        printf("Timings for array of size %d\n", sizes[i]);
        printf("\tTime for allocation is %g, time per element = %g\n", alloc_time, alloc_time/sizes[i]);
        printf("\tTime for sort_time is %g, time per element = %g\n", sort_time, sort_time/sizes[i]);
        printf("\tTime for deallocation is %g\n", dealloc_time);
    }
}
