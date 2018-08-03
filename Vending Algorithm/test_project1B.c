#include <stdio.h>
#include <stdlib.h>
#include "project1B.h"

int main(int argc, char **argv){
    int test = 0;
    if(argc == 2){
        test++;
    }
    if(argc == 5){
        test++;
    }
    
    if(test == 0){
        return EXIT_FAILURE;
    }
    //putchar(67);
    if(argc == 2){
        char* filename = argv[1];
        if(filename == NULL){
            return EXIT_FAILURE;
        }
        mode2(filename);
    }
    
    else if(argc == 5){
        double arrivalrate1 = atof(argv[1]);
        double arrivalrate2 = atof(argv[2]);
        double servicerate = atof(argv[3]);
        int numTasks = atoi(argv[4]);


        mode1(arrivalrate1, arrivalrate2, servicerate, numTasks);
    }
    
    return EXIT_SUCCESS;
}
