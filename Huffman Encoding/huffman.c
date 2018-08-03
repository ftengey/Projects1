#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

#define MAX 256

Node* createNode(char info, int frequency){
    Node* node = (Node*) malloc(sizeof(Node));
    node -> left = NULL;
    node -> right = NULL;
    node -> info = info;
    node -> frequency = frequency;
    return node;
}

Node* removeMin(Heap* heap)
{
    Node* temp = heap -> array[0];
    for(int i = 0; i < heap -> size -1; i++){
        heap -> array[i] = heap -> array[i + 1];
    }
    --heap -> size;

    return temp;
}

Heap* createHeap(int max){
    Heap* heap = (Heap*) malloc(sizeof(Heap));
    heap->size = 0;
    heap -> max = max;
    heap -> array = (Node**) malloc(heap -> max * sizeof(Node*));

    return heap;
}

void placeHeap(Heap* heap, Node* node){

    ++heap -> size;
    int i = (heap -> size) - 1;

    while(i && node -> frequency < heap -> array[(i - 1) / 2] -> frequency){
        heap -> array[i] = heap -> array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->array[i] = node;

    return;
}


Heap* finishHeap(char *info, int *frequency, int size){
    Heap* heap = createHeap(size);
    for (int i = 0; i < size; ++i){
        heap -> array[i] = createNode(info[i], frequency[i]);
        printf("%c", heap -> array[i] -> info);
    }
    printf("%d", size);
    heap -> size = size;

    return heap;
}

void buildHeap(Heap* heap){
    int x = (heap -> size) - 1;
    for (int i = (x - 1) / 2; i >= 0; --i){
        conversion(heap, i);
    }
}

int countComp(Node* head, int *array, int up){
    int ct = 0;
    if (head->left){
        array[up] = 0;
        countComp(head ->left, array, up + 1);
    }

    if (head -> right){
        array[up] = 1;
        countComp(head->right, array, up + 1);
    }

    if (!(head->left) && !(head->right)){
        for (int i = 0; i < up; ++i){
            ct++;
        }
    }

    return ct;
}

void genCodes(Node* head, int up, FILE *fp1){

    if (head->left){
        genCodes(head ->left, up + 1, fp1);
    }

    if (head -> right){
        genCodes(head->right, up + 1, fp1);
    }
    if (!(head->left) && !(head->right)){
        fputc('1', fp1);
        fputc(head -> info, fp1);
    }

}


void printCodes(Node* head, int *array, int up, FILE *fp1){



    if (head->left){
        array[up] = 0;
        printCodes(head ->left, array, up + 1, fp1);
    }


    if (head -> right){
        array[up] = 1;
        printCodes(head->right, array, up + 1, fp1);
    }

    if (!(head->left) && !(head->right)){
        fprintf(fp1, "%c:", head -> info);

        for (int i = 0; i < up; ++i){
            fprintf(fp1, "%ld", (long int) array[i]);
        }
        fprintf(fp1, "\n");

    }


    return;
}

void compress_to_file(FILE *fp, FILE *fp1){

    Node *head = create_tree(fp, fp1);
    int array[100];
    int up = 0;
    unsigned int size = countComp(head, array, up);

    fseek(fp1, 0, SEEK_SET);
    fputc(size, fp1);
    fputc('\n', fp1);
    up = 0;

    genCodes(head, up, fp1);

    return;
}

void conversion(Heap* heap, int index){
    int least = index;
    int right = (2 * index) + 2;
    int left = (2 * index) + 1;


    if(left < heap->size && heap->array[left]->frequency < heap->array[least]->frequency){
        least = left;
    }
    if(right < heap->size && heap->array[right]->frequency < heap->array[least]->frequency){
        least = right;
    }
    if(least != index){
        switchNode(&heap->array[least], &heap->array[index]);
        conversion(heap, least);
    }

    return;
}
void printNum(FILE *fp, FILE *fp1){
    int c = 0;
    int* array= calloc(MAX, sizeof(int));
    fseek(fp, 0 , SEEK_SET);
    while((c = fgetc(fp)) != EOF){
        for(int i = 0; i < MAX ; i++){
            if(c == i){
                array[i]++;
            }
        }
    }
    for(int i = 0; i < MAX; i++){
        fprintf(fp1, "%ld\n", (long int )array[i]);
    }

    free(array);

    return;
}

void printHuff(FILE *fp, FILE *fp1, FILE *fp2){
    Node *head = create_tree(fp, fp2);
    int *array = malloc(sizeof(int) * 100);
    int up = 0;
    printCodes(head, array, up, fp1);

    free(head);

    return;
}
void insert(Heap* heap, Node* node){

    int index = 0;
    for(int i = 0; i < heap -> size && index == 0; i++){
        if(node -> frequency < heap -> array[i] -> frequency){
            index = i;
        }
    }
    if(index == 0){
        index = heap -> size;
    }
    for(int j = heap -> size; j > index; j--){
        heap -> array[j] = heap -> array[j-1];
    }
    heap -> size++;
    heap -> array[index] = node;

    return;
}
Node* create_tree(FILE * fp, FILE * fp2){

    fseek(fp, 0, SEEK_END);
    int size = (int) ftell(fp);
    int c = 0;
    int j = 0;
    int i = 0;
    int *frequency = malloc(sizeof(int) * size);
    char *newarray = malloc(sizeof(char) * size);
    char *array = malloc(sizeof(char) * size);
    Node *left;
    Node *right;
    Node *up;

    fseek(fp, 0, SEEK_SET);
    while((c = fgetc(fp)) != EOF){
        array[i] = c;
        i++;
    }

    int* array1= calloc(MAX, sizeof(int));
    fseek(fp, 0 , SEEK_SET);
    while((c = fgetc(fp)) != EOF){
        for(int x = 0; x < MAX ; x++){
            if(c == x){
                array1[x]++;
            }
        }
    }
    for(int ct = 0; ct < MAX; ct++){
      if(array1[ct] != 0 && (char) ct != '\n'){
          newarray[j] = ct;
          frequency[j] = array1[ct];
          j++;
      }
    }
     sorting(frequency, newarray, j);

    size = j;
    free(array1);


    int temp = 0;
    char temp1 = 0;

    for (int a = 1; a < size; a++) {
        for (int b = 0; b < size - a; b++) {
            if (frequency[b] > frequency[b + 1]) {
                temp = frequency[b];
                temp1 = newarray[b];
                frequency[b] = frequency[b + 1];
                newarray[b] = newarray[b + 1];
                frequency[b + 1] = temp;
                newarray[b + 1] = temp1;
            }
        }
    }

    for(int u = 0; u < size;u++){
      fprintf(fp2, "%c:%ld->", newarray[u], (long int) frequency[u]);

    }
    fprintf(fp2, "NULL\n");


    Heap* heap = finishHeap(newarray, frequency, size);



    while(!(heap -> size == 1)){
        printf("**");
        for(int g = 0; g < heap -> size; g++){
            printf("%c|%d", heap -> array[g] -> info, heap -> array[g] -> frequency);
        }
        printf("**");
        left = removeMin(heap);
        right = removeMin(heap);
        up = createNode('$', left -> frequency + right -> frequency);
        up -> left = left;
        printf("Left-%c%d ", up -> left -> info, up -> left -> frequency);
        up -> right = right;
        printf("Right-%c%d ", up -> right -> info, up -> right -> frequency);
        insert(heap, up);
    }

    free(frequency);
    free(array);
    free(newarray);

    return removeMin(heap);
}


void switchNode(Node** inp1, Node** inp2){
    Node* temp = *inp1;
    *inp1 = *inp2;
    *inp2 = temp;

    return;
}

void sorting(int *array,char *array2, int size) {

    int temp = 0;
    char temp1;

    for (int i = 1; i < size; i++) {
        for (int j = 0; j < size - i; j++) {
            if (array[j] > array[j + 1]) {
                temp = array[j];
                temp1 = array2[j];
                array[j] = array[j + 1];
                array2[j] = array2[j + 1];
                array[j + 1] = temp;
                array2[j + 1] = temp1;
            }
        }
    }

    return;
}
