// EE3410 HW10 Coin Set Design
// 111061237, 王修呈
// 2025/05/10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

int NCoin = 4;
int Coins[] = {1, 5, 10, 50};
int **G;
int D = 99;
double avg_greedy = 0;
double avg_recursive = 0;
double avg_top = 0;
double avg_bottom = 0;
double t1, t2, t3, t4;
double GetTime(void);
void ResetArray(int **A, int a, int b);
void Origin(void);
void Replace(int C);
void ReplaceTwo(void);
int MCoinGreedy(int D, int NCoin, int COINs[]); 
int NCoinDP_R(int D, int Ncoin, int Coins[]);   // DP recursive approach
int NCoinDP_TD(int D, int Ncoin, int Coins[]);  // DP top-down approach
void NCoinDP_BU(int D, int Ncoin, int Coins[]); // DP bottom-up approach

double GetTime(void)
{
    struct timeval tv;                            // _time value

    gettimeofday(&tv, NULL);                      // get local _time
    return tv.tv_sec + 1e-6 * tv.tv_usec;         // return _time in seconds
}

int MCoinGreedy(int D, int NCoin, int Coins[])
{
    int i;
    int result;

    i = NCoin;
    result = 0;
    while (D && i > 0) {
        while (D < Coins[i - 1])
            i--;
        if (i > 0) {
            D -= Coins[i - 1];
            result++;
        }
    }
    return result;
}

int NCoinDP_R(int D, int Ncoin, int Coins[])
{
    int x_i;
    int temp, result;
    int min_ = 100;

    if (Ncoin == 1)
        return D;
    for (x_i = 0; x_i <= D / Coins[Ncoin - 1]; x_i++) {
        if (D - x_i * Coins[Ncoin - 1]) {
            temp = NCoinDP_R(D - x_i * Coins[Ncoin - 1], Ncoin - 1, Coins) + x_i;
        } else {
            temp = x_i;
        }
        min_ = (temp < min_) ? temp : min_;
    }
    result = min_;
    return result;
}

int NCoinDP_TD(int D, int Ncoin, int Coins[])
{
    int x_i;
    int temp, result;
    int min_ = 100;

    if (G[Ncoin - 1][D - 1])
        return G[Ncoin - 1][D - 1];
    else if (Ncoin == 1)
        return D;
    for (x_i = 0; x_i <= D / Coins[Ncoin - 1]; x_i++) {
        if (D - x_i * Coins[Ncoin - 1]) {
            temp = NCoinDP_TD(D - x_i * Coins[Ncoin - 1], Ncoin - 1, Coins) + x_i;
        } else {
            temp = x_i;
        }
        min_ = (temp < min_) ? temp : min_;
    }
    G[Ncoin - 1][D - 1] = min_;
    result = min_;
    return result;
} 

void NCoinDP_BU(int D, int Ncoin, int Coins[])
{   
    int x_i;
    int i, j;
    int temp;
    int result;
    int G[4][99] = {0};

    for (i = 1; i <= Ncoin; i++) {
        for (j = 1; j <= D; j++) {
            if (i == 1)
                G[i - 1][j - 1] = j;
            else
                G[i - 1][j - 1] = 100;
        }
    }

    for (i = 2; i <= Ncoin; i++) {
        for (j = 1; j <= D; j++) {
            for (x_i = 0; x_i <= j / Coins[i - 1]; x_i++) {
                temp = (j - x_i * Coins[i - 1]) ?  G[i - 2][j - x_i * Coins[i - 1] - 1] + x_i : x_i;
                G[i-1][j-1] = (temp < G[i-1][j-1]) ? temp : G[i-1][j-1];
            }
        }
    }
    for (i = 1; i <= D; i++) {
        avg_bottom += G[Ncoin - 1][i - 1];
    }
    avg_bottom /= D;
}

void ResetArray(int **A, int a, int b)
{
    int i, j;

    for (i = 0; i < a; i++) {
        for (j = 0; j < b; j++) {
            A[i][j] = 0;
        }
    }
}

void Origin(void)
{
    int i, j, temp;

    G = (int**)malloc(4 * sizeof(int*));
    for (i = 0; i < 4; i++)
        G[i] = (int*)malloc(99 * sizeof(int));

    printf("Original coin set:\n");
    t1 = GetTime();
    for (i = 1; i <= D; i++) {
        avg_greedy += MCoinGreedy(i, NCoin, Coins);
    }
    t1 = GetTime() - t1;
    avg_greedy /= D;
    t2 = GetTime();
    for (i = 1; i <= D; i++) {
        avg_recursive += NCoinDP_R(i, NCoin, Coins);
    }
    t2 = GetTime() - t2;
    avg_recursive /= D;
    t3 = GetTime();
    for (i = 1; i <= D; i++) {
        avg_top += NCoinDP_TD(i, NCoin, Coins);
    }
    t3 = GetTime() - t3;
    avg_top /= D;
    ResetArray(G, 4, 99);
    t4 = GetTime();
    NCoinDP_BU(D, NCoin, Coins);
    t4 = GetTime() - t4;
}

void Replace(int C)
{
    int i, j, begin, end;
    double temp;
    double test;
    
    begin = Coins[C - 2] + 1;
    end = (C == 4) ? Coins[C - 1] : Coins[C] - 1;
    printf("Replacing $%d:\n", Coins[C - 1]);
    t1 = GetTime();
    temp = 100;
    for (j = begin; j <= end; j++) {
        Coins[C - 1] = j;
        avg_greedy = 0;
        for (i = 1; i <= D; i++) {
            avg_greedy += MCoinGreedy(i, NCoin, Coins);
        }
        avg_greedy /= D;
        temp = (avg_greedy < temp) ? avg_greedy : temp;
    }
    t1 = GetTime() - t1;
    avg_greedy = temp;
    t2 = GetTime();
    temp = 100;
    for (j = begin; j <= end; j++) {
        Coins[C - 1] = j;
        avg_recursive = 0;
        for (i = 1; i <= D; i++) {
            avg_recursive += NCoinDP_R(i, NCoin, Coins);
        }
        avg_recursive /= D;
        temp = (avg_recursive < temp) ? avg_recursive : temp;
    }
    t2 = GetTime() - t2;
    avg_recursive = temp;
    t3 = GetTime();
    temp = 100;
    for (j = begin; j <= end; j++) {
        Coins[C - 1] = j;
        avg_top = 0;
        for (i = 1; i <= D; i++) {
            avg_top += NCoinDP_TD(i, NCoin, Coins);
        }
        avg_top /= D;
        temp = (avg_top < temp) ? avg_top : temp;
        ResetArray(G, 4, 99);
    }
    t3 = GetTime() - t3;
    avg_top = temp;
    t4 = GetTime();
    avg_bottom = 100;
    for (j = begin; j <= end; j++) {
        Coins[C - 1] = j;
        temp = avg_bottom;
        avg_bottom = 0;
        NCoinDP_BU(D, NCoin, Coins);
        avg_bottom = (temp < avg_bottom) ? temp : avg_bottom;
    }
    t4 = GetTime() - t4;
}

void ReplaceTwo(void)
{
    int i, j, k, begin, middle, end;
    double temp;

    Coins[2] = 10;
    begin = Coins[1] + 1;
    middle = Coins[3] - 1;
    end = Coins[3];
    printf("Replacing $%d and $%d:\n", Coins[2], Coins[3]);
    t1 = GetTime();
    temp = 100;
    for (i = begin; i <= middle; i++) {
        Coins[2] = i;
        for (j = i + 1; j <= end; j++) {
            Coins[3] = j;
            avg_greedy = 0;
            for (k = 1; k <= D; k++) {
                avg_greedy += MCoinGreedy(k, NCoin, Coins);
            }
            avg_greedy /= D;
            temp = (avg_greedy < temp) ? avg_greedy : temp;
        }
    }
    t1 = GetTime() - t1;
    avg_greedy = temp;
    t2 = GetTime();
    temp = 100;
    for (i = begin; i <= middle; i++) {
        Coins[2] = i;
        for (j = i + 1; j <= end; j++) {
            Coins[3] = j;
            avg_recursive = 0;
            for (k = 1; k <= D; k++) {
                avg_recursive += NCoinDP_R(k, NCoin, Coins);
            }
            avg_recursive /= D;
            temp = (avg_recursive < temp) ? avg_recursive : temp;
        }
    }
    t2 = GetTime() - t2;
    avg_recursive = temp;
    t3 = GetTime();
    temp = 100;
    for (i = begin; i <= middle; i++) {
        Coins[2] = i;
        for (j = i + 1; j <= end; j++) {
            Coins[3] = j;
            avg_top = 0;
            for (k = 1; k <= D; k++) {
                avg_top += NCoinDP_TD(k, NCoin, Coins);
            }
            avg_top /= D;
            temp = (avg_top < temp) ? avg_top : temp;
            ResetArray(G, 4, 99);
        }
    }
    t3 = GetTime() - t3;
    avg_top = temp;
    t4 = GetTime();
    avg_bottom = 100;
    for (i = begin; i <= middle; i++) {
        Coins[2] = i;
        for (j = i + 1; j <= end; j++) {
            Coins[3] = j;
            temp = avg_bottom;
            avg_bottom = 0;
            NCoinDP_BU(D, NCoin, Coins);
            avg_bottom = (temp < avg_bottom) ? temp : avg_bottom;
        }
    }
    t4 = GetTime() - t4;
}

int main(void)
{
    Origin();
    printf("  Greedy method: {1, 5, 10, 50} average is %g, CPU time: %g sec\n", avg_greedy, t1);
    printf("  DP recursive: {1, 5, 10, 50} average is %g, CPU time: %g sec\n", avg_recursive, t2);
    printf("  DP top-down: {1, 5, 10, 50} average is %g, CPU time: %g sec\n", avg_top, t3);
    printf("  DP bottom-up: {1, 5, 10, 50} average is %g, CPU time: %g sec\n", avg_bottom, t4);
    Replace(4);
    printf("  Greedy method: {1, 5, 10, C4} average is %g, CPU time: %g sec\n", avg_greedy, t1);
    printf("  DP recursive: {1, 5, 10, C4} average is %g, CPU time: %g sec\n", avg_recursive, t2);
    printf("  DP top-down: {1, 5, 10, C4} average is %g, CPU time: %g sec\n", avg_top, t3);
    printf("  DP bottom-up: {1, 5, 10, C4} average is %g, CPU time: %g sec\n", avg_bottom, t4);
    Replace(3);
    printf("  Greedy method: {1, 5, C3, 50} average is %g, CPU time: %g sec\n", avg_greedy, t1);
    printf("  DP recursive: {1, 5, C3, 50} average is %g, CPU time: %g sec\n", avg_recursive, t2);
    printf("  DP top-down: {1, 5, C3, 50} average is %g, CPU time: %g sec\n", avg_top, t3);
    printf("  DP bottom-up: {1, 5, C3, 50} average is %g, CPU time: %g sec\n", avg_bottom, t4);
    ReplaceTwo();
    printf("  Greedy method: {1, 5, C3, C4} average is %g, CPU time: %g sec\n", avg_greedy, t1);
    printf("  DP recursive: {1, 5, C3, C4} average is %g, CPU time: %g sec\n", avg_recursive, t2);
    printf("  DP top-down: {1, 5, C3, C4} average is %g, CPU time: %g sec\n", avg_top, t3);
    printf("  DP bottom-up: {1, 5, C3, C4} average is %g, CPU time: %g sec\n", avg_bottom, t4);
    return 0;
}