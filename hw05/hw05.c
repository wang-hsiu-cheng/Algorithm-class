// EE3410 HW05 Trading Stock
// 111061237, 王修呈
// 2025/03/31

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct sSTKprice 
{
    int year, month, day;
    double price, change;
} STKprice;

int N;                                           // number of data
int Buy;                                         // buy sock index
int Sell;                                        // sell stock index
double Earn;                                     // profit
STKprice *data;                                  // stock price, change, date
int Nrepeat = 1000;                              // repeat time for average 
void readInput(void);                            // read all inputs
double GetTime(void);                            // get local time in seconds
void BruteForce(void);
double DivideConquer(STKprice* data, int begin, int end);
double DivideConquerXB(STKprice* data, int begin, int mid, int end, int low, 
                        int high);
void ImprovedBruteForce(void);
double BestALG(void);

void readInput(void)
{
    int i;                                       // loop index
    
    scanf("%d", &N);                             // read data number
    data = (STKprice *)malloc(N * sizeof(STKprice)); // allocate array's memory
    for (i = 0; i < N; i++) {
        scanf("%d %d %d %lg",
             &data[i].year, &data[i].month, &data[i].day, &data[i].price);
        if (i > 0)
            data[i].change = data[i].price - data[i - 1].price;
    }
}

double GetTime(void)
{
    struct timeval tv;                           // time value

    gettimeofday(&tv, NULL);                     // get local time
    return tv.tv_sec + 1e-6 * tv.tv_usec;        // return time in seconds
}

void BruteForce()
{
    int j, k, i;
    double sum;
    
    Earn = 0;                                    // Initialize
    Buy = 0;
    Sell = N - 1;
    for (j = 0; j < N; j++) {                    // Try all possible ranges
        for (k = j; k < N; k++) {
            sum = 0;
            for (i = j; i <= k; i++) {           // Summation for A[j:k]
                sum += data[i].change;
            }
            if (sum > Earn) {                    // Record max value and range
                Earn = sum;
                Buy = j-1;
                Sell = k;
            }
        }
    }
}

double DivideConquer(STKprice *data, int begin, int end)
{
    int low, high;
    int mid, llow, lhigh, rlow, rhigh, xlow, xhigh;
    double lsum, rsum, xsum;
    
    if (begin == end) {                          // termination condition
        low = begin;
        high = end;
        return data[begin].change;
    }
    mid = (begin + end) / 2;
    lsum = DivideConquer(data, begin, mid);      // left region
    rsum = DivideConquer(data, mid + 1, end);    // right region
    xsum = DivideConquerXB(data, begin, mid, end, xlow, xhigh);//cross boundary
    if ((lsum >= rsum) && (lsum >= xsum)) {      // lsum is the largest
        low = llow;
        high = lhigh;
        return lsum;
    }
    else if ((rsum >= lsum) && (rsum >= xsum)) { // rsum is the largest
        low = rlow;
        high = rhigh;
        return rsum;
    }
    low = xlow;
    high = xhigh;
    return xsum;                               // cross-boundary is the largest
}

double DivideConquerXB(STKprice *data, int begin, int mid, int end, int low, int high)
{
    int i;
    double lsum, rsum, sum;

    lsum = 0;                                    // Initialize for lowerhalf
    low = mid;
    sum = 0;
    for (i = mid; i > begin - 1; i--) {          // find low to max sum
        sum += data[i].change;                   // continue to add
        if (sum > lsum) {                        // record if larger
            lsum = sum;
            low = i;
        }
    }
    rsum = 0;                                    // Initialize for higher half
    high = mid + 1;
    sum = 0;
    for (i = mid + 1; i < end + 1; i++) {        // find end to max sum
        sum += data[i].change;                   // Continue to add
        if (sum > rsum) {                        // Record if larger
            rsum = sum;
            high = i;
        }
    }
    return lsum + rsum;                          // Overall sum
}

void ImprovedBruteForce()
{
    int j, k, i;
    double sum;
    
    Earn = 0;                                    // Initialize
    Buy = 0;
    Sell = N - 1;
    sum = 0;
    for (j = 0; j < N; j++) {                    // Try all possible ranges
        for (k = j; k < N; k++) {
            sum = data[k].price - data[j].price;
            if (sum > Earn) {                    // Record max value and range
                Earn = sum;
                Buy = j;
                Sell = k;
            }
        }
    }
}

double BestALG() {
    int i;
    double maxSum;

    Earn = data[0].change;
    maxSum = data[0].change;
    Buy = 0;
    Sell = 0;
    for (i = 1; i < N; i++) {
        if (maxSum + data[i].change > data[i].change) {
            maxSum += data[i].change;            // max sum subarray end at i-1
        }
        else {                                   // start new subarray from i
            maxSum = data[i].change;
            Buy = i - 1;
        }
        if (Earn <= maxSum) {         // Update earn if max subarray sum > earn
            Earn = maxSum;                       // max sum end at index i
            Sell = i;
        }
    }
    return Earn;
}

int main(void)
{
    int i;                                       // loop index
    double t0, t1, t2, t3, t4;                   // time counter
    
    readInput();                        // Read and initialize every day change
    printf("N = %d\n", N);                       // print out N

    t0 = GetTime();                              // initialize time counter
    BruteForce();
    t1 = GetTime();
    printf("Brute-force approach: CPU time %g s\n"
        "  Buy:     %d/%d/%d at %g\n"
        "  Sell     %d/%d/%d at %g\n"
        "  Earning: %g per share.\n", 
        (t1 - t0),
        data[Buy].year, data[Buy].month, data[Buy].day, data[Buy].price,
        data[Sell].year, data[Sell].month, data[Sell].day, data[Sell].price,
        Earn);
    for (i = 0; i < Nrepeat; i++)
        Earn = DivideConquer(data, 0, N - 1);
    t2 = GetTime();
    printf("Divide and Conquer: CPU time %g s\n"
        "  Buy:     %d/%d/%d at %g\n"
        "  Sell     %d/%d/%d at %g\n"
        "  Earning: %g per share.\n", 
        (t2 - t1) / Nrepeat,
        data[Buy].year, data[Buy].month, data[Buy].day, data[Buy].price,
        data[Sell].year, data[Sell].month, data[Sell].day, data[Sell].price,
        Earn);
    for (i = 0; i < Nrepeat; i++)
        ImprovedBruteForce();
    t3 = GetTime();
    printf("Improved brute-force approach: CPU time %g s\n"
        "  Buy:     %d/%d/%d at %g\n"
        "  Sell     %d/%d/%d at %g\n"
        "  Earning: %g per share.\n", 
        (t3 - t2) / Nrepeat,
        data[Buy].year, data[Buy].month, data[Buy].day, data[Buy].price,
        data[Sell].year, data[Sell].month, data[Sell].day, data[Sell].price,
        Earn);
    for (i = 0; i < Nrepeat; i++)
        Earn = BestALG();
    t4 = GetTime();
    printf("Best Algorithm: CPU time %g s\n"
        "  Buy:     %d/%d/%d at %g\n"
        "  Sell     %d/%d/%d at %g\n"
        "  Earning: %g per share.\n", 
        (t4 - t3) / Nrepeat,
        data[Buy].year, data[Buy].month, data[Buy].day, data[Buy].price,
        data[Sell].year, data[Sell].month, data[Sell].day, data[Sell].price,
        Earn);

    return 0;
}