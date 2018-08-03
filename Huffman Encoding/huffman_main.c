#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"


int main(int argc, char* argv[])
{
    if(argc != 5){
        fprintf(stderr, "Invalid number of inputs");
        return EXIT_FAILURE;
    }
    
    FILE *fp = fopen(argv[1], "r");
    
    if(fp == NULL){
        fprintf(stderr, "File could not open");
        return EXIT_FAILURE;
    }
    
    FILE *fp1 = fopen(argv[2], "w");
    
    if(fp1 == NULL){
        fprintf(stderr, "File could not open");
        return EXIT_FAILURE;
    }
    
    FILE *fp2 = fopen(argv[3], "w");
     
     if(fp2 == NULL){
     fprintf(stderr, "File could not open");
     return EXIT_FAILURE;
     }
    
    FILE *fp3 = fopen(argv[4], "w");
    
    if(fp3 == NULL){
        fprintf(stderr, "File could not open");
        return EXIT_FAILURE;
    }
    
    printNum(fp, fp1);
    
    printHuff(fp, fp3, fp2);
    
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    
    
    return EXIT_SUCCESS;
}
