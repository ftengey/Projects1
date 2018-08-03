#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char* argv[]){
    FILE *fp = fopen("6x6_24bit.bmp", "r");
    
    if(fp == NULL){
        fprintf(stderr, "File could not open");
        return EXIT_SUCCESS;
    }
    
    FILE *fp2 = fopen("output1.bmp", "w");
    
    if(fp2 == NULL){
        fprintf(stderr, "File could not open");
        return EXIT_SUCCESS;
    }

    char* error = NULL;
    
    BMPImage* image = read_bmp(fp, &error);
    if(error!=NULL)
    {
        fprintf(stderr,error);
        return EXIT_SUCCESS;
    }
    //printf("%ld", sizeof(*(image->data)));
    // //////////
    int x = 2;
    int y = 3;
    int w = 4;
    int h = 2;
    
    BMPImage * crop = crop_bmp(image, x, y, w, h, &error);
    //printf("[%c]", crop->data);
    if(error!=NULL)
    {
        fprintf(stderr,error);
        return EXIT_SUCCESS;
    }
    // //////////
    
    if(!(write_bmp(fp2, crop, &error))){
        if(error!=NULL)
        {
            
            fprintf(stderr,error);
            return EXIT_SUCCESS;
        }
        free(error);
        return EXIT_SUCCESS;
    }
    
    
    
    free_bmp(crop);
    free_bmp(image);
    
    
    if(error != NULL){
        free(error);
        return EXIT_SUCCESS;
    }
    free(error);
    
    fclose(fp);
    fclose(fp2);
    
    
    return EXIT_SUCCESS;
}
