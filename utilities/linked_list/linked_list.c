#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include <stdbool.h>
#include <string.h>
#include "linked_list.h"


/* This function is to add an element to the last position of linked list */
void Link_addLastNode(Node ** headNode, LIST_Data_t * newData){
    /* 1. allocate node */
	Node * newNode = (Node *)malloc(sizeof(Node));
	Node * lastNode = * headNode;  /* used in step 5*/
	
	/* 2. put in the data  */
    newNode->data  = newData;

	/* 3. This new node is going to be the last node, so make next of it as NULL*/
    newNode->next = NULL;

	/* 4. If the Linked List is empty, then make the new node as head */
    if ((* headNode) == NULL)
    {
       (* headNode) = newNode;
       return;
    }
       
    /* 5. Else traverse till the last node */
    while (lastNode->next != NULL)
        lastNode = lastNode->next;
   
    /* 6. Change the next of last node */
    lastNode->next = newNode;
    return;     
}

int32_t Link_findNode(Node * phead, uint32_t index)
{
    int32_t retVal = 0;
    while (phead != NULL) {
        if (phead->data->DATA_Num == index)
        {
            return retVal;
        }    
        phead = phead->next;
        retVal++;
    }
}

/* This function is to delete the last element of linked list */
void Link_deleteLastNode(Node ** headNode)
{
    if((* headNode) != NULL) {
    	/* If there is one node, delete that one */
    	if ((* headNode)->next == NULL){
            (* headNode) = NULL;
        }
    	/* If there are 2 or more nodes, find the last node and delete it */
        else{
            Node * temp = * headNode;
            while((temp->next)->next != NULL){
                temp = temp->next;
            }
            temp->next = NULL;
        }
    }
}

