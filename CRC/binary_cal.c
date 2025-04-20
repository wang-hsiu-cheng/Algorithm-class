#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

char C[33] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1};
char *E;
int T[6000] = {0};
char *R;
char *temp;
char *B;
long int c = 4374732215;
long int b = 63;
long int t = 0;
int len_b = 6;
int N = 0;
int max_N = 0;

double GetTime(void)
{
    struct timeval tv;                            // time value

    gettimeofday(&tv, NULL);                      // get local time
    return tv.tv_sec + 1e-6 * tv.tv_usec;         // return time in seconds
}

void assign()
{
    E = (char *)malloc(5870 * sizeof(char));
    R = (char *)malloc(32 * sizeof(char));
    B = (char *)malloc(len_b * sizeof(char));
    temp = (char *)malloc(6000 * sizeof(char));
    for (int i = 0; i < 6; i++)
        B[i] = 1;
}

// uint32_t binary_to_decimal(const char *binary) {
//     uint32_t decimal = 0;
//     int length = strlen(binary);

//     for (int i = 0; i < length; i++) {
//         if (binary[i] == '1') {
//             decimal += pow(2, length - 1 - i);
//         } else if (binary[i] != '0') {
//             printf("Invalid binary digit: %c\n", binary[i]);
//             return -1;
//         }
//     }

//     return decimal;
// }

void decimal_to_binary(long int num) {
    if (num == 0) {
        return;
    }

    // Array to store binary digits
    int i = 0;

    while (num > 0) {
        T[i++] = num % 2;
        if (T[i-1])
            N++;
        num /= 2;
    }

    // // Print in reverse order
    // for (int j = i - 1; j >= 0; j--) {
    //     if (T[j])
    //         N++;
    //     printf("%d", T[j]);
    // }
}

long int binary_multiply(long int a, long int b) {
    long int result = 0;
    int i = 0;

    while (b != 0) {
        // 如果 b 的最低位是 1，則加上 a 左移 i 位
        if (b & 1) {
            result += (a << i);
        }
        b >>= 1;  // b 右移一位（往下一位處理）
        i++;      // 追蹤現在是第幾位
    }

    return result;
}

void find()
{
    max_N = 15;
    for (int i = 0; i < 11; i++) {
        N = 0;

        t = b * c;
        // printf("%ld", t);
        printf("b: %ld\n", b);
        decimal_to_binary(t);
        b = b * 64 + 63;
        max_N = (N < max_N) ? N : max_N;
        printf(" max: %d\n", max_N);
    }
}

int main() {
    double t;

    t = GetTime();
    find();
    t = (GetTime() - t);
    printf("%g\n", t);
    // pow(2, 2);

    // printf("Product of %u and %u = %u\n", x, y, product);
    return 0;
}