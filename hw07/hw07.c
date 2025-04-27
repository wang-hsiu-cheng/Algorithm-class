// EE3410 HW07 Grouping Friends
// 111061237, 王修呈
// 2025/04/21

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

typedef struct Node                          // node structure
{
    char *name;                              // chinese name
    int index;                               // index of the adjcency list
    struct Node *tail;                       // tail of the linked list
    struct Node *next;                       // next node
} Node;

int V;                                       // input vertex numbers
int E;                                       // input edge numbers
Node *data_l;                                // graph data
Node *data_l_transpose;                      // transpose graph data
int *visited;                                // if the subgraph visited 
int *p, *d, *f;                              // 3 array used in DFS
int _time;                                    
int current_scc = 0;                         // subgroup numbers
int *scc;                                    // subgroup sets
void readInput(void);                        // read all inputs
double GetTime(void);                        // get local time in seconds
void DFS(int v, Node *data);
void DFS_Call_G(Node *data);
void DFS_Call_GT(Node *data);
void SCC();

void readInput(void)
{
    int i, j;                                // loop index
    char *oper, *name0, *name1;
    int name0_index, name1_index;
    Node *node;
    Node *temp;
    
    scanf("%d", &V);                         // read input vertex numbers
    scanf("%d", &E);                         // read input edges numbers
    data_l = (Node *)malloc(V * sizeof(Node));// allocate memory for graph
    // allocate memory for transpose graph
    data_l_transpose = (Node *)malloc(V * sizeof(Node)); 
    name0 = (char*)malloc(10 * sizeof(char)); // allocate memory for input name
    name1 = (char*)malloc(10 * sizeof(char)); // allocate memory for input name
    // allocate memory for "->" between names
    oper = (char*)malloc(2 * sizeof(char));
    for (i = 0; i < V; i++)         // initialize each vertex of adjacnecy list
    {
        data_l[i].name = (char*)malloc(10 * sizeof(char));
        data_l[i].next = NULL;
        data_l[i].tail = NULL;
        data_l_transpose[i].name = (char*)malloc(10 * sizeof(char));
        data_l_transpose[i].next = NULL;
        data_l_transpose[i].tail = NULL;
        scanf("%s", data_l[i].name);          // read vertices
        data_l_transpose[i].name = data_l[i].name;
    }
    for (i = 0; i < E; i++)                   // add edges
    {
        scanf("%s", name0);
        scanf("%s", oper);
        scanf("%s", name1);
        for (j = 0; j < V; j++) {     // find the index of the name in adj list
            if (strcmp(data_l[j].name, name0) == 0)
                name0_index = j;
            if (strcmp(data_l[j].name, name1) == 0)
                name1_index = j;
        }
        node = (Node *)malloc(sizeof(Node)); // allocate memory of new node
        node->index = name1_index;
        node->name = data_l[name1_index].name;
        node->next = NULL;
        node->tail = NULL;
        // add the new node directly to the tail of the linked list
        if (data_l[name0_index].tail != NULL) {
            data_l[name0_index].tail->next = node;
            data_l[name0_index].tail = node;
        }
        else {
            data_l[name0_index].next = node;
            data_l[name0_index].tail = node;
        }
        // do the same things for transpose graph
        node = (Node *)malloc(sizeof(Node));
        node->index = name0_index;
        node->name = data_l_transpose[name0_index].name;
        node->next = NULL;
        if (data_l_transpose[name1_index].tail != NULL) {
            data_l_transpose[name1_index].tail->next = node;
            data_l_transpose[name1_index].tail = node;
        }
        else {
            data_l_transpose[name1_index].next = node;
            data_l_transpose[name1_index].tail = node;
        }
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

void DFS(int v, Node *data)
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
            DFS(temp->index, data);
        }
        temp = temp->next;
    }
    visited[v] = 2;
    t = _time + 1;
    f[v] = _time;
}

void DFS_Call_G(Node *data)
{
    int v;

    for (v = 0; v < V; v++) { // Initialize to not visited and no predecessor
        visited[v] = 0;
        p[v] = 0;
        d[v] = 0;
        f[v] = 0;
    }
    _time = 0;                               // Global variable to track _time
    for (v = 0; v < V; v++) {                // To handle forest case.
        if(visited[v] == 0)
            DFS(v, data);
    }
}

void DFS_Call_GT(Node *data)
{
    int i;                                   // loop index

    for (i = 0; i < V; i++) { // Initialize to not visited and no predecessor
        visited[i] = 0;
        p[i] = 0;
        d[i] = 0;
        f[i] = 0;
    }
    current_scc = 0;
    _time = 0;                               // Global variable to track times
    for (i = 0; i < V; i++) {                // To handle forest case
        if(visited[f[i]-1] == 0) {
            DFS(i, data);
            current_scc++;
        }
    }
}

void SCC()
{
    int i;                                   // loop index

    scc = (int *)malloc(V *sizeof(int));
    visited = (int *)malloc(V *sizeof(int));
    p = (int *)malloc(V *sizeof(int));
    d = (int *)malloc(V *sizeof(int));
    f = (int *)malloc(V *sizeof(int));

    DFS_Call_G(data_l);                      // Perform DFS to get array f[1:V]
    for (i = 0; i < V; i++)
        scc[i] = 0;
    DFS_Call_GT(data_l_transpose);           // Perform DFS on GT
}

int main(void)
{
    int i, j;                               // loop index
    double t;                               // time counter

    readInput();                            // read input to data array
    printf("N = %d M = %d ", V, E);                  // print out input size

    t = GetTime();                              // initialize time counter
    SCC();
    t = GetTime() - t;

    printf("Subgroup = %d CPU time = %g\n", current_scc, t);
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