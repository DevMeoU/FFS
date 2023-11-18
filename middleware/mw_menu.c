#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../hal/file_hal.h"
// #include "../utilities/linked_list/linked_list.h"
#include "../fatfs_drive/fatfs_drive.h"
#include "mw_menu.h"

/* This Funtion is read content of SubDirectory */
static int32_t APPMIDW_MenuReadSubDir(Node *NodeData);
/* This Funtion is initilize menu */
static int32_t APPMIDW_MenuReadFile(Node *NodeData, uint32_t *InputUser);
/* This Funtion is print data */
static int32_t APPMIDW_MenuInfo(Node printList);
/* This function is to check the number inputed by user
The number is a positive integer from 0 to maxNumber */
static uint32_t APPMIDW_MenuInputNumberByUser(uint32_t maxNumber);
/* CODE */

/* This function in use link list for manage data in menu */
int32_t APPMIDW_MenuInit(const char *const filePath)
{
    int32_t returnValue = -1;
    uint32_t inputUser = 0;
    uint32_t i = 0;
    uint32_t maxNode = 0;
    uint32_t inputUserFile = 0;
    uint32_t temp;
    bool readFile = false;

    returnValue = HAL_FileInit(filePath);
    APPMIDW_Assert(returnValue, "File initilization fail.", "File initilization success.");

    returnValue = FATFS_InitFile();
    APPMIDW_Assert(returnValue, NULL, NULL);
#if 0
    MAIN_RT:
    Node * NodeData = NULL;
    FATFS_readDirectory(&NodeData, true);
    CLS
    TITLE_MENU
    Node * PrintData = NodeData;
    Node * Element = NULL;
    Node * Temp = NULL;
    /* print Node root */
    while(PrintData != NULL)
    {
        APPMIDW_MenuInfo(* PrintData);
        maxNode++;
        PrintData = PrintData->next;
    }
    inputUser = maxNode;
    Element = NodeData;
    while(1)
    {
        Temp = Element;
        while(Temp != NULL)
        {
            Temp = Temp->next;
            inputUser++;
        }

        inputUser = APPMIDW_MenuInputNumberByUser(inputUser);
        do{
            if((Element == NULL) && (inputUser == temp))
                goto MAIN_RT;
            else if(Element == NULL)
                break;
            if(inputUser == Element->data.DATA_Num)
            {
                if(Element->data.DATA_SubDir == 2)
                {
                    if(Element->data.DATA_FstClusLO == 0)
                    {
                        goto MAIN_RT;
                    }
                    else
                    {
                        while(Element != NULL)
                        {
                            if(Element->data.DATA_SubDir != 2)
                            {
                                Element = Element->next;
                            }
                            else
                            {
                                APPMIDW_MenuReadSubDir(Element);
                                for(i = 0; i < inputUser; i++)
                                    Element = Element->next;
                                    break;
                            }
                        }
                        break;
                    }
                }
                if((Element->data.DATA_Attr == ATT_DIRECTORY) && (Element->data.DATA_SubDir == 0))
                    APPMIDW_MenuReadSubDir(Element);
                else if((Element->data.DATA_Attr != ATT_DIRECTORY) && (Element->data.DATA_SubDir == 0))
                {
                    APPMIDW_MenuReadFile(Element, &inputUserFile);
                    readFile = true;
                    if(readFile == true)
                    {
                        do
                        {
                           printf("\n\tPlease input '0 from the keyboard to back: ");
                           scanf("%d", &inputUserFile);
                        } while(inputUserFile != 0);
                        if(Element->data.DATA_SubDir != 1)
                            goto MAIN_RT;
                        else{

                        }
                    }
                }
                break;
            }
            else
            {
                if(Element->next == NULL)
                {
                    temp = Element->data.DATA_Num;
                }
                Element = Element->next;
            }
        } while(inputUser != 0);
    }
#endif
EXIT_RT:
    return 0;
}

static int32_t APPMIDW_MenuReadSubDir(Node *NodeData)
{
    int32_t InputUser = 0;
    Node *Element = NodeData;
    Node *PrintData = NULL;
    Element->next = NULL;                 /* Entry Root */
    FATFS_readDirectory(&Element, false); /* read entry subDir */
    if ((Element->data.DATA_SubDir == 2) && (Element->next->data.DATA_SubDir == 1))
    {
        Element = Element->next->next;
    }
    PrintData = Element;
    /* print sub entry info */
    CLS
        TITLE_MENU while (PrintData != NULL)
    {
        if (PrintData->data.DATA_SubDir == 2)
        {

            while ((PrintData != NULL) && (PrintData->data.DATA_SubDir != 1))
            {
                APPMIDW_MenuInfo(*PrintData);
                InputUser++;
                PrintData = PrintData->next;
            }
        }
        else
        {
            PrintData = PrintData->next;
        }
    }
    return InputUser;
}

static int32_t APPMIDW_MenuReadFile(Node *NodeData, uint32_t *InputUser)
{
    int32_t retVal = 0;
    uint32_t NextClus = 0;
    uint8_t text[512];
    Node *Element = NodeData;
    Element->next = NULL; /* Entry Root */
    CLS
        NextClus = Element->data.DATA_FstClusLO;
    do
    {
        NextClus = FATFS_readFile(text, NextClus);
        for (int i = 0; i < sizeof(text); i++)
            printf("%c", text[i]);
    } while (FATFS_readFile(text, NextClus) != -1);
    return retVal;
}

static int32_t APPMIDW_MenuInfo(Node printList)
{
    int32_t retVal = 0;
    uint32_t i = 0;
    if (printList.data.DATA_SubDir == 1)
    {
        __TAB(4)
        printf("| %02d |", printList.data.DATA_Num);
        __TAB(4)
        printf(" .");
        __TAB(4)
        TAB printf("|");
        __TAB(9)
        TAB printf("|");
        __TAB(9)
        TAB printf("|");
        __TAB(9)
        TAB printf("|");
        __TAB(11)
        TAB printf("|");
        __TAB(12)
        printf("|");
        NEWLINE __LINE
    }
    else if (printList.data.DATA_SubDir == 2)
    {
        __TAB(4)
        printf("| %02d |", printList.data.DATA_Num);
        __TAB(4)
        printf("..");
        __TAB(4)
        TAB printf("|");
        __TAB(9)
        TAB printf("|");
        __TAB(9)
        TAB printf("|");
        __TAB(9)
        TAB printf("|");
        __TAB(11)
        TAB printf("|");
        __TAB(12)
        printf("|");
        NEWLINE __LINE
    }
    else
    {
        __TAB(4)
        printf("| %02d |", printList.data.DATA_Num);
        TAB for (i = 0; i < 8; i++)
            printf(" %c", printList.data.DATA_FileName[i]);
        TAB printf("|");
        __TAB(4)
        for (i = 0; i < 3; i++)
            printf(" %c", printList.data.DATA_Ext[i]);
        __TAB(3)
        printf("|");
        if (printList.data.DATA_Attr == ATT_DIRECTORY)
        {
            __TAB(2)
            printf(" File folder");
            __TAB(2)
            printf("|");
        }
        else
        {
            __TAB(4)
            printf("File");
            __TAB(4)
            printf("|");
        }
        __TAB(3)
        printf("%06dB ", printList.data.DATA_FileSize);
        __TAB(3)
        printf("|");
        __TAB(3)
        printf(" %02d:%02d:%02d ", printList.data.DATA_CrtDate, printList.data.DATA_CrtMonth, printList.data.DATA_CrtYear);
        __TAB(3)
        printf("|");
        __TAB(4)
        printf("%02d:%02d:%02d", printList.data.DATA_CrtHour, printList.data.DATA_CrtMin, printList.data.DATA_CrtSec);
        __TAB(4)
        printf("|");
        NEWLINE __LINE
    }
    return retVal;
}

/* This function is to check the number inputed by user
 * The number is a positive integer from 0 to maxNumber */
static uint32_t APPMIDW_MenuInputNumberByUser(uint32_t maxNumber)
{
    uint32_t number = 0;
    do
    {
        printf("\n\n\tPlease input your Select from keyboard: ");
        fflush(stdin);
        scanf("%d", &number);
    } while ((number < 0) || (number > maxNumber));
    return number;
}
