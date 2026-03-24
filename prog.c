#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100

// Structure to define a Task
typedef struct {
    int id;
    char description[100];
    int priority;    // Lower number = Higher priority
    long timestamp;  // For tie-breaking (FCFS)
} Task;

// Structure for the Priority Queue (Min-Heap)
typedef struct {
    Task heap[MAX];
    int size;
} PriorityQueue;

// Function to swap two tasks
void swap(Task *a, Task *b) {
    Task temp = *a;
    *a = *b;
    *b = temp;
}

// Comparison logic: Priority first, then Timestamp for ties
int compare(Task a, Task b) {
    if (a.priority < b.priority) return -1;
    if (a.priority > b.priority) return 1;
    if (a.timestamp < b.timestamp) return -1;
    return 1;
}

// Restore heap property from bottom to top
void heapifyUp(PriorityQueue *pq, int index) {
    int parent = (index - 1) / 2;
    while (index > 0 && compare(pq->heap[index], pq->heap[parent]) < 0) {
        swap(&pq->heap[index], &pq->heap[parent]);
        index = parent;
        parent = (index - 1) / 2;
    }
}

// Restore heap property from top to bottom
void heapifyDown(PriorityQueue *pq, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && compare(pq->heap[left], pq->heap[smallest]) < 0)
        smallest = left;
    if (right < pq->size && compare(pq->heap[right], pq->heap[smallest]) < 0)
        smallest = right;

    if (smallest != index) {
        swap(&pq->heap[index], &pq->heap[smallest]);
        heapifyDown(pq, smallest);
    }
}

// Add a new task to the scheduler
void addTask(PriorityQueue *pq) {
    if (pq->size >= MAX) {
        printf("\n[!] Error: Scheduler queue is full!\n");
        return;
    }

    Task t;
    printf("\n--- Create New Task ---\n");
    printf("Enter Task ID (Integer): ");
    scanf("%d", &t.id);
    printf("Enter Description: ");
    scanf(" %[^\n]s", t.description);
    printf("Enter Priority (1-10, where 1 is urgent): ");
    scanf("%d", &t.priority);
    
    // Using a counter-based timestamp to ensure unique arrival order
    t.timestamp = time(NULL) + pq->size; 

    pq->heap[pq->size] = t;
    heapifyUp(pq, pq->size);
    pq->size++;

    printf("[+] Task '%s' added successfully.\n", t.description);
}

// Remove and execute the highest priority task
void executeTask(PriorityQueue *pq) {
    if (pq->size == 0) {
        printf("\n[!] No pending tasks to execute.\n");
        return;
    }

    Task top = pq->heap[0];
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->size--;
    heapifyDown(pq, 0);

    printf("\n>>> EXECUTING: [ID: %d] %s (Priority: %d)\n", top.id, top.description, top.priority);
}

// Display tasks in their current heap storage order
void displayQueue(PriorityQueue *pq) {
    if (pq->size == 0) {
        printf("\n[i] The queue is currently empty.\n");
        return;
    }

    printf("\n--- Current Task Queue (Heap Order) ---\n");
    printf("%-5s %-20s %-10s\n", "ID", "Description", "Priority");
    for (int i = 0; i < pq->size; i++) {
        printf("%-5d %-20s %-10d\n", pq->heap[i].id, pq->heap[i].description, pq->heap[i].priority);
    }
}

int main() {
    PriorityQueue pq = { .size = 0 };
    int choice;

    while (1) {
        printf("\n==============================");
        printf("\n   TASK SCHEDULER SYSTEM");
        printf("\n==============================");
        printf("\n1. Add New Task");
        printf("\n2. Execute Next Priority Task");
        printf("\n3. View All Pending Tasks");
        printf("\n4. Exit");
        printf("\nSelect Option: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1: addTask(&pq); break;
            case 2: executeTask(&pq); break;
            case 3: displayQueue(&pq); break;
            case 4: 
                printf("Shutting down scheduler. Goodbye!\n");
                exit(0);
            default: 
                printf("[!] Invalid selection. Please try again.\n");
        }
    }
    return 0;
}
