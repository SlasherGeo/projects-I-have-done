#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/*
 * This files purpose is to serve as a doubly linked list for the sq file.
 * created by Habeeb Rehman on 4/24/22
 */

typedef struct NODE {
    ElemType data;
    struct NODE *next;
    struct NODE *prev;
} NODE;

typedef struct DLIST {
    int length;
    NODE *front;
    NODE *back;
} DLIST;

//creates double list
extern DLIST *dlst_create() {
    DLIST *dlst = malloc(sizeof(DLIST));

    dlst->front = NULL;
    dlst->back = NULL;
    dlst->length = 0;

    return dlst;
}

//frees every node
void dlst_free(DLIST *dlst) {
    NODE *free_ptr = dlst->front;

    while (free_ptr != NULL) {
        NODE *temp = free_ptr;
        free_ptr = free_ptr->next;
        free(temp);
    }
    free(dlst);
}

int dlst_is_empty(DLIST *dlst) {
    return dlst->front == NULL;
}

void dlst_print(DLIST *dlst) {
    printf("[");
    NODE *d_print = dlst->front;
    while (d_print != NULL) {
        printf(" %d ", d_print->data);
        d_print = d_print->next;
    }
    printf("]");
}

//pushes a new node into the front of list
NODE * dlst_push_front(DLIST *dlst, ElemType val) {
    NODE *new_front = malloc(sizeof(NODE));
    new_front->data = val;
    new_front->next = dlst->front;
    new_front->prev = NULL;


    if (dlst->front != NULL) {
        dlst->front->prev = new_front;
    }
    if (dlst->back == NULL) {
        dlst->back = new_front;
    }
    dlst->front = new_front;

    dlst->length++;
    return new_front;
}

//pushes a new node into the back of the list
NODE * dlst_push_back(DLIST *dlst, ElemType val) {
    NODE *new_back = malloc(sizeof(NODE));
    new_back->data = val;
    new_back->prev = dlst->back;
    new_back->next = NULL;

    if (dlst->back != NULL) {
        dlst->back->next = new_back;
    }
    if (dlst->front == NULL) {
        dlst->front = new_back;
    }
    dlst->back = new_back;

    dlst->length++;
    return new_back;
}

//returns length of list
int dlst_length(DLIST *dlst) {
    return dlst->length;
}

//returns data from the front node of list and deletes it
extern ElemType dlst_pop_front(DLIST *dlst) {
    if (dlst_is_empty(dlst)) {
        return 0; //nothing in the list
    }

    NODE *head = dlst->front;
    if (dlst->front == dlst->back) {
        ElemType capture = dlst->front->data;
        free(dlst->front);
        dlst->back = NULL;
        dlst->front = NULL;
        dlst->length--;
        return capture;
    }


    ElemType capture = head->data;
    NODE *head_n = head->next;

    head_n->prev = NULL;
    free(head);
    dlst->front = head_n;
    dlst->length--;

    return capture;
}

//returns data from the last node of list and deletes it
extern ElemType dlst_pop_back(DLIST *dlst) {
    if (dlst_is_empty(dlst)) {
        return 0; //nothing in the list
    }

    NODE *tail = dlst->back;
    ElemType capture = tail->data;
    NODE *tail_p = tail->prev;

    if (dlst->front == dlst->back) {
        capture = tail->data;
        free(dlst->front);
        dlst->front = NULL;
        dlst->back = NULL;
        dlst->length--;
        return capture;
    }

    tail_p->next = NULL;
    tail->next = NULL;
    tail->prev = NULL;
    free(tail);
    dlst->back = tail_p;
    dlst->length--;

    return capture;
}

int dlst_remove_first(DLIST *dlst, NODE *del) {
    if(dlst_is_empty(dlst)){
        return 0;
    }
    if(del == NULL){
        return 0;
    }
    if(del == dlst->front){
        dlst_pop_front(dlst);
        return 1;
    }
    if(del == dlst->back){
        dlst_pop_back(dlst);
        return 1;
    }
    del->prev->next = del->next;
    del->next->prev = del->prev;
    free(del);
    dlst->length--;
    return 1;

}




