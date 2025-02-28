// EE3410 HW01 Basic Sorts
// 111061237, 王修呈
// 2025/03/01
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
int N;  // input size
char **data; // input data
char **A; // array to be sorted
int R = 500; // number of repetitions
void readInput(void); // read all inputs
void printArray(char **A); // print the content of array A
void copyArray(char **data, char **A); // copy data to array A
double GetTime(void); // get local time in seconds
void SelectionSort(char **list,int n); // in-place selection sort
void InsertionSort(char **list,int n); // in-place insertion sort
void GnomeSort(char **list,int n); // in-place gnome sort

void readInput(void) {
    int i; // loop index
    scanf("%d", &N); // read input size
    data = (char **)malloc(N * sizeof(char *)); // allocate memory for data
    A = (char **)malloc(N * sizeof(char *)); // allocate memory for A
    for (i = 0; i < N; i++) {
        data[i] = (char *)malloc(8 * sizeof(char)); // allocate memory for each data
        A[i] = (char *)malloc(8 * sizeof(char)); // allocate memory for each A
        scanf("%s", data[i]); // read data
    }
}
void printArray(char **A) {
    int i; // loop index
    for (i = 0; i < N; i++) {
        printf("%s\n", A[i]);
    }
}
void copyArray(char **data, char **A) {
    int i; // loop index
    for (i = 0; i < N; i++) {
        strcpy(A[i], data[i]);
    }
}
double GetTime(void) {
    time_t seconds;
    seconds = time(NULL);
    return (double)seconds;
}
void SelectionSort(char **list,int n) {
    
}
void InsertionSort(char **list,int n) {

}
void GnomeSort(char **list,int n) {

}

int main(void) {
int i; // loop index
double t; // for CPU time tracking
readInput(); // read input to data array
printf("N = %d", N); // print out input size

t = GetTime(); // initialize time counter
for (i = 0; i < R; i++) {
    copyArray(data, A); // initialize array for sorting
    SelectionSort(A, N); // execute selection sort
}
t = (GetTime()- t) / R; // calculate CPU time per iteration
printf("Selection sort: %g seconds", t); // print out CPU time

t = GetTime(); // initialize time counter
for (i = 0; i < R; i++) {
    copyArray(data, A); // initialize array for sorting
    SelectionSort(A, N); // execute selection sort
}
t = (GetTime()- t) / R; // calculate CPU time per iteration
printf("Insertion sort: %g seconds", t); // print out CPU time

t = GetTime(); // initialize time counter
for (i = 0; i < R; i++) {
    copyArray(data, A); // initialize array for sorting
    SelectionSort(A, N); // execute selection sort
}
t = (GetTime()- t) / R; // calculate CPU time per iteration
printf("Bubble sort: %g seconds", t); // print out CPU time

t = GetTime(); // initialize time counter
for (i = 0; i < R; i++) {
    copyArray(data, A); // initialize array for sorting
    SelectionSort(A, N); // execute selection sort
}
t = (GetTime()- t) / R; // calculate CPU time per iteration
printf("Min-max sort: %g seconds", t); // print out CPU time

printArray(A); // print sorted results
return 0;
}