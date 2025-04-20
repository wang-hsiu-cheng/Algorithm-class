// EE3410 HW07 Grouping Friends
// 111061237, 王修呈
// 2025/04/20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

typedef struct Node 
{
    char *name;
    int index;
    struct Node *next;
} Node;

int V;                                       // input size
int E;                                       // input size
// Node *data;                                 // input data
Node *data_l;
Node *data_l_transpose;
// Matrix *data_m;
// Matrix *data_m_transpose;
int *visited;
int *p, *d, *f;
int _time;
int current_scc = 0;
int *scc;
char **A;                                    // array to be sorted
void readInput(void);                        // read all inputs
double GetTime(void);                        // get local _time in seconds
void DFS_d(int v, Node *data);
void DFS_Call(Node *data);
void DFS_Call_1(Node *data);

typedef struct matrix 
{
    char *name;
    int index;
    int *childs;
} Matrix;

void readInput(void)
{
    int i, j; // loop index
    char *oper, *name0, *name1;
    int name0_index, name1_index;
    Node *node;
    Node *temp;
    
    scanf("%d", &V);                              // read input size
    scanf("%d", &E);                              // read input size
    data_l = (Node *)malloc(V * sizeof(Node));   // allocate memory for data
    data_l_transpose = (Node *)malloc(V * sizeof(Node));
    // data_m = (Matrix *)malloc(V * sizeof(Matrix));
    name0 = (char*)malloc(10 * sizeof(char));
    name1 = (char*)malloc(10 * sizeof(char));
    oper = (char*)malloc(2 * sizeof(char));
    for (i = 0; i < V; i++)
    {
        // data_m[i].name = (char*)malloc(10 * sizeof(char));
        // data_m[i].childs = (int*)malloc(V * sizeof(int));
        data_l[i].name = (char*)malloc(10 * sizeof(char));
        data_l[i].next = NULL;

        data_l_transpose[i].name = (char*)malloc(10 * sizeof(char));
        data_l_transpose[i].next = NULL;
        scanf("%s", data_l[i].name);                     // read data
        data_l_transpose[i].name = data_l[i].name;
    }
    for (i = 0; i < E; i++)
    {
        scanf("%s", name0);
        scanf("%s", oper);
        scanf("%s", name1);
        for (j = 0; j < V; j++) {
            if (strcmp(data_l[j].name, name0) == 0)
                name0_index = j;
            if (strcmp(data_l[j].name, name1) == 0)
                name1_index = j;
        }
        // printf("%s %s", data_l[name0_index].name, data_l[name1_index].name);
        node = (Node *)malloc(sizeof(Node));
        node->index = name1_index;
        node->name = data_l[name1_index].name;
        node->next = NULL;
        temp = &data_l[name0_index];
        // printf("temp= %s. ", temp->name);
        while (temp->next != NULL) {
            // printf("next= %s. ", temp->next->name);
            temp = temp->next;
        }
        temp->next = node;
        // printf("%s %s %s\n", data_l[name0_index].next->name, temp->name, temp->next->name);

        node = (Node *)malloc(sizeof(Node));
        node->index = name0_index;
        node->name = data_l_transpose[name0_index].name;
        node->next = NULL;
        temp = &data_l_transpose[name1_index];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = node;

        // data_m[name0_index].childs[name1_index] = 1;
    }
}

void PrintGraph(Node *data)
{
    int i, j;
    Node *temp;

    for (i = 0; i < V; i++) {
        printf("node %d %s: ", i, data[i].name);
        temp = data[i].next;
        j = 0;
        while (temp != NULL && j < V) {
            printf("%s, ", temp->name);
            temp = temp->next;
            j++;
        }
        printf("\n");
    }
}

double GetTime(void)
{
    struct timeval tv;                            // _time value

    gettimeofday(&tv, NULL);                      // get local _time
    return tv.tv_sec + 1e-6 * tv.tv_usec;         // return _time in seconds
}

void DFS_d(int v, Node *data)
{
    Node *temp;
    int t;

    visited[v] = 1;
    scc[v] = current_scc;
    _time++;
    d[v] = _time;
    temp = (&data[v])->next;
    while (temp != NULL) {
        if (visited[temp->index] == 0) {
            p[temp->index] = v;
            DFS_d(temp->index, data);
        }
        temp = temp->next;
    }
    visited[v] = 2;
    t = _time + 1;
    f[v] = _time;
}

void DFS_Call(Node *data)
{
    int v;

    for (v = 0; v < V; v++) { //Initialize to not visited and no predecessor.
        visited[v] = 0;
        p[v] = 0;
        d[v] = 0;
        f[v] = 0;
    }
    _time = 0; // Global variable to track _time.
    for (v = 0; v < V; v++) { // To handle forest case.
        if(visited[v] == 0)
            DFS_d(v, data);
    }
}

void DFS_Call_1(Node *data)
{
    int v;

    for (v = 0; v < V; v++) { //Initialize to not visited and no predecessor.
        visited[v] = 0;
        p[v] = 0;
        d[v] = 0;
        f[v] = 0;
    }
    current_scc = 0;
    _time = 0; // Global variable to track _time.
    for (v = 0; v < V; v++) { // To handle forest case.
        if(visited[f[v]-1] == 0) {
            DFS_d(v, data);
            current_scc++;
        }
    }
}

void SCC()
{
    int i;

    scc = (int *)malloc(V *sizeof(int));
    visited = (int *)malloc(V *sizeof(int));
    p = (int *)malloc(V *sizeof(int));
    d = (int *)malloc(V *sizeof(int));
    f = (int *)malloc(V *sizeof(int));

    DFS_Call(data_l); // Perform DFS to get array f[1:V].
    for (i = 0; i < V; i++)
        scc[i] = 0;
    // for (i = 0; i < V; i++)
    //     printf("%d ", f[i]);
    DFS_Call_1(data_l_transpose); //PerformDFSonGT.
    // for (i = 0; i < V; i++)
    //     printf("%d ", scc[i]);
}

int main(void)
{
    int i, j;                               // loop index
    double t;                               // _time counter

    readInput();                            // read input to data array
    printf("N = %d M = %d ", V, E);                  // print out input size

    // PrintGraph(data_l);
    // PrintGraph(data_l_transpose);

    t = GetTime();                              // initialize _time counter
    SCC();
    t = GetTime() - t;
    printf("Subgroup = %d CPU _time = %g\n", current_scc, t);
    printf("Number of subgroups: %d\n", current_scc);
    for (i = 0; i < current_scc; i++) {
        printf("  Subgroup %d:", i);
        for (j = 0; j < V; j++) {
            if (scc[j] == i)
                printf(" %s", data_l[j].name);
        }
        printf("\n");
    }
    return 0;
}