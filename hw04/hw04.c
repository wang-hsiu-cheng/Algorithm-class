// EE3410 HW04 Network Connectivity Problem
// 111061237, 王修呈
// 2025/03/24

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int V;                                           // vertex number
int E;                                           // edge number
int **graph;                                     // input graph 
int *pArray;                                     // represent disjoint sets
int *R;                                          // record root of each vertex
int Nrepeat = 100;                               // repeat time for average 
void readGraph(void);                            // read all inputs
double GetTime(void);                            // get local time in seconds
int Connect1(void);
int SetFind(int root);
void SetUnion(int root1, int root2);
int Connect2(void);
void WeightedUnion(int root1, int root2);
int Connect3(void);
int CollapsingFind(int root);
int Connect4(void);
int PathHalvingFind(int root);

void readGraph(void)
{
    int i;                                       // loop index
    
    scanf("%d", &V);                             // read vertex numbers
    scanf("%d", &E);                             // read edge numbers
    graph = (int **)malloc(E * sizeof(int *));   // allocate memory for graph
    pArray = (int *)malloc(V * sizeof(int));  // allocate vertex array's memory
    R = (int *)malloc(V * sizeof(int));       // allocate vertex array's memory
    for (i = 0; i < E; i++)
    {
        graph[i] = (int *)malloc(2 * sizeof(int));  // allocate vertex's memory
        scanf("%d %d", &graph[i][0], &graph[i][1]); // record the connection
    }
}

double GetTime(void)
{
    struct timeval tv;                            // time value

    gettimeofday(&tv, NULL);                      // get local time
    return tv.tv_sec + 1e-6 * tv.tv_usec;         // return time in seconds
}

int Connect1(void)
{
    int i;                                       // loop index
    int NetworkSets;                             // Number of disjoint sets
    int set1, set2;                              // root of two vertices

    NetworkSets = V;
    for (i = 0; i < V; i++)
        pArray[i] = -1;                      // initialize every vertex as root
    for (i = 0; i < E; i++)
    {
        set1 = SetFind(graph[i][0]);             // find the root
        set2 = SetFind(graph[i][1]);             // find the root
        if (set1 != set2) 
        {                                        // if they'rent connect
            NetworkSets--;        // decrease the number of sets & connect them
            SetUnion(set1, set2);
        }
    }
    for (i = 0; i < V; i++)
        R[i] = SetFind(i + 1);                // record the root of each vertex
    return NetworkSets;                          // return the number of sets
}

int SetFind(int root)
{
    while (pArray[root - 1] > 0)
        root = pArray[root - 1];                 // find the root
    return root;
}

void SetUnion(int root1, int root2)
{
    pArray[root1 - 1] = root2;                   // connect two disjoint sets
}

int Connect2(void)
{
    int i;                                       // loop index
    int NetworkSets;                             // Number of disjoint sets
    int set1, set2;                              // root of two vertices

    NetworkSets = V;
    for (i = 0; i < V; i++)
        pArray[i] = -1;                      // initialize every vertex as root
    for (i = 0; i < E; i++)
    {
        set1 = SetFind(graph[i][0]);             // find the root
        set2 = SetFind(graph[i][1]);             // find the root
        if (set1 != set2)
        {                                        // if they'rent connect
            NetworkSets--;                       // decrease the number of sets
            WeightedUnion(set1, set2);
        }
    }
    for (i = 0; i < V; i++)
        R[i] = SetFind(i + 1);           // record the root of each vertex
    return NetworkSets;                          // return the number of sets
}

void WeightedUnion(int root1, int root2)
{
    int temp;

    // temp records the total number of vertex under root1 & root2
    temp = pArray[root1 - 1] + pArray[root2 - 1];
    if (pArray[root1 - 1] > pArray[root2 - 1])   // root1 has fewere lements
    {
        pArray[root1 - 1] = root2;               // let root2 be the root
        pArray[root2 - 1] = temp;
    }
    else                                         // root2 has fewer elements
    {
        pArray[root2 - 1] = root1;               // let root1 be the root
        pArray[root1 - 1] = temp;
    }
}

int Connect3(void)
{
    int i;                                       // loop index
    int NetworkSets;                             // Number of disjoint sets
    int set1, set2;                              // root of two vertices

    NetworkSets = V;
    for (i = 0; i < V; i++)
        pArray[i] = -1;                      // initialize every vertex as root
    for (i = 0; i < E; i++)
    {
        set1 = CollapsingFind(graph[i][0]);      // find the root
        set2 = CollapsingFind(graph[i][1]);      // find the root
        if (set1 != set2)
        {                                        // if they'rent connect
            NetworkSets--;                       // decrease the number of sets
            WeightedUnion(set1, set2);
        }
    }
    for (i = 0; i < V; i++)
        R[i] = CollapsingFind(i + 1);            // record each vertex's root
    return NetworkSets;                          // return the number of sets
}

int CollapsingFind(int root)
{
    int r;
    int s;

    r = root;                                    // initialized r to root
    while (pArray[r - 1] > 0)
        r = pArray[r - 1];                       // find the root
    while (root != r)                        // collapse the vertex on the path
    {
        s = pArray[root - 1];
        pArray[root - 1] = r;
        root = s;
    }
    return r;
}

int Connect4(void)
{
    int i;                                       // loop index
    int NetworkSets;                             // Number of disjoint sets
    int set1, set2;                              // root of two vertices

    NetworkSets = V;
    for (i = 0; i < V; i++)
        pArray[i] = -1;                      // initialize every vertex as root
    for (i = 0; i < E; i++)
    {
        set1 = PathHalvingFind(graph[i][0]);     // find the root
        set2 = PathHalvingFind(graph[i][1]);     // find the root
        if (set1 != set2)
        {                                        // if they'rent connect
            NetworkSets--;                       // decrease the number of sets
            WeightedUnion(set1, set2);
        }
    }
    for (i = 0; i < V; i++)
        R[i] = PathHalvingFind(i + 1);           // record each vertex's root
    return NetworkSets;                          // return the number of sets
}

int PathHalvingFind(int root)
{
    int r;
    int s;

    if (pArray[root - 1] < 0)
        return root;                             // root1 is a root 
    r = root;                                    // r is the current node
    s = pArray[r - 1];                           // s is its parent
    while (pArray[s - 1] > 0)                    // s is not a root
    {
        pArray[r - 1] = pArray[s - 1];// reducting path length of r to the root
        r = s;                                   // move toward root
        s = pArray[s - 1];
    }
    return s;
}

int main(void)
{
    int i;                                       // loop index
    int networkSet1, networkSet2, networkSet3, networkSet4; // number of sets
    double t0, t1, t2, t3, t4;                   // time counter
    
    readGraph(); //Readanetworkfromstdin.
    printf("|V| = %d, |E| = %d\n", V, E);        // print out vertex and edges

    t0 = GetTime();                              // initialize time counter
    if (V < 100000)
        for (i = 0; i < Nrepeat; i++)
            networkSet1 = Connect1();
    t1 = GetTime();
    for (i = 0; i < Nrepeat; i++)
        networkSet2 = Connect2();
    t2 = GetTime();
    for (i = 0; i < Nrepeat; i++)
        networkSet3 = Connect3();
    t3 = GetTime();
    for (i = 0; i < Nrepeat; i++)
        networkSet4 = Connect4();
    t4 = GetTime();
    printf("Connect1 CPU time: %g, Disjoint sets: %d\n", 
        (t1 - t0) / Nrepeat, networkSet1);
    printf("Connect2 CPU time: %g, Disjoint sets: %d\n", 
        (t2 - t1) / Nrepeat, networkSet2);
    printf("Connect3 CPU time: %g, Disjoint sets: %d\n", 
        (t3 - t2) / Nrepeat, networkSet3);
    printf("Connect4 CPU time: %g, Disjoint sets: %d\n", 
        (t4 - t3) / Nrepeat, networkSet4);

    return 0;
}