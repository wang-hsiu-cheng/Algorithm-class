// EE3410 HW03 Heap Sorts
// 111061237, 王修呈
// 2025/03/17

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

int N;                                       // input size
char **data;                                 // input data
char **A;                                    // array to be sorted
int R = 500;                                 // repeat time for average 
void readInput(void);                        // read all inputs
void printArray(char **A);                   // print the content of array A
void copyArray(char **data, char **A);       // copy data to array A
double GetTime(void);                        // get local time in seconds
int compare(char *str1, char *str2);
void swap(char **a, char **b);               // swap two strings
void HeapSort(char **list, int n);           // heap sort with strcmp
void Heapify(char **list, int i, int n);
void BUHeapSort(char **list, int n);         // buttom-up heap sort with strcmp
void BUHeapify(char **list, int i, int n);
void HeapSort1(char **list, int n);          // heap sort with compare
void Heapify1(char **list, int i, int n);
void BUHeapSort1(char **list, int n);       // buttom-up heap sort with compare
void BUHeapify1(char **list, int i, int n);

void readInput(void)
{
    int i; // loop index
    
    scanf("%d", &N);                              // read input size
    data = (char **)malloc(N * sizeof(char *));   // allocate memory for data
    A = (char **)malloc(N * sizeof(char *));      // allocate memory for A
    for (i = 0; i < N; i++)
    {
        data[i] = (char*)malloc(10 * sizeof(char));//allocate memory for string
        A[i] = (char *)malloc(10 * sizeof(char)); // allocate memory for string
        scanf("%s", data[i]);                     // read data
    }
}

void printArray(char **list)
{
    int i;                                        // loop index

    for (i = 0; i < N; i++)
    {
        printf("%s\n", list[i]);                  // print out string
    }
}

void copyArray(char **a, char **b)
{
    int i;                                        // loop index

    for (i = 0; i < N; i++)
    {
        strcpy(b[i], a[i]);                       // copy string
    }
}

double GetTime(void)
{
    struct timeval tv;                            // time value

    gettimeofday(&tv, NULL);                      // get local time
    return tv.tv_sec + 1e-6 * tv.tv_usec;         // return time in seconds
}

int compare(char *str1, char *str2)
{
    struct timespec tim = {}, tim2 = {};

    tim.tv_nsec = 10;                             // set delay time
    nanosleep(&tim, &tim2);                       // leday for a while
    return strcmp(str1, str2);                    // compare two string
}

void swap(char **a, char **b) {
    char *temp;                                   // temporary variable

    temp = *a;                                    // swap two char
    *a = *b;
    *b = temp;
}

void HeapSort(char **list, int n)
{
    int i;

    for (i = (n - 1) / 2; i >= 0; i--)            // Initialize A[0:n-1] to be a max heap.
    {
        Heapify(list, i, n);
    }
    for (i = n - 1; i >= 0; i--)
    {
        swap(&list[i], &list[0]);                 // Move maximum to the end.
        Heapify(list, 0, i);                      // Then make A[0:i] a max heap.
    }
}

void Heapify(char **list, int i, int n)
{
    int j;
    char *item;
    int done;

    j = 2 * i + 1;                                // A[j] is the lchild.
    item = list[i];
    done = 0;
    while (j < n-1 && !done)                      // A[j+1] is the rchild.
    {
        if (j < n-1 && strcmp(list[j], list[j + 1]) < 0)
        {
            j++;                                  // A[j] is the larger child.
        }
        if (strcmp(item, list[j]) > 0)        // If larger than children, done.
        {
            done = 1;
        }
        else                                      // Otherwise, continue.
        {
            list[(j + 1) / 2 - 1] = list[j];
            j = 2 * j + 1;
        }
    }
    list[(j + 1) / 2 - 1] = item;
}

void BUHeapSort(char **list, int n)
{
    int i;

    for (i = (n - 1) / 2; i >= 0; i--) // Initialize A[0:n-1] to be a max heap.
    {
        BUHeapify(list, i, n);
    }
    for (i = n - 1; i >= 0; i--)
    {
        swap(&list[i], &list[0]);                 // Move maximum to the end.
        BUHeapify(list, 0, i);                  // Then make A[0:i] a max heap.
    }
}

void BUHeapify(char **list, int i, int n)
{
    int j;
    char *item;

    j = i;                       // assuming A[i] is smaller than all the nodes
    while (2 * j + 2 < n)                         // j’s rchild exists
    {
        if (strcmp(list[2 * j + 1], list[2 * j + 2]) > 0)
            j = 2 * j + 1;                        // to lchild
        else
            j = 2 * j + 2;                        // to rchild
    }
    if (2 * j + 1 < n)
        j = 2 * j + 1;                            // lchild exists, move to it
    while (strcmp(list[i], list[j]) > 0)          // find place for A[i]
        j = (j + 1) / 2 - 1;
    item = list[j];                               // save A[j] to item
    list[j] = list[i];                            // move A[i] to A[j]
    while (j > i)                           // move all nodes up along the path
    {
        j = (j + 1) / 2 - 1;                      // to parent
        swap(&list[j], &item);                    // swap with item
    }
}

void HeapSort1(char **list, int n)
{
    int i;

    for (i = (n - 1) / 2; i >= 0; i--) // Initialize A[0:n-1] to be a max heap.
    {
        Heapify1(list, i, n);
    }
    for (i = n - 1; i >= 0; i--)
    {
        swap(&list[i], &list[0]);                 // Move maximum to the end.
        Heapify1(list, 0, i);                   // Then make A[0:i] a max heap.
    }
}

void Heapify1(char **list, int i, int n)
{
    int j;
    char *item;
    int done;

    j = 2 * i + 1;                                // A[j] is the lchild.
    item = list[i];
    done = 0;
    while (j < n-1 && !done)                      // A[j+1] is the rchild.
    {
        if (j < n-1 && compare(list[j], list[j + 1]) < 0)
        {
            j++;                                  // A[j] is the larger child.
        }
        if (compare(item, list[j]) > 0)       // If larger than children, done.
        {
            done = 1;
        }
        else                                      // Otherwise, continue.
        {
            list[(j + 1) / 2 - 1] = list[j];
            j = 2 * j + 1;
        }
    }
    list[(j + 1) / 2 - 1] = item;
}

void BUHeapSort1(char **list, int n)
{
    int i;

    for (i = (n - 1) / 2; i >= 0; i--) // Initialize A[0:n-1] to be a max heap.
    {
        BUHeapify1(list, i, n);
    }
    for (i = n - 1; i >= 0; i--)
    {
        swap(&list[i], &list[0]);                 // Move maximum to the end.
        BUHeapify1(list, 0, i);                   // Then make A[0:i] a max heap.
    }
}

void BUHeapify1(char **list, int i, int n)
{
    int j;
    char *item;

    j = i;                       // assuming A[i] is smaller than all the nodes
    while (2 * j + 2 < n)                         // j’s rchild exists
    {
        if (compare(list[2 * j + 1], list[2 * j + 2]) > 0)
            j = 2 * j + 1;                        // to lchild
        else
            j = 2 * j + 2;                        // to rchild
    }
    if (2 * j + 1 < n)
        j = 2 * j + 1;                            // lchild exists, move to it
    while (compare(list[i], list[j]) > 0)         // find place for A[i]
        j = (j + 1) / 2 - 1;
    item = list[j];                               // save A[j] to item
    list[j] = list[i];                            // move A[i] to A[j]
    while (j > i)                           // move all nodes up along the path
    {
        j = (j + 1) / 2 - 1;                      // to parent
        swap(&list[j], &item);                    // swap with item
    }
}

int main(void)
{
    int i, j;                               // loop index
    double t;                               // time counter
    double total_time;

    readInput();                            // read input to data array
    printf("N = %d\n", N);                  // print out input size

    for (i = 0; i < R; i++)
    {
        copyArray(data, A);                 // initialize array for sorting
        t = GetTime();                      // initialize time counter
        HeapSort(A, N);
        total_time += (GetTime() - t);
    }
    t = total_time / R;                     // calculate CPU time per iteration
    printf("HeapSort with strcmp: %g second\n", t); // print out CPU time

    t = GetTime();                          // initialize time counter
    for (i = 0; i < R; i++)
    {
        copyArray(data, A);                 // initialize array for sorting
        t = GetTime();                      // initialize time counter
        BUHeapSort(A, N);
        total_time += (GetTime() - t);
    }
    t = total_time / R;                     // calculate CPU time per iteration
    printf("BUHeapSort with strcmp: %g second\n", t); // print out CPU time

    t = GetTime();                          // initialize time counter
    for (i = 0; i < R; i++)
    {
        copyArray(data, A);                 // initialize array for sorting
        t = GetTime();                      // initialize time counter
        HeapSort1(A, N);
        total_time += (GetTime() - t);
    }
    t = total_time / R;                     // calculate CPU time per iteration
    printf("HeapSort with compare: %g second\n", t);      // print out CPU time

    t = GetTime();                          // initialize time counter
    for (i = 0; i < R; i++)
    {
        copyArray(data, A);                 // initialize array for sorting
        t = GetTime();                      // initialize time counter
        HeapSort(A, N);
        total_time += (GetTime() - t);
    }
    t = total_time / R;                     // calculate CPU time per iteration
    printf("BUHeapSort with compare: %g second\n", t);    // print out CPU time

    printArray(A);                          // print sorted results
    return 0;
}