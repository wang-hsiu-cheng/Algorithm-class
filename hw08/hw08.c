// EE3410 HW08 Minimum-Cost Spanning Tree
// 111061237, 王修呈
// 2025/04/28

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

typedef struct Graph                         // an edge
{
    int node_0;                              // vertex of edge
    int node_1;                              // vertex of edge
    double cost;                             // cost of edge
} Graph;

int V;                                       // input vertex numbers
int E;                                       // input edge numbers
int Nrepeat = 1000;
Graph *graph;                                // input graph data
Graph *copy;                                 // copy of the graph
Graph *temp;                                 // temp graph used in marge sort
int *pArray;                                 // record the root of vertices
int *tree_index;                             // record the edges of span tree
void readGraph(void);                        // read all inputs
void CopyGraph(Graph *g_old, Graph *g_new, int N); // copy the graph
double GetTime(void);                        // get local time in seconds
void MergeSort(Graph *list, int low, int high);
void Merge(Graph *list, int low, int mid, int high);
void InsertionSort(Graph *list, int low, int high);
double SpanningTree(Graph *list);

void readGraph(void)
{
    int i;                                   // loop index
    
    scanf("%d", &V);                         // read vertex numbers
    scanf("%d", &E);                         // read edge numbers
    graph = (Graph *)malloc(E * sizeof(Graph)); // allocate memory for graph
    copy = (Graph *)malloc(E * sizeof(Graph));
    temp = (Graph *)malloc(E * sizeof(Graph));
    pArray = (int *)malloc(V * sizeof(int)); // allocate vertex array's memory
    tree_index = (int *)malloc(V * sizeof(int));
    for (i = 0; i < E; i++)
    {
        // record the connection
        scanf("%d %d %lg", &graph[i].node_0, &graph[i].node_1, &graph[i].cost);
    }
}

void CopyGraph(Graph *g_old, Graph *g_new, int N)
{
    int i;

    for (i = 0; i < N; i++) {             // pass value of graph into new graph
        g_new[i].cost = g_old[i].cost;
        g_new[i].node_0 = g_old[i].node_0;
        g_new[i].node_1 = g_old[i].node_1;
    }
}

double GetTime(void)
{
    struct timeval tv;                       // _time value

    gettimeofday(&tv, NULL);                 // get local _time
    return tv.tv_sec + 1e-6 * tv.tv_usec;    // return _time in seconds
}

void InsertionSort(Graph *list, int low, int high) {
    int j, i;
    Graph item;

    for (j = low + 1; j < high + 1; j++) {   // check for every low < j <= high
        item = list[j];                  // compare list[i] and list[j], i < j.
        i = j - 1;
        // Make room for item = list[j]
        while ((i >= low) && (item.cost < list[i].cost)) {
            list[i + 1] = list[i];
            i--;
        }
        list[i + 1] = item;                  // store item.
    }
}

void Merge(Graph *list, int low, int mid, int high)
{
    int h, i, j, k, l;

    h = low;
    i = low;
    j = mid + 1;                             // initialize looping indices.

    while ((h <= mid) && (j <= high)) {      // store smaller one to temp[i].
        if (list[h].cost <= list[j].cost) {  // list[h] is smaller.
            temp[i] = list[h];
            h++;
        }
        else {                               // list[j] is smaller.
            temp[i] = list[j];
            j++;
        }
        i++;
    }
    if (h > mid)                             // list[j:high] remaining.
        for (k = j; k < high + 1; k++){
            temp[i] = list[k];
            i++;
        }
    else                                     // list[h:mid] remaining.
        for (k = h; k < mid + 1; k++) {
            temp[i] = list[k];
            i++;
        }
    for (k = low; k < high + 1; k++)
    {
        list[k] = temp[k];                   // copy temp to list.
    }
}

void MergeSort(Graph *list, int low, int high)
{
    int mid;

    if ((high - low) < 15)       // hen list is small, perfor minsertion sort.
        return InsertionSort(list, low, high);
    else{                     // for large list, divide-and-conquer merge sort.
        mid = (low + high) / 2;
        MergeSort(list, low, mid);
        MergeSort(list, mid + 1, high);
        Merge(list, low, mid, high);
    }
}

int SetFind(int root)
{
    while (pArray[root - 1] > 0)
        root = pArray[root - 1];             // find the root
    return root;
}

void SetUnion(int root1, int root2)
{
    pArray[root1 - 1] = root2;               // connect two disjoint sets
}

double SpanningTree(Graph *list)
{
    int i, j, k;                     // loop index
    double cost;                     // cost to be returned

    for (i = 0; i < V; i++){         // initialize the pArray and spanning tree
        pArray[i] = -1;
        tree_index[i] = 0;
    }
    i = 0;                                   // reset loop index to 0
    j = 0;
    k = 0;
    MergeSort(list, 0, E - 1);               // sort the graph
    while (i < E && j < V - 1) {  // loop before the edge of spanning reach V-1
        // add the smallest edge into spanning tree if it doesn't form loop
        if (SetFind(list[i].node_0) != SetFind(list[i].node_1)) {
            SetUnion(list[i].node_0, list[i].node_1);
            tree_index[j] = i;
            j++;                       // count number of edge in spanning tree
        }
        i++;                           // count total edges we've visited
    }
    cost = 0;                          // reset cost
    for (i = 0; i < V - 1; i++) {      // count the total cost
        cost += list[tree_index[i]].cost;
    }
    return cost;
}

int main(void)
{
    int i;                                   // loop index
    double minCost;                          // minimum cost of spanning tree
    double t;                                // time counter
    
    readGraph();                             // Read a network from stdin.

    t = GetTime();                           // initialize time counter
    for (i = 0; i < Nrepeat; i++) {          // repeat 1000 times
        CopyGraph(graph, copy, E);           // copy the graph
        minCost = SpanningTree(copy);        // MST algorithm
    }
    t = (GetTime() - t) / Nrepeat;           // end the counter
    printf("Minimum-cost spanning tree:\n");
    for (i = 0; i < V - 1; i++) {            // print out MST
        printf("  %d: <%d %d> %g\n",
            i + 1, copy[tree_index[i]].node_0, copy[tree_index[i]].node_1,
            copy[tree_index[i]].cost);
    }
    printf("|V| = %d |E| = %d\n", V, E);     // print out vertex and edges
    printf("Minimum cost: %g\n", minCost);   // print out minimum cost
    printf("CPU time: %g seconds\n", t);     // print out CPU time
    return 0;
}