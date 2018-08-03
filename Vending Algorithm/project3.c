#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct _Node {
    int supplierID;
    char *supplierCity;
    int suppCity_count;
    char *partCity;
    char *jobCity;
    int partID;
    int jobID;
    int quantity; 
    int qtyAvg;
    int colorCode;
    struct _Node *next;
} Node;

void getData(FILE * fp1, FILE * fp2, FILE * fp3, FILE * fp4, FILE * fp5);
Node * _enqueue(Node **pq, int supplierID, int partID, int jobID, int quantity, char name[100], char name1[100], char name2[100], int code, FILE * fp1, FILE * fp2, FILE * , FILE * fp4);
char* checkSupplier(int supplier, int *size, FILE * fp);
char* checkPart(int part, int *code, FILE * fp);
char* checkJob(int part, FILE * fp);
int cmp_fn(Node * current, Node * node);
void output1(Node **vendor, double avg);
int partition (int arr[], int low, int high);
void quickSort(int arr[], int low, int high);
void swap(int* a, int* b);
void output2(Node **vendor);
void output3(Node **vendor, FILE * fp3);
void free_node(Node *pq);
int checkTotal(int supplierID, FILE * fp4);
double calcAvg(FILE * fp1, FILE * fp4);



int main(int argc, char** argv){

    if(argc != 5){
        fprintf(stderr, "Invalid number of input arguments");
        return EXIT_FAILURE;
    }
    FILE * fp1 = fopen(argv[1], "r");
    if(fp1 == NULL){
        fprintf(stderr, "File cannot be opened");
    }
    FILE * fp2 = fopen(argv[2], "r");
    if (fp2 == NULL){
        fprintf(stderr, "File cannot be opened");
    }
    FILE * fp3 = fopen(argv[3], "r");
    if (fp3 == NULL){
        fprintf(stderr, "File cannot be opened");
    }
    FILE * fp4 = fopen(argv[4], "r");
    FILE * fp5 = fopen(argv[4], "r");
    if (fp4 == NULL){
        fprintf(stderr, "File cannot be opened");
    }

    getData(fp1, fp2, fp3, fp4, fp5);

    return EXIT_SUCCESS;
}

void getData(FILE *fp1, FILE *fp2, FILE *fp3, FILE *fp4, FILE * fp5){
    char sid[100];
    char sid1[100];
    char pid1[100];
    char sid2[100];
    char code[100];
    char pid2[100];
    char sid3[100];
    char pid3[100];
    int supplier;
    int supplier1;
    int supplier2;
    int supplier3;
    int job;
    int part;
    int quantity;
    char pid[100];
    char jid[100];
    char qty[100];
    char test[100];
    int ch;
    int code1;
    int lines = 0;
    int lines1 = 0;
    int lines2 = 0;
    int lines3 = 0;
    int sumQty = 0;
    double avg;
    // char * name1 = malloc(sizeof(char) * 1000);
    // char * name2 = malloc(sizeof(char) * 1000);
    // char * name3 = malloc(sizeof(char) * 1000);


    if(fp1 == NULL){
        return;
    }
    if(fp2 == NULL){
        return;
    }
    if(fp3 == NULL){
        return;
    }
    if(fp4 == NULL){
        return;
    }
    while ((ch = fgetc(fp4)) != EOF)
    {
      if (ch == '\n')
    lines++;
    }
    while ((ch = fgetc(fp1)) != EOF)
    {
      if (ch == '\n')
    lines1++;
    }
    while ((ch = fgetc(fp2)) != EOF)
    {
      if (ch == '\n')
    lines2++;
    }
    while ((ch = fgetc(fp3)) != EOF)
    {
      if (ch == '\n')
    lines3++;
    }
    
    fseek(fp4, 0L, SEEK_SET);
    Node * vendor = NULL;
    //printf("%d", lines);
    for(int i = 0; i < lines+1; i++) /*just assuming number of entries here to demonstrate problem*/
    {       
        fscanf(fp4, " %s,%[^,],%[^,],%[^\n]", sid, pid, jid, qty);
        supplier = atoi(sid);
        part = atoi(pid);
        job = atoi(jid);
        quantity = atoi(qty);
        printf("F[%d],[%d],[%d],[%d]\n", supplier1, part, job, quantity);
        fseek(fp1, 0L, SEEK_SET);
        for(int i = 0; i < lines1+1; i++){
            fscanf(fp1, "%[^,],%[^\n]", sid1, pid1);
            supplier1 = atoi(sid1);
            if(supplier1 == supplier){
                break;
                
            }
        }
        fseek(fp2, 0L, SEEK_SET);
        for(int i = 0; i < lines2+1; i++){
            fscanf(fp2, "%[^,]%[^,],%[^\n]", sid2, code, pid2);
            supplier2 = atoi(sid2);
            code1 = atoi(code);
            if(supplier2 == part){
                break;
                i = lines1+1;
            }
        }
        fseek(fp3, 0L, SEEK_SET);
        for(int i = 0; i < lines3+1; i++){
            fscanf(fp3, "%[^,]%[^\n]", sid3, pid3);
            supplier3 = atoi(sid3);
            if(supplier3 == job){
                break;
                i = lines1+1;
            }
        }
        //printf("%d %d %d %d\n", supplier, part, job, quantity);
        sumQty += quantity;
        _enqueue(&vendor, supplier, part, job, quantity, pid1, pid2, pid3, code1, fp1, fp2, fp3, fp5);
    }
    
    fseek(fp4, 0L, SEEK_SET);
    fseek(fp1, 0L, SEEK_SET);
    fseek(fp3, 0L, SEEK_SET);
    avg = calcAvg(fp1, fp4);
    fseek(fp1, 0L, SEEK_SET);
    output1(&vendor, avg);
    output2(&vendor);
    output3(&vendor, fp1);
    free_node(vendor);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    return;
} 

Node * _enqueue(Node **pq, int supplierID, int partID, int jobID, int quantity,char name[100], char name1[100], char name2[100], int code, FILE * fp1, FILE * fp2, FILE * fp3, FILE * fp4){    
    Node *node = malloc(sizeof(Node));
    int size = 1;
    
    if(node == NULL){
        return NULL;
    }
     FILE * file = fopen("check.txt", "w");
     fprintf(file, "%s - %s\n", name, name2);

    node->supplierCity = malloc(sizeof(char) * 1000);
    node->partCity = malloc(sizeof(char) * 1000);
    node->jobCity = malloc(sizeof(char) * 1000);

    node->supplierID = supplierID;
    node->partID = partID;
    node->jobID = jobID;
    node->quantity = quantity;
    for(int i = 0;i < sizeof(name);i++){
        node->supplierCity[i] = name[i];
    }
    //node->supplierCity = name;
    node->suppCity_count = size;
    for(int i = 0;i < strlen(name1);i++){
        node->partCity[i] = name1[i];
    }
    //node->partCity = name1;
    node->colorCode = code;
    for(int i = 0;i < sizeof(name2);i++){
        node->jobCity[i] = name2[i];
    }
    //node->jobCity = name2;
    node->qtyAvg = checkTotal(supplierID, fp4);
    
    if(*pq == NULL){
        *pq = node;
        return node;
    }
    Node temp;
    
    temp.next = *pq;
    Node *prev = &temp;
    
    Node *current = *pq;
    
    while(current != NULL){
        if(cmp_fn(current, node) > 0){
            break;
        }
        prev = current;
        current = current -> next;
        
    }
    prev -> next = node;
    node -> next = current;
    *pq = temp.next;
    
    return node;
}
int checkTotal(int supplierID, FILE * fp4){
    int ch;
    int lines = 0;
    char sid[10];
    int supplier;
    int job;
    int part;
    int quantity;
    char pid[10];
    char jid[10];
    char qty[10];
    int total_qty = 0;

    fseek(fp4, 0L, SEEK_SET);
    while ((ch = fgetc(fp4)) != EOF)
    {
      if (ch == '\n')
    lines++;
    }
    fseek(fp4, 0L, SEEK_SET);

    for(int i = 0; i < lines+1; i++) /*just assuming number of entries here to demonstrate problem*/
    {
        fscanf(fp4, "%[^,]%[^,],%[^,],%[^\n]", sid, pid, jid, qty);
        supplier = atoi(sid);
        part = atoi(pid);
        job = atoi(jid);
        quantity = atoi(qty);
        if(supplier == supplierID){
            total_qty += quantity;
        }
    }
    return total_qty;
}

double calcAvg(FILE * fp1, FILE * fp4){
    int ch;
    int ct = 0;
    int ct1 = 0;
    int lines = 0;
    int lines1 = 0;
    char sid[10];
    int supplier;
    int supplier1;
    int job;
    int part;
    int quantity;
    char pid[10];
    char jid[10];
    char qty[10];
    int total_qty = 0;
    double avg;
    double avg1;
    int check;
    double total2 = 0.0;

    fseek(fp4, 0L, SEEK_SET);
    fseek(fp1, 0L, SEEK_SET);
    while ((ch = fgetc(fp4)) != EOF)
    {
      if (ch == '\n')
        lines++;
    }
    while ((ch = fgetc(fp1)) != EOF)
    {
      if (ch == '\n')
        lines1++;
    }
    fseek(fp4, 0L, SEEK_SET);
    fseek(fp1, 0L, SEEK_SET);
    
    for(int i = 0; i < lines1+1; i++) /*just assuming number of entries here to demonstrate problem*/
    {

        fscanf(fp1, " %[^,]%*[^\n]", sid, pid);
        //printf("[ %s ]", sid);
        supplier = atoi(sid);
        //printf("[ %s ]", pid);
        fseek(fp4, 0, SEEK_SET);
        for(int j = 0; j < lines+1; j++){
            fscanf(fp4, "%[^,]%[^,],%[^,],%[^\n]", sid, pid, jid, qty);
            supplier1 = atoi(sid);
            //printf("[-%d-]", supplier1);
            quantity = atoi(qty);
            if(supplier == supplier1){
                total_qty += quantity;
                ct1++;
                check = 1;
            }
        }
        if(check == 1){
            ct++;
        }
        check = 0;
        if(ct1 != 0){
            avg = (double)total_qty / (double)ct1;
            printf("[%lf]", avg);
        }
        else{
            avg = 0;
        }
        total_qty = 0;
        total2 += avg;
        ct1 = 0;
    }

    avg1 =  total2 / (double)ct;
    printf("%lf, %d", avg1, ct);

    return avg1;
}

char* checkSupplier(int supplier, int *size, FILE * fp){
    int ct = 0;
    int ct2 = 0;
    int c;
    int i = 0;
    int size1;
    int check = 0;
    char* name; 

  
    while((c = fgetc(fp)) != EOF && check != 1){
        if(ct == 0 && c != ',' && c != '\n'){
            if(c == supplier){
                ct = 1;
            }
        }
        if(ct == 1 && c == ','){
            ct = 2;
        }
        if(ct == 2 && c != ',' && c != '\n'){
            name[i] = c;
            i++;
        }
        if(ct == 2 && c == '\n'){
            check = 1;
            name[i] = '\0';
        }
    }
    //printf("[%s]",name);
    return name;
}

char* checkPart(int part, int *code, FILE * fp){
    int ct = 0;
    int c;
    int i = 0;
    int check = 0;
    char* name = malloc(sizeof(char) * 12);
    while((c = fgetc(fp)) != EOF && check != 1){
        if(ct == 0 && c != ',' && c != '\n'){
            if(c == part){
                ct = 1;
            }
        }
        if(ct == 1 && c == ','){
            ct = 2;
        }
        if(ct == 2 && c != ',' && c != '\n'){
            *code = c;
            ct = 3;
        }
        if(ct == 3 && c == ','){
            ct = 4;
        }
        if(ct == 4 && c != ',' && c != '\n'){
            name[i] = c;
            i++;
        }
        if(ct == 4 && c == '\n'){
            check = 1;
            name[i] = '\0';
        }
    }
    return name;
}

char* checkJob(int part, FILE * fp){
    int c;
    int ct = 0;
    int i = 0;
    int check = 0;
    char* name;

    while(((c = fgetc(fp)) != EOF) && check != 1){
        if(ct == 0 && c != ',' && c != '\n'){
            if(c == part){
                ct = 1;
            }
        }
        if(ct == 1 && c == ','){
            ct = 2;
        }
        if(ct == 2 && c != ',' && c != '\n'){
            name[i] = c;
            i++;
        }
        if(ct == 2 && c == '\n'){
            check = 1;
            name[i] = '\0';
        }

    }
    return name;
}

int cmp_fn(Node * current, Node * node){
    if(current->supplierID <= node->supplierID){
        return 1;
    }
    else{
        return 0;
    }
}

void output1(Node **vendor, double avg){
    FILE * fp = fopen("output1.txt", "w");
    
    Node temp;
    temp.next = *vendor;
    int* output = malloc(sizeof(int) * 1000);
    //int *output;
    int i = 0;

    Node* temp1 = *vendor;
    while(temp1 != NULL){
        if(temp1->qtyAvg > avg){
            output[i] = temp1->supplierID;
            i++;
        }
        temp1 = temp1->next;
    }
    quickSort(output, 0, i - 1);

    for(int ct = 0; ct < i; ct++){
        fprintf(fp, "%d\n", output[ct]);
    }
    
    *vendor = temp.next;

    free(output);
    fclose(fp);

    return;
}

void output2(Node **vendor){
    FILE * fp = fopen("output2.txt", "w");
   
    
    Node temp;
    temp.next = *vendor;
    int* output = malloc(sizeof(int) * 1000);
    int i = 0;

    Node* temp1 = *vendor;
    while(temp1 != NULL){
        
            if(strcmp(temp1->supplierCity, temp1->jobCity) == 0 && temp1->colorCode != 2){
                
                output[i] = temp1->supplierID;
                i++;
            }
        temp1 = temp1->next;
    }
    quickSort(output, 0, i - 1);

    for(int ct = 0; ct < i; ct++){
        fprintf(fp, "%d\n", output[ct]);
    }
    
    *vendor = temp.next;

    free(output);
    fclose(fp);
//fclose(fp1);
    
    return;
}

void output3(Node **vendor, FILE * fp3){
    FILE * fp = fopen("output3.txt", "w");
    char pid3[100];
    char sid3[100];
    int lines3 = 0;
    int ch = 0;
    int supplier3;
    
    Node temp;
    temp.next = *vendor;
    int* output = malloc(sizeof(int) * 6000);
    //char **output1 = malloc(sizeof(char*) * 120);
    int i = 0;

    Node* temp1 = *vendor;
    while(temp1 != NULL){
            
            if(strcmp(temp1->supplierCity, temp1->partCity) == 0 && strcmp(temp1->supplierCity, temp1->jobCity) != 0){
                output[i] = temp1->jobID;
                //strcpy(output1[i], temp1->jobCity);
                i++;
            }
        temp1 = temp1->next;
    }
    quickSort(output, 0, i - 1);

    while ((ch = fgetc(fp3)) != EOF)
    {
      if (ch == '\n')
    lines3++;
    }

    for(int ct = 0; ct < i; ct++){
        //fprintf(fp, "%d,%s/n", output[ct], output1);
        fseek(fp3, 0L, SEEK_SET);
        for(int i = 0; i < lines3+1; i++){
            fscanf(fp3, "%[^,],%[^\n]", sid3, pid3);
            supplier3 = atoi(sid3);
            if(supplier3 == output[ct]){
                break;
            }
        }
        fprintf(fp, "%d,%s\n", output[ct], pid3);
    }
    
    *vendor = temp.next;

    free(output);
    //free(output1);
    fclose(fp);
    
    return;
}

int partition (int arr[], int low, int high){
    int pivot = arr[high];
    int i = (low - 1);
 
    for (int j = low; j <= high- 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high){
    if (low < high)
    {
        int pi = partition(arr, low, high);
 
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void swap(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;
}

void free_node(Node *pq){
    
    if (pq == NULL) {
        return;
    }
    Node *temp = pq;
    while(temp != NULL){
        Node *end = temp -> next;
        free(temp);
        temp = end;
    }
    return;
}