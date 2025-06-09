// EE3410 Project 千字文
// 111061237, 王修呈
// 2025/05/21

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

char **data, **A;                                // graph data
char **duplicate, **temp, **hash_table;
int duplicateNum;
int charactersNum = 1000;
int repeat = 100;
void PrintData(char **data);                // print all chinese words
void ReadData(void);                        // read all inputs
void CopyData(char **dataNew, char **dataOld); // copy chinese words
double GetTime(void);                       // get local time in seconds
void MergeSort(char **list, int low, int high);
void Merge(char **list, int low, int mid, int high);
void BruteForce(char **data);               
void SortBeforeSearch(char **data);
void HashTable(char **data);

double GetTime(void)
{
    struct timeval tv;                            // time value

    gettimeofday(&tv, NULL);                      // get local time
    return tv.tv_sec + 1e-6 * tv.tv_usec;         // return time in seconds
}

void ReadData(void)
{
    int i, j;
    int ch, count;
    char buffer[4];  // UTF-8 max is 4 bytes
    char c;

    // allocate memory for data
    data = (char**)malloc(charactersNum * sizeof(char *));
    A = (char**)malloc(charactersNum * sizeof(char *));
    temp = (char**)malloc(charactersNum * sizeof(char *));
    hash_table = (char**)malloc(charactersNum * sizeof(char *));
    duplicate = (char**)malloc(charactersNum * sizeof(char *));
    for (i = 0; i < charactersNum; i++) {        // loop all characters
        data[i] = (char*)malloc(4 * sizeof(char)); // allocate memory for 1 word
        A[i] = (char*)malloc(4 * sizeof(char));
        temp[i] = (char*)malloc(4 * sizeof(char));
        hash_table[i] = (char*)malloc(4 * sizeof(char));
        duplicate[i] = (char*)malloc(4 * sizeof(char));
    }
    count = 0;
    j = 0;
    while ((ch = getchar()) != EOF) {            // read until EOF
        c = (char)ch;

        // detect start of a multibyte UTF-8 character
        if ((c & 0xF0) == 0xE0) {  // likely 3-byte Chinese character
            buffer[0] = c;
            buffer[1] = getchar();
            buffer[2] = getchar();
            buffer[3] = '\0';
            count++;
            if ((count-3) % 5 != 0 && count > 3) { // skip title and punctuation
                strcpy(data[j], buffer);
                j++;
            }
        }
    }
}

void PrintData( char **data)
{
    int i, j;

    for (i = 0; i < charactersNum / 8; i++) {
        for (j = 0; j < 8; j++)
            printf("%s ", data[i * 8 + j]);
        printf("\n");
    }
}

void CopyData(char **dataNew, char **dataOld)
{
    int i;

    for (i = 0; i < charactersNum; i++) {
        strcpy(dataNew[i], dataOld[i]);
    }
}

void BruteForce(char **data)
{
    int i, j;
    char target[4];

    duplicateNum = 0;
    int count = 0;
    for (i = 0; i < charactersNum; i++) {        // loop all characters
        while (data[i][0] == '\0') {             // skip visited strings
            i++;
        }
        strcpy(target, data[i]);
        for (j = 0; j < i; j++) {                // loop all word before target
            if (data[j] == NULL) continue;
            if (strcmp(target, data[j]) == 0) {  // found duplicate
                strcpy(duplicate[duplicateNum], target);
                duplicateNum++;
                data[i][0] = '\0';              // mark as visited
                data[j][0] = '\0';              // mark as visited
            }
        }
    }
}

void MergeSort( char **list, int low, int high)
{
    int mid;

    if (low < high) {
        mid = (low + high) / 2;
        MergeSort(list, low, mid);
        MergeSort(list, mid + 1, high);
        Merge(list, low, mid, high);
    }
}

void Merge( char **list, int low, int mid, int high)
{
    int h, i, j, k;

    h = low;
    i = low;
    j = mid + 1;                                 // initialize looping indices

    while ((h <= mid) && (j <= high)) {          // store smaller to temp[i]
        if (strcmp(list[h], list[j]) <= 0) {     // list[h] is smaller.
            temp[i] = list[h];
            h++;
        }
        else {                                   // list[j] is smaller.
            temp[i] = list[j];
            j++;
        }
        i++;
    }
    if (h > mid)                                 // list[j:high] remaining.
        for (k = j; k < high + 1; k++){
            temp[i] = list[k];
            i++;
        }
    else                                         // temp[h:mid] remaining.
        for (k = h; k < mid + 1; k++) {
            temp[i] = list[k];
            i++;
        }
    for (k = low; k < high + 1; k++)
    {
        list[k] = temp[k];                       // copy temp to list.
    }
}

void SortBeforeSearch(char **data)
{
    int i;
    int canCheck;

    canCheck = 1;                     // check if the string is already checked
    duplicateNum = 0;                            // initialize duplicateNum
    MergeSort(data, 0, charactersNum - 1);       // sort the data
    for (i = 0; i < charactersNum - 1; i++) {    // loop all characters
        // found duplicate
        if (strcmp(data[i], data[i + 1]) == 0 && data[i][0] != '\0' && canCheck) {
            duplicate[duplicateNum] = data[i];
            duplicateNum++;
            canCheck = 0;
        }
        // if the string is not duplicate with the previous one, 
        // continue to check in the next word
        else if (!canCheck){
            canCheck = 1;
        }
    }
}

int hash(char *data)
{
    int i, hash_value;

    hash_value = 0;
    for(i = 0; i < 3; i++){                  // hash the characters
        hash_value += data[i];                  
        hash_value = abs(hash_value * data[i]) % charactersNum;
    }
    return hash_value;
}

void HashTable(char **data)
{
    int i, hash_value;

    duplicateNum = 0;
    for (i = 0; i < charactersNum; i++) {        // loop all characters
        if (data[i] == NULL) i++;                // skip empty strings

        hash_value = hash(data[i]);              // hash the string
        // check for duplicates
        while (hash_table[hash_value][0] != '\0' &&
               strcmp(hash_table[hash_value], data[i]) != 0) {
            hash_value = (hash_value + 1) % charactersNum; // linear probing
        }
        if (strcmp(hash_table[hash_value], data[i]) == 0) { // found duplicate
            duplicate[duplicateNum] = data[i];
            duplicateNum++;
        } else {
            strcpy(hash_table[hash_value], data[i]); // insert into hash table
        }
    }
}

void ResetResult(void)
{
    int i;

    for (i = 0; i < duplicateNum; i++) {         // reset duplicate array
        duplicate[i] = "";
    }
    duplicateNum = 0;
}

int main(void)
{
    int i, j;                                    // loop index
    double t1, t2, t3;                           // time counter

    ReadData();                                  // read input to data
    
    t1 = GetTime();
    for (i = 0; i < repeat; i++) {               // repeat the test
        CopyData(A, data);
        BruteForce(A);
        ResetResult();
    }
    t1 = (GetTime() - t1) / repeat;              // calculate average time
    // print out the result of bruce force method
    // printf("%d duplicates found\n", duplicateNum);
    // if (duplicateNum) {
    //     printf("duplicated words: ");
    //     for (i = 0; i < duplicateNum; i++)
    //         printf("%s", duplicate[i]);
    //     printf("\n");
    // }

    t2 = GetTime();
    for (i = 0; i < repeat; i++) {               // repeat the test
        CopyData(A, data);
        SortBeforeSearch(A);
        ResetResult();
    }
    t2 = (GetTime() - t2) / repeat;              // calculate average time
    // print out the result of sort before searchgggf method
    // printf("%d duplicates found\n", duplicateNum);
    // if (duplicateNum) {
    //     printf("duplicated words: ");
    //     for (i = 0; i < duplicateNum; i++)
    //         printf("%s", duplicate[i]);
    //     printf("\n");
    // }

    t3 = GetTime();
    for (i = 0; i < repeat; i++) {               // repeat the test
        CopyData(A, data);
        HashTable(A);
        ResetResult();
    }
    t3 = (GetTime() - t3) / repeat;              // calculate average time
    if (duplicateNum > 1)
        printf("%d duplicates found\n", duplicateNum);
    else 
        printf("%d duplicate found\n", duplicateNum);
    if (duplicateNum) {
        printf("duplicated words: ");
        for (i = 0; i < duplicateNum; i++)
            printf("%s", duplicate[i]);
        printf("\n");
    }

    printf("CPU time for O(N^2) algorithm: %g seconds\n", t1);
    printf("CPU time for O(N log(N)) algorithm: %g seconds\n", t2);
    printf("CPU time for O(N) algorithm: %g seconds\n", t3);

    return 0;
}