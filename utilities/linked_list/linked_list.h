/*** LINKED LISH ***/
#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stdbool.h>

/** Struct */
typedef struct Node{
    uint32_t * data; /* [0]: start cluster, [1]: entry data of cluster */
    struct Node * next;
} Node;

typedef struct {
    uint32_t list;
    uint32_t element;
} ListNode_t;

/** Function Prototype */
/* This function is to add an element to the last position of linked list */
void Link_addLastNode(Node ** headNode, uint32_t * newData);
/* This function is to find an element to the last position of linked list */
int32_t Link_findNode(Node * phead, uint32_t index);
/* This function is to delete the last element of linked list */
void Link_deleteLastNode(Node ** headNode);

#endif /*_LINKED_LIST_H_*/
