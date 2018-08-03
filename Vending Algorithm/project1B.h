//
//  proj1.h
//  Project 1
//
//  Created by Francis Tengey on 1/25/18.
//  Copyright © 2018 Francis Tengey. All rights reserved.
//

#ifndef project1B_h
#define project1B_h

typedef struct _Node {
    int arrivalTime;
    int serviceTime;
    int priority;
    struct _Node *next;
} Node;

typedef struct _Server{
    int free;
    int busy;
    int arrivTime;
    int servTime;
    struct _Server *next;
} Server;

void mode1(double arrival1, double arrival2, double service_rate, int num_tasks);

void mode2(char* filename);
#endif /* proj1_h */
