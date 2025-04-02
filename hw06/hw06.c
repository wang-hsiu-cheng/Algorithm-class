// EE3410 HW06 Better Sorts
// 111061237, 王修呈
// 2025/04/07

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

int N;                                       // input size
char **data;                                 // input data
char **A;                                    // array to be sorted
int R = 1;                                 // repeat time for average 
void readInput(void);                        // read all inputs
void printArray(char **A);                   // print the content of array A
void copyArray(char **data, char **A);       // copy data to array A
double GetTime(void);                        // get local time in seconds
void swap(char **a, char **b);               // swap two strings
void BUHeapSort(char **list, int n);         // buttom-up heap sort with strcmp
void BUHeapify(char **list, int i, int n);
void MergeSort(char **list, int low, int high);
void Merge(char **list, int low, int mid, int high);
void MergeSort1(char **list, int low, int high);
void InsertionSort(char **list, int low, int high); // in-place insertion sort
void QuickSort(char **list, int low, int high);
int Partition(char **list, int low, int high);
void QuickSort1(char **list, int low, int high);
void RQuickSort(char **list, int low, int high);

void readInput(void)
{
    int i; // loop index
    
    scanf("%d", &N);                              // read input size
    data = (char **)malloc((N+1) * sizeof(char *));   // allocate memory for data
    A = (char **)malloc((N+1) * sizeof(char *));      // allocate memory for A
    for (i = 0; i < N; i++)
    {
        data[i] = (char*)malloc(10 * sizeof(char));//allocate memory for string
        A[i] = (char *)malloc(10 * sizeof(char)); // allocate memory for string
        scanf("%s", data[i]);                     // read data
    }
    data[N] = (char*)malloc(10 * sizeof(char));
    A[N] = (char *)malloc(10 * sizeof(char)); 
    data[N] = "zzzzzzzz";
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

    for (i = 0; i < N + 1; i++)
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

void swap(char **a, char **b) {
    char *temp;                                   // temporary variable

    temp = *a;                                    // swap two char
    *a = *b;
    *b = temp;
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

void MergeSort(char **list, int low, int high)
{
    int mid;

    if (low < high) {
        mid = (low + high) / 2;
        MergeSort(list, low, mid);
        MergeSort(list, mid + 1, high);
        Merge(list, low, mid, high);
    }
}

void Merge(char **list, int low, int mid, int high)
{
    int h, i, j, k, l;
    char **temp;

    h = low;
    i = low;
    j = mid + 1; //Initializeloopingindices.
    temp = (char **)malloc(N * sizeof(char *));      // allocate memory for A
    for (l = 0; l < N; l++)
        temp[l] = (char*)malloc(10 * sizeof(char));//allocate memory for string
    while ((h <= mid) && (j <= high)) { //Store smaller one to B[i].
        if (strcmp(list[h], list[j]) <= 0) { //list[h] issmaller.
            temp[i] = list[h];
            h++;
        }
        else { //list[j] issmaller.
            temp[i] = list[j];
            j++;
        }
        i++;
    }
    if (h > mid) //list[j:high] remaining.
        for (k = j; k < high + 1; k++){
            temp[i] = list[k];
            i++;
        }
    else // A[h:mid] remaining.
        for (k = h; k < mid + 1; k++) {
            temp[i] = list[k];
            i++;
        }
    for (k = low; k < high + 1; k++)
    {
        list[k] = temp[k]; // Copy B to list.
    }
}

void MergeSort1(char **list, int low, int high)
{
    int mid;

    if ((high - low) < 15) // When list is small, perfor minsertion sort.
        return InsertionSort(list, low, high);
    else{ // For large list, divide-and-conquer merge sort.
        mid = (low + high) / 2;
        MergeSort1(list, low, mid);
        MergeSort1(list, mid + 1, high);
        Merge(list, low, mid, high);
    }
}

void InsertionSort(char **list, int low, int high) {
    int j, i;
    char* item;

    for (j = low + 1; j < high + 1; j++) { // Check for every low < j <= high
        item = list[j]; // Compare list[i] and list[j], i < j.
        i = j - 1;
        while ((i >= low) && (strcmp(item, list[i]) < 0)) { // Make room for item = list[j]
            list[i + 1] = list[i];
            i--;
        }
        list[i + 1] = item; // Store item.
    }
}

void QuickSort(char **list, int low, int high)
{
    int mid;

    if (low < high) {
        mid = Partition(list, low, high + 1);
        // printf("low: %d, high: %d, mid: %d\n", low, high, mid);
        // printArray(list);
        QuickSort(list, low, mid - 1);
        QuickSort(list, mid + 1, high);
    }
}

int Partition(char **list, int low, int high)
{
    char* v;
    int i, j;

    v = list[low];
    i = low;
    j = high; // Initialize
    while (i < j) { //Check for all elements.
        while (strcmp(list[i], v) <= 0) {
            i++;
        } // Find i such that list[i] ≥ v.
        while (strcmp(list[j], v) > 0) {
            j--;
        } // Find j such that list[j] ≤ v.
        if (i < j)
            swap(&list[i], &list[j]); //Exchange list[i] and list[j].
    }
    list[low] = list[j];
    list[j] = v; // Move v to the right position.
    return j;
 }

void QuickSort1(char **list, int low, int high)
{
    int mid;
    if ((high - low) < 15) // When list is small, perfor minsertion sort.
        return InsertionSort(list, low, high);
    if (low < high) {
        mid = Partition(list, low, high + 1);
        QuickSort(list, low, mid - 1);
        QuickSort(list, mid + 1, high);
    }
}

void RQuickSort(char **list, int low, int high)
{
    int mid;
    int random;

    if (low < high) {
        if ((high - low) > 5)
        {
            random = rand() % (high - low) + 1;
            swap(&list[low + random], &list[low]);
        }
        mid = Partition(list, low, high + 1);
        RQuickSort(list, low, mid - 1);
        RQuickSort(list, mid + 1, high);
    }
}

int main(void)
{
    int i, j;                               // loop index
    double t;                               // time counter
    double t0, t1, t2, t3, t4, t5, t6;      // time counter

    srand(time(NULL));                           // setup the random seed
    readInput();                            // read input to data array
    printf("N = %d\n", N);                  // print out input size

    // t0 = GetTime();                              // initialize time counter
    // for (i = 0; i < R; i++)
    // {
    //     copyArray(data, A);
    //     BUHeapSort(A, N);
    // }
    // printArray(A);
    // t1 = GetTime();
    // for (i = 0; i < R; i++)
    // {
    //     copyArray(data, A);    
    //     MergeSort(A, 0, N - 1);
    // }
    // t2 = GetTime();
    // for (i = 0; i < R; i++)
    // {
    //     copyArray(data, A);    
    //     MergeSort1(A, 0, N - 1);
    // }
    // printArray(A);
    // t3 = GetTime();
    // for (i = 0; i < R; i++)
    // {
    //     copyArray(data, A); 
    //     QuickSort(A, 0, N - 1);
    // }
    // printArray(A);
    // t4 = GetTime();
    // for (i = 0; i < R; i++)
    // {
    //     copyArray(data, A);    
    //     QuickSort1(A, 0, N - 1);
    // }
    t5 = GetTime();
    for (i = 0; i < R; i++)
    {
        copyArray(data, A);    
        RQuickSort(A, 0, N-1);
    }
    t6 = GetTime();
    printf("HeapSort CPU time: %g s\n", (t1 - t0) / R);
    printf("MergeSort CPU time: %g s\n", (t2 - t1) / R);
    printf("MergeSort1 CPU time: %g s\n", (t3 - t2) / R);
    printf("QuickSort CPU time: %g s\n", (t4 - t3) / R);
    printf("QuickSort1 CPU time: %g s\n", (t5 - t4) / R);
    printf("RQuickSort CPU time: %g s\n", (t6 - t5) / R);

    printArray(A);                          // print sorted results
    return 0;
}