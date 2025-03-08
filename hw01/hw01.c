// EE3410 HW01 Basic Sorts
// 111061237, 王修呈
// 2025/03/02
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
int N;  // input size
char **data; // input data
char **A; // array to be sorted
int R = 500; // number of repetitions
void readInput(void); // read all inputs
void printArray(char **A); // print the content of array A
void copyArray(char **data, char **A); // copy data to array A
void swap(char **a, char **b); // swap two strings
double GetTime(void); // get local time in seconds
void SelectionSort(char **list, int n); // in-place selection sort
void InsertionSort(char **list, int n); // in-place insertion sort
void BubbleSort(char **list, int n); // in-place bubble sort
void minMaxSort(char **list, int n); // in-place min-max sort

void readInput(void) {
    int i; // loop index
    scanf("%d", &N); // read input size
    data = (char **)malloc(N * sizeof(char *)); // allocate memory for data
    A = (char **)malloc(N * sizeof(char *)); // allocate memory for A
    for (i = 0; i < N; i++) {
        data[i] = (char*)malloc(8 * sizeof(char)); // allocate memory for string
        A[i] = (char *)malloc(8 * sizeof(char)); // allocate memory for string
        scanf("%s", data[i]); // read data
    }
}
void printArray(char **A) {
    int i; // loop index
    for (i = 0; i < N; i++) {
        printf("%s\n", A[i]);   // print out string
    }
}
void copyArray(char **data, char **A) {
    int i; // loop index
    for (i = 0; i < N; i++) {
        strcpy(A[i], data[i]);  // copy string
    }
}
void swap(char **a, char **b) {
    char *temp;  // temporary variable
    temp = *a;  // swap two char
    *a = *b;
    *b = temp;
}
double GetTime(void) {
    struct timeval tv; // time value
    gettimeofday(&tv, NULL); // get local time
    return tv.tv_sec + 1e-6 * tv.tv_usec; // return time in seconds
}
void SelectionSort(char **list, int n) {
    int i, j, k; // loop index
    for (i = 0; i < n; i++) {
        j = i;
        for (k = i; k < n; k++) {   // find the smallest string
            if (strcmp(list[k], list[j]) < 0) { // compare two strings
                j = k;  // find the smallest string
            }
        }
        if (j != i) {   // swap two strings if necessary
            swap(&list[i], &list[j]);
        }
    }
}
void InsertionSort(char **list, int n) {
    int i, j; // loop index
    char *item; // item to be inserted
    for (j = 1; j < n; j++) {
        item = list[j]; // get the item to be inserted
        i = j - 1;
        while (i >= 0 && strcmp(item, list[i]) < 0) { // find the correct place
            list[i + 1] = list[i];  // move the string
            i = i - 1;  // move the index
        }
        list[i + 1] = item; // insert the item
    }
}
void BubbleSort(char **list, int n) {
    int i, j; // loop index
    for (i = 0; i < n - 1; i++) { // loop to visit all strings
        for (j = n - 1; j > i; j--) { // choose last string then sart looping
            if (strcmp(list[j], list[j - 1]) < 0) { // keep swapping until stop
                swap(&list[j], &list[j - 1]);
            }
        }
    }
}
void minMaxSort(char **list, int n) {
    int l = 0, h = n - 1; // left and right index
    int i, imin, imax; // loop index
    if (strcmp(list[h], list[l]) < 0) { // make sure the first string is min
        swap(&list[l], &list[h]);
    }
    while (l < h) { // loop to find the min and max string
        imin = l; 
        imax = h;
        for (i = l + 1; i < h; i++) {
            if (strcmp(list[i], list[imin]) < 0) {  // find the min string
                imin = i;
            } else if (strcmp(list[i], list[imax]) > 0) { // find the max string
                imax = i;
            }
        }
        if (imin > l) {
            swap(&list[l], &list[imin]);
        }
        if (imax < h) {
            swap(&list[h], &list[imax]);
        }
        l++;    // move left index
        h--;    // move right index
    }
}

int main(void) {
int i; // loop index
double t;   // time counter

readInput(); // read input to data array
printf("N = %d\n", N); // print out input size

t = GetTime(); // initialize time counter
for (i = 0; i < R; i++) {
    copyArray(data, A); // initialize array for sorting
    SelectionSort(A, N); // execute selection sort
}
t = (GetTime() - t) / R; // calculate CPU time per iteration
printf("Selection sort: %g seconds\n", t); // print out CPU time

t = GetTime(); // initialize time counter
for (i = 0; i < R; i++) {
    copyArray(data, A); // initialize array for sorting
    InsertionSort(A, N); // execute selection sort
}
t = (GetTime() - t) / R; // calculate CPU time per iteration
printf("Insertion sort: %g seconds\n", t); // print out CPU time

t = GetTime(); // initialize time counter
for (i = 0; i < R; i++) {
    copyArray(data, A); // initialize array for sorting
    BubbleSort(A, N); // execute selection sort
}
t = (GetTime() - t) / R; // calculate CPU time per iteration
printf("Bubble sort: %g seconds\n", t); // print out CPU time

t = GetTime(); // start time
for (i = 0; i < R; i++) {
    copyArray(data, A); // initialize array for sorting
    minMaxSort(A, N); // execute selection sort
}
t = (GetTime() - t) / R; // calculate CPU time per iteration
printf("Min-max sort: %g seconds\n", t); // print out CPU time

printArray(A); // print sorted results
return 0;
}