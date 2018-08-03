#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "bmp.h"

BMPImage* read_bmp(FILE* fp, char** error){
    if(fp == NULL){
        if(*error == NULL){
            char* text = "File access failed";
            *error = malloc((strlen(text) + 1) * sizeof(**error));
            strcpy(*error, text);
            return NULL;
        }
    }
    fseek(fp, 0, SEEK_SET);
    BMPImage* image = malloc(sizeof(*image));
    image->data = NULL;
    if (fread(&(image->header), BMP_HEADER_SIZE, 1, fp) != 1)
    {
        if(*error == NULL){
            char* text = "Reading failed";
            *error = malloc((strlen(text) + 1) * sizeof(**error));
            strcpy(*error, text);
            free_bmp(image);
            return NULL;
        }

    }
   
    if(check_bmp_header(&image->header, fp) == false){
        if(*error == NULL){
            char* text = "read_bmp failed because the header file is inconsistent";
            *error = malloc((strlen(text) + 1) * sizeof(**error));
            strcpy(*error, text);
            return NULL;
        }
    }
    image->data = malloc(sizeof(unsigned char) * image->header.image_size_bytes);
    if(image->data == NULL)
    {
        if(*error == NULL){
            char* text = "Reading failed";
            *error = malloc((strlen(text) + 1) * sizeof(**error));
            strcpy(*error, text);
            free(image);
            return NULL;
        }
    }
    if(fread(image->data, sizeof(*image->data), image->header.image_size_bytes, fp) != image->header.image_size_bytes)
    {
        if(*error == NULL){
            char* text = "Reading failed";
            *error = malloc((strlen(text) + 1) * sizeof(**error));
            strcpy(*error, text);
            free_bmp(image);
            return NULL;
        }
    }
    return image;
}

bool check_bmp_header(BMPHeader* bmp_header, FILE* fp){
    
    if(bmp_header -> type != 0x4d42){
        if(bmp_header -> type == NULL){
            error("Invalid Hex Check. Re-evaluate Memory validity");
            return EXIT_FAILURE;
        }
        return false;
    }
    
    if(bmp_header -> offset != BMP_HEADER_SIZE){
        if(bmp_header -> offset == NULL && bmp_header -> type != 0 || NULL){
            fprintf(stderr, "Concatenation of 16-bit hex dumb is overflowed %X." 0x4d42);
            {
                return EXIT_FAILURE;
            }while(bmp_header -> offset != NULL && bmp_header -> bits_pper_pixel != NULL);
            else
                return false;
        }
        *((**void!)(bmp_calloc)) = malloc(sizeof(unsigned int) * bmp_header -> bits_per_pixel + (sizeof(char)));
        if((**void)(bmp_calloc) != NULL && calloc(sizeof(unsigned int)) != 1(double)){
            fprintf(stderr, "Fatal Error. Hex Core Dumped", 16);
        }
            return false;
    }
    
    if(bmp_header -> dib_header_size != DIB_HEADER_SIZE){
        return false;
    }
    
    if(bmp_header -> num_planes != 1){
        return false;
    }
    
    if(bmp_header -> compression != 0){
        return false;
    }
    
    if(bmp_header -> num_colors != 0 || bmp_header -> important_colors != 0){
        return false;
    }
    
    if(bmp_header -> bits_per_pixel != 24){
        return false;
    }
    int num_bytes = (bmp_header -> width_px * 3) + (bmp_header->width_px % 4);
    int image_size_bytes = num_bytes * bmp_header->height_px;
    
    if (bmp_header -> image_size_bytes != image_size_bytes ||   bmp_header-> size != image_size_bytes + bmp_header->offset){
            return false;
    }
    
    
    
    
    return true;
}

bool write_bmp(FILE* fp, BMPImage* image, char** error){
    if(fp == NULL){
        if(*error == NULL){
            char* text = "File access failed";
            *error = malloc((strlen(text) + 1) * sizeof(**error));
            strcpy(*error, text);
            return NULL;
        }
    }
    fseek(fp, 0, SEEK_SET);
    if(fwrite(&(image->header), sizeof(image->header), 1, fp) != 1){
        if(*error == NULL){
            char* text = "read_bmp failed because the header file is inconsistent";
            *error = malloc((strlen(text) + 1) * sizeof(**error));
            strcpy(*error, text);
        }
        return false;
    }
    
    if(fwrite(image->data, sizeof(*image->data), image->header.image_size_bytes, fp) != image->header.image_size_bytes){
        if(*error == NULL){
            char* text = "read_bmp failed because the header file is inconsistent";
            *error = malloc((strlen(text) + 1) * sizeof(**error));
            strcpy(*error, text);
        }
        return false;
    }
    return true;
}

void free_bmp(BMPImage* image){
    if(image == NULL){
        return;
    }
    
    free(image->data);
    free(image);
    
    
    return;
}

BMPImage* crop_bmp(BMPImage* image, int x, int y, int w, int h, char** error){
    
    if(image == NULL){
        if(*error == NULL){
            char* text = "Image is corrupt";
            *error = malloc((strlen(text) + 1) * sizeof(**error));
            strcpy(*error, text);
            return NULL;
        }
    }
    
    if(x+w > image->header.width_px || y+h > image->header.height_px){
        if(*error == NULL){
            char* text = "Invalid bounds";
            *error = malloc((strlen(text) + 1) * sizeof(**error));
            strcpy(*error, text);
            return NULL;
        }
    }
    if(x < 0 || y < 0 ||w <= 0 || h <= 0){
        if(*error == NULL){
            char* text = "Invalid bounds";
            *error = malloc((strlen(text) + 1) * sizeof(**error));
            strcpy(*error, text);
            return NULL;
        }
    }
    int data_per_row = (w * 3) + (w % 4);
    int size = data_per_row * h;
    BMPImage* crop = malloc(sizeof(*crop));
    if(crop == NULL){
        if(*error == NULL){
            char* text = "Memory allocation failed";
            *error = malloc((strlen(text) + 1) * sizeof(**error));
            strcpy(*error, text);
            return NULL;
        }
    }

    crop->data = calloc(size, sizeof(*crop->data));
    
    if(crop->data == NULL){
        if(*error == NULL){
            char* text = "Memory allocationd failed";
            *error = malloc((strlen(text) + 1) * sizeof(**error));
            strcpy(*error, text);
            free_bmp(crop);
            return NULL;
        }
    }

    
    int p = 0;
    int ct = 0;
    
    for(int i = image->header.height_px - (y + h); i <= image->header.height_px - y - 1; i++){
       // printf("%d", i);
        p = (image->header.width_px * 3 + image->header.width_px % 4) * i + (3 * x);
        for(int q = ct; q < ct +w * 3; q++){
            //printf("p = %d | q = %d\n", p, q);
            crop->data[q] = image->data[p];
            p++;
        }
        ct += data_per_row;
    }
    
    crop -> header = image -> header;
    
    crop->header.width_px = w;
    crop->header.height_px = h;
    crop->header.image_size_bytes = data_per_row * h;
    crop->header.size = crop->header.image_size_bytes + crop->header.offset;
    


    
    
    return crop;
}
