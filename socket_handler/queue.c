
#include "queue.h"

queue* queue_create(void) {
    queue* q = safe_calloc(1, sizeof(queue));
    q->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    q->cond = (pthread_cond_t )PTHREAD_COND_INITIALIZER;
    q->front = NULL;
    q->back = NULL;
    return q;
}

data queue_dequeue(queue* q) {
    pthread_mutex_lock(&q->mutex);

    while(q->front == NULL) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }

    node* front_node = q->front;
    data data_to_return = front_node->value;
    q->front = front_node->next_in_line;
    free(front_node);
    pthread_mutex_unlock(&q->mutex);
    return data_to_return;
}

void queue_enqueue(queue* q, data value) {
    pthread_mutex_lock(&q->mutex);
    node* new_node = safe_calloc(1, sizeof(node));
    new_node->value = value;
    new_node->next_in_line = NULL;

    if(q->front == NULL)
        q->front = q->back = new_node;
    else {
        q->back->next_in_line = new_node;
        q->back = new_node;
    }
    
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->mutex);
}

bool queue_is_empty(queue* q){
    pthread_mutex_lock(&q->mutex);
    bool is_empty = q->front == NULL;
    pthread_mutex_unlock(&q->mutex);
    return is_empty;
}

void queue_free(queue* q) {
    pthread_mutex_lock(&q->mutex);
    while(q->front != NULL){
        node *next_node = q->front->next_in_line;
        if(q->freeFunc != NULL)
            q->freeFunc(q->front->value);
        free(q->front);
        q->front = next_node;
    }
    free(q);
    pthread_mutex_unlock(&q->mutex);
}

void queue_set_memory_handler(queue* q, memFreeFunc* freeFunc){
    pthread_mutex_lock(&q->mutex);
    q->freeFunc = freeFunc;
    pthread_mutex_unlock(&q->mutex);
}

void queue_release_threads(queue* q) {
    pthread_cond_broadcast(&q->cond);
}
