#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSES 10
#define NUM_QUEUES 3

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
    int pid;
    int burst_time;
    int remaining_time;
    int priority;
} Process;

typedef struct {
    Process *queue[MAX_PROCESSES];
    int front, rear;
} Queue;

void initializeQueue(Queue *q) {
    q->front = q->rear = -1;
}

int isEmpty(Queue *q) {
    return (q->front == -1);
}

void enqueue(Queue *q, Process *process) {
    if (q->rear == MAX_PROCESSES - 1) {
        printf("La cola está llena\n");
        exit(EXIT_FAILURE);
    }

    if (q->front == -1)
        q->front = 0;
    q->rear++;
    q->queue[q->rear] = process;
}

Process *dequeue(Queue *q) {
    if (isEmpty(q)) {
        printf("La cola está vacía\n");
        exit(EXIT_FAILURE);
    }

    Process *process = q->queue[q->front];
    if (q->front == q->rear)
        q->front = q->rear = -1;
    else
        q->front++;
    return process;
}

double execute_process(Process *process, int time_slice) {
    double start_time = (double)clock() / CLOCKS_PER_SEC;
    while (process->remaining_time > 0) {
        if (process->remaining_time > time_slice) {
            process->remaining_time -= time_slice;
            process->priority++;
        } else {
            process->remaining_time = 0;
        }
    }
    double end_time = (double)clock() / CLOCKS_PER_SEC;
    return end_time - start_time;
}

double mlfq(Process *processes, int num_processes, int time_slice) {
    Queue queues[NUM_QUEUES];
    for (int i = 0; i < NUM_QUEUES; i++)
        initializeQueue(&queues[i]);

    for (int i = 0; i < num_processes; i++)
        enqueue(&queues[0], &processes[i]);

    double total_time = 0.0;
    while (1) {
        int all_empty = 1;
        for (int queue_idx = 0; queue_idx < NUM_QUEUES; queue_idx++) {
            if (!isEmpty(&queues[queue_idx])) {
                all_empty = 0;
                Process *process = dequeue(&queues[queue_idx]);
                double execution_time = execute_process(process, time_slice);
                total_time += execution_time;
                if (process->remaining_time > 0)
                    enqueue(&queues[MIN(queue_idx + 1, NUM_QUEUES - 1)], process);
            }
        }
        if (all_empty)
            break;
    }

    return total_time;
}

int main() {
    Process processes[3] = {{1, 5, 5, 0}, {2, 3, 3, 0}, {3, 7, 7, 0}};
    int num_processes = 3;
    int time_slice = 2;

    clock_t start_time = clock();
    double total_time = mlfq(processes, num_processes, time_slice);
    clock_t end_time = clock();

    printf("Tiempo total de ejecución en C: %.6f segundos\n", total_time);
    printf("Tiempo real de ejecución en C: %.6f segundos\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    return 0;
}
