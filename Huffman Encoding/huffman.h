#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>

typedef struct Node{
    char info;
    int frequency;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct Heap
{
    int size;
    int max;
    struct Node **array;
} Heap;

void switchNode(Node** inp1, Node** inp2);

void conversion(Heap* heap, int index);

void buildHeap(Heap* heap);

void placeHeap(Heap* heap, Node* node);

Node* create_tree(FILE * fp, FILE *fp1);

Heap* finishHeap(char *info, int *frequency, int size);

void printNum(FILE *fp, FILE *fp1);

void printHuff(FILE *fp, FILE *fp1, FILE *fp2);

void sorting(int *frequency,char *array, int size);

#endif
