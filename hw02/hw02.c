// EE3410 HW02 Random Data Searches
// 111061237, 王修呈
// 2025/03/10
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

int N;                                            // input size
char **data;                                      // input data
int RAverage = 500;                               // repeat time for average 
int RWorstCase = 5000;                            // repeat times for worstcase
void readInput(void);                             // read all inputs
double GetTime(void);                             // get local time in seconds
int Search(char *word, char **list, int n);       // Linear Search
int BDSearch(char *word, char **list,int n);      // Bidirection Search
int RDSearch(char *word, char **list,int n);      // Random-direction Search

void readInput(void)
{
    int i;                                        // loop index

    scanf("%d", &N); // read input size
    data = (char **)malloc(N * sizeof(char *));   // allocate memory for data
    for (i = 0; i < N; i++)
    {
        data[i] = (char*)malloc(8 * sizeof(char));// allocate memory for string
        scanf("%s", data[i]);                     // read data
    }
}

double GetTime(void)
{
    struct timeval tv;                            // time value

    gettimeofday(&tv, NULL);                      // get local time
    return tv.tv_sec + 1e-6 * tv.tv_usec;         // return time in seconds
}

int Search(char *word, char **list, int n)
{
    int i;

    for (i = 0; i < n; i++)                     // compare all possible entries
        if (strcmp(list[i], word) == 0) 
            return i;                             // successful
    return -1;
}

int BDSearch(char *word, char **list,int n)
{
    int i;

    for (i = 0; i < (n - 1) / 2 + 1; i++)   // compare all entries from two dir
    { 
        if (strcmp(list[i], word) == 0)           // successful
            return i;
        if (strcmp(list[n - i - 1], word) == 0)   // successful
            return n - i - 1;
    }
    return -1;
}

int RDSearch(char *word, char **list, int n)
{
    int i, j;

    j = rand() % 2;
    if (j == 1)
    {
        for (i = 0; i < n; i++)
            if (strcmp(list[i], word) == 0)
                return i;
    }
    else
    {
        for (i = n - 1; i >= 0; i--)
            if (strcmp(list[i], word) == 0)
                return i;
    }
    return -1;
}

int main(void)
{
    int i, j;                                // loop index
    double t;                                // time counter
    int worstCase, worstCaseBD, worstCaseRD; // worstcase index for each search

    srand(time(NULL));                           // setup the random seed

    readInput();                                 // read input to data array
    printf("n: %d\n", N);                        // print out input size
    worstCase = N - 1;                           // assign the worst case index
    worstCaseBD = (N - 1) / 2;                   // assign the worst case index
    worstCaseRD = 0;                             // assign the worst case index
    t = GetTime();                               // initialize time counter
    for (i = 0; i < RAverage; i++)
    {
        for (j = 0; j < N; j++)
        {
            Search(data[j], data, N);
        }
    }
    t = (GetTime() - t) / (RAverage * N);   // calculate CPU time per iteration
    printf("Linear search average CPU time: %g\n", t);    // print out CPU time

    t = GetTime();                          // initialize time counter
    for (i = 0; i < RAverage; i++)
    {
        for (j = 0; j < N; j++)
        {
            BDSearch(data[j], data, N);
        }
    }
    t = (GetTime() - t) / (RAverage * N);   // calculate CPU time per iteration
    printf("Bidirection search average CPU time: %g\n", t);   // print CPU time

    t = GetTime(); // initialize time counter
    for (i = 0; i < RAverage; i++)
    {
        for (j = 0; j < N; j++)
        {
            RDSearch(data[j], data, N);
        }
    }
    t = (GetTime() - t) / (RAverage * N);   // calculate CPU time per iteration
    printf("Random-direction search average CPU time: %g\n", t);

    t = GetTime();                          // initialize time counter
    for (i = 0; i < RWorstCase; i++)
    {
        Search(data[worstCase], data, N);
    }
    t = (GetTime() - t) / RWorstCase;       // calculate CPU time per iteration
    printf("Linear search worst-case CPU time: %g\n", t); // print out CPU time

    t = GetTime();                          // initialize time counter
    for (i = 0; i < RWorstCase; i++)
    {
        BDSearch(data[worstCaseBD], data, N);
    }
    t = (GetTime() - t) / RWorstCase;       // calculate CPU time per iteration
    printf("Bidirection search worst-case CPU time: %g\n", t);

    t = GetTime();                          // initialize time counter
    for (i = 0; i < RWorstCase; i++)
    {
        RDSearch(data[worstCaseRD], data, N);
    }
    t = (GetTime() - t) / RWorstCase;       // calculate CPU time per iteration
    printf("Random-direction search worst-case CPU time: %g\n", t);

    return 0;
}