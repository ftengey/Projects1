#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "project1B.h"
#define P0 0
#define P1 1

double _constantRate(double inputRate);

Node *_enqueue(Node **pq, int service_time, int arrival_time, int priority);
Node *_enqueue1(Node **pq, int service_time, int arrival_time, int priority);
Node *_dequeue(Node **pq);
void _pq_push(Node **pq, int service_time, int arrival_time, int priority);
int _cmp_fn(Node *current, Node *node);
int _cmp_fn1(Node *current, Node *node);
double _getArrivaltime(double service_rate, double arrivalrate);
double _getServicetime(double service_rate, double arrivalrate);
int _getSubtasks();
void free_node(Node *pq);

void mode1(double arrivalrate1, double arrivalrate2, double service_rate, int num_tasks)
{
    if (arrivalrate1 + arrivalrate2 >= service_rate)
    {
        fprintf(stderr, "Invalid wait times");
        return;
    }
    if (num_tasks <= 0)
    {
        fprintf(stderr, "Number of tasks must be greater than 0");
        return;
    }

    Node *queue = NULL;
    Node *wait_queue = NULL;

    for (int i = 0; i < num_tasks; i++)
    {
        int arrival_time = _getArrivaltime(service_rate, arrivalrate1);
        int service_time = _getServicetime(service_rate, arrivalrate1);
        if (service_time == 0)
        {
            service_time++;
        }

        _enqueue(&queue, service_time, arrival_time, P0);
    }

    for (int j = 0; j < num_tasks; j++)
    {
        int arrival_time1 = _getArrivaltime(service_rate, arrivalrate2);
        int service_time1 = _getServicetime(service_rate, arrivalrate2);
        if (service_time1 == 0)
        {
            service_time1++;
        }
        _enqueue(&queue, service_time1, arrival_time1, P1);
    }
    double temp = queue->arrivalTime;
    double time_elapsed = temp;
    int *waitLine = malloc(sizeof(int) * num_tasks * 2);
    int sumWait = 0;
    double totalWait1 = 0.0;
    double totalWait2 = 0;
    double idle_time = 0;
    int ct = 0;
    int i = 0;
    double temp1 = 0.0;
    int y = 0;
    while (queue != NULL)
    {
        if (queue->priority != 3)
        {
            time_elapsed += queue->serviceTime;
            temp1 = queue->serviceTime;
            if (queue->next != NULL)
            {
                queue->priority = 3;
                queue = queue->next;
            }
            if (queue->next == NULL)
            {
                break;
            }
            while (queue->arrivalTime <= time_elapsed && queue->priority != 3 && y != 1)
            {
                ct++;
                if (queue->priority != 3)
                {
                    _enqueue1(&wait_queue, queue->serviceTime, queue->arrivalTime, queue->priority);
                }
                queue->priority = 3;
                if (queue->next != NULL)
                {
                    queue = queue->next;
                }
                else
                {
                    y = 1;
                }
            }
            while (wait_queue != NULL)
            {
                if (wait_queue->priority != 3)
                {
                    _pq_push(&queue, wait_queue->serviceTime, wait_queue->arrivalTime, wait_queue->priority);
                }
                if (wait_queue->priority == 0)
                {
                    totalWait1 += (time_elapsed - wait_queue->arrivalTime);
                }
                if (wait_queue->priority == 1)
                {
                    totalWait2 += (time_elapsed - wait_queue->arrivalTime);
                }
                wait_queue->priority = 3;
                wait_queue = wait_queue->next;
            }
            
            waitLine[i] = ct;
            if (waitLine[i] == 0)
            {
                idle_time += temp1;
            }
        
            sumWait += ct;
            ct = 0;
            i++;
        }
        else
        {
            if (queue->next != NULL)
            {
                queue = queue->next;
            }
            if (queue->next == NULL)
            {
                break;
            }
        }
    }

    time_elapsed -= temp;
    double avg_wait1 = totalWait1 / (double)num_tasks;
    double avg_wait2 = totalWait2 / (double)num_tasks;
    double avg_queue_length = sumWait / ((double)num_tasks * 2);
    double CPU_util = (time_elapsed - idle_time) / time_elapsed;
    printf("TE - %lf", time_elapsed);
    printf("IT - %lf", idle_time);

    FILE *fp1 = fopen("output1", "w");

    fprintf(fp1, "%.6lf\n", avg_wait1);
    fprintf(fp1, "%.6lf\n", avg_wait2);
    fprintf(fp1, "%.6lf\n", avg_queue_length);
    fprintf(fp1, "%.6lf\n", CPU_util);

    fclose(fp1);

    free(waitLine);
    free_node(wait_queue);
    free_node(queue);

    return;
}

void mode2(char *filename)
{

    FILE *fp = fopen(filename, "r");

    Node *queue = NULL;
    char c = 0;
    int ct1 = 0;
    int p0 = 0;
    int p1 = 0;
    int x = 0;

    double arrival_time = 0.0;
    double service_time = 0.0;
    int priority = 0;

    fseek(fp, 0, SEEK_SET);

    while ((c = fgetc(fp)) != EOF)
    {
        if (x == 0 && c != ' ')
        {
            arrival_time = atoi(&c);
            x = 1;
        }
        else if (x == 1 && c != ' ')
        {
            priority = atoi(&c);
            if (priority == 0)
            {
                p0++;
            }
            else
            {
                p1++;
            }
            x = 2;
        }
        else if (x == 2 && c != ' ' && c != '\n')
        {
            service_time = atoi(&c);
            x = 3;
        }
        else if (c == '\n')
        {
            x = 0;
            ct1++;
            _enqueue(&queue, service_time, arrival_time, priority);
        }
    }

    fclose(fp);

    int num_tasks = p0 + p1;
    Node *wait_queue = NULL;
    double temp = queue->arrivalTime;
    double time_elapsed = temp;
    int *waitLine = malloc(sizeof(int) * num_tasks);
    int sumWait = 0;
    double totalWait1 = 0.0;
    double totalWait2 = 0;
    double idle_time = 0;
    int ct = 0;
    int i = 0;
    double temp1 = 0.0;
    int y = 0;

    while (queue != NULL)
    {
        if (queue->priority != 3)
        {
            time_elapsed += queue->serviceTime;
            temp1 = queue->serviceTime;
            if (queue->next != NULL)
            {
                queue->priority = 3;
                queue = queue->next;
            }
            if (queue->next == NULL)
            {
                break;
            }
            time_elapsed += queue->arrivalTime;
            temp1 = queue->serviceTime;

            while (queue->arrivalTime <= time_elapsed && queue->priority != 3 && y != 1)
            {
                ct++;
                if (queue->priority != 3)
                {
                    _enqueue1(&wait_queue, queue->serviceTime, queue->arrivalTime, queue->priority);
                }
                queue->priority = 3;
                if (queue->next != NULL)
                {
                    queue = queue->next;
                }
                else
                {
                    y = 1;
                }
            }
            while (wait_queue != NULL)
            {
                if (wait_queue->priority != 3)
                {
                    _pq_push(&queue, wait_queue->serviceTime, wait_queue->arrivalTime, wait_queue->priority);
                }
                if (wait_queue->priority == 0)
                {
                    totalWait1 += (time_elapsed - wait_queue->arrivalTime);
                }
                if (wait_queue->priority == 1)
                {
                    totalWait2 += (time_elapsed - wait_queue->arrivalTime);
                }
                wait_queue->priority = 3;
                wait_queue = wait_queue->next;
            }
            waitLine[i] = ct;
            if (waitLine[i] == 0)
            {
                idle_time += temp1;
            }

            sumWait += ct;
            ct = 0;
            i++;
        }
        else
        {
            if (queue->next != NULL)
            {
                queue = queue->next;
            }
            if (queue->next == NULL)
            {
                break;
            }
        }
    }
    
    time_elapsed -= temp;
    double avg_wait1 = totalWait1 / (double)p0;
    double avg_wait2 = totalWait2 / (double)p1;
    double avg_queue_length = sumWait / ((double)num_tasks);
    double CPU_util = (time_elapsed - idle_time) / time_elapsed;

    FILE *fp1 = fopen("proj1-b_output", "w");

    fprintf(fp1, "%.6lf\n", avg_wait1);
    fprintf(fp1, "%.6lf\n", avg_wait2);
    fprintf(fp1, "%.6lf\n", avg_queue_length);
    fprintf(fp1, "%.6lf\n", CPU_util);

    fclose(fp1);

    free(waitLine);
    free_node(wait_queue);
    free_node(queue);

    return;
}

double _constantRate(double service_rate)
{
    int random_value = rand();
    float unit_rand = random_value / (double)RAND_MAX;
    return ((-1 / service_rate) * log(1 - unit_rand));
}

double _getArrivaltime(double service_rate, double arrivalrate)
{
    return arrivalrate * exp(-arrivalrate * _constantRate(service_rate));
}

double _getServicetime(double service_rate, double arrivalrate)
{
    return service_rate * exp(-service_rate * _constantRate(service_rate));
}

int _getSubtasks()
{
    return (rand() % 32) + 1;
}
int _cmp_fn(Node *current, Node *node)
{
    if (current->arrivalTime < node->arrivalTime)
    {
        return 0;
    }
    else if (current->arrivalTime > node->arrivalTime)
    {
        return 1;
    }
    else
    {
        if (current->priority >= node->priority)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

int _cmp_fn1(Node *current, Node *node)
{
    if (current->priority <= node->priority)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

Node *_enqueue(Node **pq, int service_time, int arrival_time, int priority)
{

    Node *node = malloc(sizeof(Node));

    if (node == NULL)
    {
        return NULL;
    }

    node->serviceTime = service_time;
    node->arrivalTime = arrival_time;
    node->priority = priority;

    if (*pq == NULL)
    {
        *pq = node;
        return node;
    }
    Node temp;

    temp.next = *pq;
    Node *prev = &temp;

    Node *current = *pq;

    while (current != NULL)
    {
        if (_cmp_fn(current, node) > 0)
        {
            break;
        }
        prev = current;
        current = current->next;
    }
    prev->next = node;
    node->next = current;
    *pq = temp.next;

    return node;
}

Node *_enqueue1(Node **pq, int service_time, int arrival_time, int priority)
{

    Node *node = malloc(sizeof(Node));

    if (node == NULL)
    {
        return NULL;
    }

    node->serviceTime = service_time;
    node->arrivalTime = arrival_time;
    node->priority = priority;

    if (*pq == NULL)
    {
        *pq = node;
        return node;
    }
    Node temp;

    temp.next = *pq;
    Node *prev = &temp;

    Node *current = *pq;

    while (current != NULL)
    {
        if (_cmp_fn1(current, node) > 0)
        {
            break;
        }
        prev = current;
        current = current->next;
    }
    prev->next = node;
    node->next = current;
    *pq = temp.next;

    return node;
}

Node *_dequeue(Node **pq)
{
    if (pq == NULL)
    {
        return NULL;
    }

    //Node *node = malloc(sizeof(Node));
    Node *node = NULL;
    if (node == NULL)
    {
        return NULL;
    }
    node = *pq;
    *pq = node->next;
    node->next = NULL;

    return node;
}

void _pq_push(Node **pq, int service_time, int arrival_time, int priority)
{
    if (pq == NULL)
    {
        return;
    }

    Node *node = malloc(sizeof(node));

    if (node == NULL)
    {
        return;
    }
    node->serviceTime = service_time;
    node->arrivalTime = arrival_time;
    node->priority = priority;
    node->next = *pq;
    *pq = node;

    return;
}

void free_node(Node *pq)
{

    if (pq == NULL)
    {
        return;
    }
    Node *temp = pq;
    while (temp != NULL)
    {
        Node *end = temp->next;
        free(temp);
        temp = end;
    }
    return;
}
