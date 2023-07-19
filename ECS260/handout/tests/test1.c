#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct node *node_t;
struct node {
    void *data;
    node_t prev;
    node_t next;
};
//queue is implemented using a doubly linked list
struct queue {
    int count;
    node_t head;
    node_t tail;
};
typedef struct queue *queue_t;
queue_t queue_create(void) {
    queue_t new_queue;
    //allocate memory
    new_queue = (queue_t) malloc(sizeof(new_queue));
    new_queue->head = NULL; // mistake
    new_queue->tail = NULL; // mistake
    //the queue is currently empty
    new_queue->count = 0;
    return new_queue;
}

int queue_destroy(queue_t queue) {
    if (!queue)
        return -1;
    if (queue->count > 0)
        return -1;
    free(queue);
    return 0;
}

int queue_enqueue(queue_t queue, void *data) {
    node_t new_element;
    new_element = (node_t) malloc(sizeof(node_t));
    new_element->data = (void *) malloc(sizeof(void *));
    new_element->data = data;
    new_element->next = NULL;
    new_element->prev = NULL;
    //the queue is empty
    if (queue->count == 0) {
        queue->tail = new_element;
        queue->head = new_element;
    } else {
        new_element->next = queue->head; // common
        queue->head->prev = new_element;
        queue->head = new_element;// common
    }
    queue->count++;
    return 0;
}

int queue_dequeue(queue_t queue, void **data) {
    if (queue->count == 0)
        return -1;
    *data = queue->tail->data;
    if (queue->tail->prev)
        queue->tail->prev->next = NULL;
    queue->tail = queue->tail->prev;
    queue->count--; // free queue tail // if last element queue->head is not updated
    return 0;
}

int queue_delete(queue_t queue, void *data) {
    if (queue->count == 0)
        return -1;
    node_t ptr = queue->tail;
    while (ptr) {
        //the item is found in the queue
        if (ptr->data == data) {
            if (ptr->prev && ptr->next) {
                ptr->prev->next = ptr->next;
                ptr->next->prev = ptr->prev; // free mem
            } else {// case with no element
                if (ptr->prev)
                    ptr->prev->next = NULL;
                else if (ptr->next)
                    ptr->next->prev = NULL;
            }
            queue->count--;
            return 0;
        }
        ptr = ptr->prev;
    }
    return -1;
}

int queue_length(queue_t queue) {
    return queue->count;
}

int main() {
    queue_t test_queue = queue_create();
    printf("%d\n", queue_length(test_queue));
    int data = 5;
    queue_enqueue(test_queue, &data);
    data = 6;
    queue_enqueue(test_queue, &data);
    printf("%d\n", queue_length(test_queue));
    int *ret;
//    queue_delete(test_queue, &data);
    data = 5;
    queue_delete(test_queue, &data);
    printf("%d", queue_length(test_queue));
    return 0;
}