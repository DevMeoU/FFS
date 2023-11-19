/*** LINKED LISH ***/
#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stdbool.h>

/* Define Struct Data Type */
typedef struct DATA
{
    uint32_t DATA_SubDir;
    uint32_t DATA_Num;
    uint8_t  DATA_FileName[8];
	uint8_t  DATA_Ext[3];  
    uint8_t  DATA_Attr;
    uint16_t DATA_CrtHour;
    uint16_t DATA_CrtMin;
    uint16_t DATA_CrtSec;
    uint16_t DATA_CrtDate;
    uint16_t DATA_CrtMonth;
    uint16_t DATA_CrtYear;
    uint16_t DATA_FstClusLO;
    uint32_t DATA_FileSize;
} LIST_Data_t;

/** Struct */
typedef struct Node{
    LIST_Data_t * data;
    struct Node * next;
} Node;

typedef struct {
    uint32_t list;
    uint32_t element;
} ListNode_t;

/** Function Prototype */
/* This function is to add an element to the last position of linked list */
void Link_addLastNode(Node ** headNode, LIST_Data_t * newData);
/* This function is to find an element to the last position of linked list */
int32_t Link_findNode(Node * phead, uint32_t index);
/* This function is to delete the last element of linked list */
void Link_deleteLastNode(Node ** headNode);

#endif /*_LINKED_LIST_H_*/
