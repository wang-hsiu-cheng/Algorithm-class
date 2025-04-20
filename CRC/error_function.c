#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int C_ascending[33] = {1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1};
int C_decending[33] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0 ,1 ,1 ,1};
int E_ascending[5870] = {0};
int T[5870] = {0};
int R[32] = {0};
int *temp;
int *B;
int baseLen_b = 6000;
int len_b = 5837;
int min_N = 15;
int current_N = 0;

void assign()
{
    B = (int *)malloc(6000 * sizeof(int));
    temp = (int *)malloc(6000 * sizeof(int));
    for (int i = 0; i < 5837; i++)
        B[i] = 1;
    for (int i = 5838; i < 6000; i++)
        B[i] = 0;
}
void multiply()
{
    int carryBit;
    int bit;
    int len_0 = 3;
    int len_1 = 4;
    int multy0[3] = {1, 1, 1};
    int multy1[4] = {1, 0, 1, 1};
    int multy_temp[9] = {0};
    int count = 0;
    for (int j = 0; j < len_0; j++) {
        for (int k = 0; k < len_1; k++) {
            carryBit = ((multy1[k] && multy0[j]) + multy_temp[k + j]) / 2;
            multy_temp[k + j] = ((multy1[k] && multy0[j]) + multy_temp[k + j]) % 2;
            int l = 1;
            while(carryBit) {
                bit = (multy_temp[k + j + l] + carryBit) % 2;
                carryBit = (multy_temp[k + j + l] + carryBit) / 2;
                multy_temp[k + j + l] = bit;
                l++;
            }
        }
        for (int i = 0; i < 9; i++)
            printf("%d", multy_temp[i]);
        printf("j=%d",j);
        printf("\n");
        count += multy_temp[j];
    }
    for (int i = len_0; i < len_0 + len_1; i++)
        count += multy_temp[i];
    printf("%d\n", count);
}

void adder()
{
    int carryBit;
    int bit;
    int i = 0;
    carryBit = (1 + B[i]) / 2;
    B[i] = (1 + B[i]) % 2;
    while (carryBit) {
        i++;
        bit = (B[i] + carryBit) % 2;
        carryBit = (B[i] + carryBit) / 2;
        B[i] = bit;
    }
    for (int j = 0; j <= i; j++)
        printf("%d", B[j]);
    printf("\n");
}

void find()
{
    int carryBit;
    int bit;
    for (int i = 0; i < 100000000; i++) {
        for (int j = 0; j < len_b; j++) {
            for (int k = 0; k < 33; k++) {
                carryBit = ((C_ascending[k] && B[j]) + temp[k + j]) / 2;
                temp[k + j] = ((C_ascending[k] && B[j]) + temp[k + j]) % 2;
                int l = 1;
                while(carryBit) {
                    bit = (temp[k + j + l] + carryBit) % 2;
                    carryBit = (temp[k + j + l] + carryBit) / 2;
                    temp[k + j + l] = bit;
                    l++;
                }
            }
            current_N += temp[j];
        }
        for (int j = len_b; j < len_b + 33; j++)
            current_N += temp[j];
        min_N = (current_N < min_N) ? current_N : min_N;
        printf("len_b=%d, i=%d, min_N=%d\n", len_b, i, min_N);
        current_N = 0;
        // len_b += baseLen_b;
        // B = (int*) realloc(B, len_b * sizeof(int));
        // for (int j = len_b - baseLen_b; j < len_b; j++)
        //     B[j] = 1;
        int j = 0;
        carryBit = (1 + B[j]) / 2;
        B[j] = (1 + B[j]) % 2;
        while (carryBit && j < 5999) {
            j++;
            bit = (B[j] + carryBit) % 2;
            carryBit = (B[j] + carryBit) / 2;
            B[j] = bit;
        }
        len_b = (j+1 > len_b) ? j+1 : len_b;
    }
}

void answer()
{
    E_ascending[0] = 1;
    E_ascending[34] = 1;
    E_ascending[48] = 1;
    E_ascending[5869] = 1;
    // for (int i = 0; i < 5870; i++)
    //     printf("%d", E_ascending[i]);
    for (int i = 0; i < (5870-32); i++) {
        if (E_ascending[i] == 1) {
            T[i] = 1;
            printf("%d", T[i]);
            for (int j = 0; j < 33; j++) {
                E_ascending[i + j] = E_ascending[i + j] ^ C_decending[j];
            }
        }
        else
            printf("%d", T[i]);
    }
    printf("\n");
    
    for (int i = 5870 - 32; i < 5870; i++) {
        R[i - (5870 -32)] = E_ascending[i];
        printf("%d", E_ascending[i]);
    }
    printf("\n");
}

int main(void)
{
    assign();
    // adder();
    find();
    // answer();
    // multiply();
    return 0;
}
