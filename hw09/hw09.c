// EE3410 HW09 Encoding ASCII Texts
// 111061237, 王修呈
// 2025/05/09

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 128  // ASCII

typedef struct Node{
	int freq;
	char value;
	char *encode;
    int visited;
	struct Node *child_0;
	struct Node *child_1;
} Node;

typedef struct QueueNode {
    Node* treeNode;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode *front, *rear;
} Queue;

int N = 100;                                       // input size
int total_chars;
int char_num;
int bits;
char *data;                                 // input data
Node **characters;
Node *tree;

void readInput(void);                        // read all inputs
void expandData(char **data);
void swap(Node **a, Node **b);               // swap two strings
void BUHeapSort(Node **list, int n);         // buttom-up heap sort with strcmp
void BUHeapify(Node **list, int i, int n);
void Enqueue(Queue* q, Node* node);
Node* Dequeue(Queue* q);
int IsEmpty(Queue* q);
void BFS();
void Huffman(void);

void expandData(char **data)
{
    char *temp;
    temp = (char *)malloc(N * sizeof(char));
    temp = *data;
    N *= 2;
    *data = (char *)malloc(N * sizeof(char));
    strcpy(*data, temp);
}

void readInput(void)
{
    int i, j;
    char ch;
    int *f;

    i = 0;
    char_num = 0;
    j = 0;

    data = (char *)malloc(N * sizeof(char));
    f = (int *)malloc(MAX_CHARS * sizeof(int));
    while ((ch = getchar()) != EOF) {        // reading character
        data[i++] = ch;
        total_chars++;                       // count the char of the article
        f[ch]++;                             // record frquency
        if (i == N)
            expandData(&data);               // dynamic expand the array size
    }

    for (i = 0; i < MAX_CHARS; i++) {
        if (f[i]) char_num++;                // how many kinds of char is used
    }
    characters = (Node **)malloc(char_num * sizeof(Node *)); // allocate array
    for (i = 0; i < MAX_CHARS; i++) {
        if (f[i]) {
            characters[j] = (Node *)malloc(sizeof(Node)); // allocate node
            characters[j]->freq = f[i];
            characters[j]->value = i;
            characters[j]->visited = 0;
            characters[j]->encode = (char *)malloc(8 * sizeof(char));
            characters[j]->child_0 = NULL;
            characters[j]->child_1 = NULL;
            j++;
        }
    }
}

void swap(Node **a, Node **b) {
    Node *temp = *a;
    *a = *b;
    *b = temp;
}

void BUHeapSort(Node **list, int n)
{
    int i;

    for (i = (n - 1) / 2; i >= 0; i--) // Initialize A[0:n-1] to be a max heap.
    {
        BUHeapify(list, i, n);
    }
    for (i = n - 1; i >= 0; i--)
    {
        swap(&list[i], &list[0]);                 // Move maximum to the end.
        BUHeapify(list, 0, i);                    // Then make A[0:i] a max heap.
    }
}

void BUHeapify(Node **list, int i, int n)
{
    int j;
    Node *item;

    item = (Node *)malloc(sizeof(Node));
    j = i;                       // assuming A[i] is smaller than all the nodes
    while (2 * j + 2 < n)                         // j’s rchild exists
    {
        if (list[2 * j + 1]->freq > list[2 * j + 2]->freq)
            j = 2 * j + 1;                        // to lchild
        else
            j = 2 * j + 2;                        // to rchild
    }
    if (2 * j + 1 < n)
        j = 2 * j + 1;                            // lchild exists, move to it
    while (list[i]->freq > list[j]->freq)         // find place for A[i]
        j = (j + 1) / 2 - 1;
    item = list[j];                               // save A[j] to item
    list[j] = list[i];                            // move A[i] to A[j]
    while (j > i)                                 // move all nodes up along the path
    {
        j = (j + 1) / 2 - 1;                      // to parent
        swap(&list[j], &item);                    // swap with item
    }
}

void Enqueue(Queue* q, Node* node) {
    QueueNode* temp = malloc(sizeof(QueueNode));
    temp->treeNode = node;
    temp->next = NULL;
    if (q->rear)
        q->rear->next = temp;
    else
        q->front = temp;
    q->rear = temp;
}

Node* Dequeue(Queue* q) {
    if (!q->front) return NULL;
    QueueNode* temp = q->front;
    Node* node = temp->treeNode;
    q->front = temp->next;
    if (!q->front) q->rear = NULL;
    free(temp);
    return node;
}

int IsEmpty(Queue* q) {
    return q->front == NULL;
}

// print leaf nodes in level order
void BFS() {
    Queue q = {NULL, NULL};

    Enqueue(&q, characters[0]);
    characters[0]->encode = "";
    while (!IsEmpty(&q)) {
        Node* curr = Dequeue(&q);
        if (!curr->child_0 && !curr->child_1) {
            bits += (int)(strlen(curr->encode) * curr->freq); // calculate total bits
            // leaf node
            if (curr->value == ' ')
                printf(" %s: %s\n", "' '", curr->encode);
            else if (curr->value == '\n')
                printf(" %s: %s\n", "\"\\n\"", curr->encode);
            else
                printf(" %c: %s\n", curr->value, curr->encode);
        }
        if (curr->child_0) {                 // if has left child
            strcpy(curr->child_0->encode, curr->encode); // copy encode of parent
            strcat(curr->child_0->encode, "0");          // append new code
            Enqueue(&q, curr->child_0);
        }
        if (curr->child_1) {                 // if has right child
            strcpy(curr->child_1->encode, curr->encode); // copy encode of parent
            strcat(curr->child_1->encode, "1");          // append new code
            Enqueue(&q, curr->child_1);
        }
    }
}

void Huffman(void) {
    int i = char_num;

    while (i > 1) {
        BUHeapSort(characters, i);           // sort first i elements
        // take out 2 lowest freq nodes
        Node *left = characters[0];          
        swap(&characters[0], &characters[i - 1]);
        i--;                                 // move the node out of heap tree
        BUHeapSort(characters, i);
        Node *right = characters[0];
        swap(&characters[0], &characters[i - 1]);
        i--;                                 // move the node out of heap tree

        Node *new_node = malloc(sizeof(Node)); // create new internal node
        new_node->freq = left->freq + right->freq;
        new_node->value = ' ';               // internal node
        new_node->encode = (char *)malloc(8 * sizeof(char));
        new_node->visited = 0;
        new_node->child_0 = left;
        new_node->child_1 = right;

        characters[i] = new_node;            // insert back into array
        i++;                                 // add new node into heap tree
    }
    // characters[0] now contains the root of the tree
    BFS(characters[0]);  // pass the root
}

int main(void)
{
    int i, j;                               // loop index
    double ratio, bytes;

    readInput();                            // read input to data array
    BUHeapSort(characters, char_num);
    printf("Huffman coding:\n");
    Huffman();
    bytes = (bits % 8) ? (bits / 8 + 1) : (bits / 8);
    ratio = bytes / total_chars;
    printf("Number of Chars read: %d\n", total_chars);
    printf(" Huffman Coding needs %d bits, %g bytes\n", bits, bytes);
    printf(" Ratio = %g %%\n", ratio);
    return 0;
}