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
static int32_t APPMIDW_MenuReadFile(LIST_Data_t * DataList, uint32_t * InputUser);
/* This Funtion is print data */
static int32_t APPMIDW_MenuInfo(LIST_Data_t DataList, bool isRoot);
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
    uint32_t numOfRootDataEntry = 0U;
    bool readFile = false;
    Node * NodePath = NULL;
    Node * RootDataPath = NULL;
    LIST_Data_t RootDataList[100];

    returnValue = HAL_FileInit(filePath);
    APPMIDW_Assert(returnValue, "File initilization fail.", "File initilization success.");

    returnValue = FATFS_InitFile();
    APPMIDW_Assert(returnValue, NULL, NULL);

    /**
     * *******************************************************************************************
     *  Read root data entry.
     * *******************************************************************************************
     */
    FATFS_readRootDataEntry(&RootDataPath, RootDataList);
    
    CLS
    TITLE_MENU

    for(i = 0; i < RootDataPath->data[1]; i++) {
        APPMIDW_MenuInfo(RootDataList[i], true);
    }
    /**
     * *******************************************************************************************
     * User selection
     * *******************************************************************************************
     */
    inputUser = APPMIDW_MenuInputNumberByUser(RootDataPath->data[1]);
EXIT_RT:
    return 0;
}

static int32_t APPMIDW_MenuReadSubDir(Node *NodeData)
{
    int32_t InputUser = 0;
    Node *Element = NodeData;
    Node *DataIndicate = NULL;
    Element->next = NULL;                 /* Entry Root */
    // FATFS_readDirectory(&Element, false); /* read entry subDir */
    // if ((Element->data->DATA_SubDir == 2) && (Element->next->data->DATA_SubDir == 1))
    // {
    //     Element = Element->next->next;
    // }
    DataIndicate = Element;
    /* print sub entry info */
    CLS
        TITLE_MENU while (DataIndicate != NULL)
    {
        // if (DataIndicate->data->DATA_SubDir == 2)
        // {

        //     while ((DataIndicate != NULL) && (DataIndicate->data->DATA_SubDir != 1))
        //     {
        //         // APPMIDW_MenuInfo(*DataIndicate);
        //         InputUser++;
        //         DataIndicate = DataIndicate->next;
        //     }
        // }
        // else
        // {
        //     DataIndicate = DataIndicate->next;
        // }
    }
    return InputUser;
}

static int32_t APPMIDW_MenuReadFile(LIST_Data_t * DataList, uint32_t * InputUser)
{
    int32_t retVal = 0;
    uint32_t NextClus = 0;
    uint8_t text[512];
    CLS
        NextClus = DataList->DATA_FstClusLO;
    do
    {
        NextClus = FATFS_readFile(text, NextClus);
        for (int i = 0; i < sizeof(text); i++)
            printf("%c", text[i]);
    } while (FATFS_readFile(text, NextClus) != -1);
    return retVal;
}

static int32_t APPMIDW_MenuInfo(LIST_Data_t DataList, bool isRoot)
{
    int32_t retVal = 0;
    uint32_t i = 0;

    if (isRoot == true)
    {
        if (DataList.DATA_SubDir == 1)
        {
            __TAB(4)
            printf("| %02d |", DataList.DATA_Num);
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
        else if (DataList.DATA_SubDir == 2)
        {
            __TAB(4)
            printf("| %02d |", DataList.DATA_Num);
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
            printf("| %02d |", DataList.DATA_Num);
            TAB for (i = 0; i < 8; i++)
                printf(" %c", DataList.DATA_FileName[i]);
            TAB printf("|");
            __TAB(4)
            for (i = 0; i < 3; i++)
                printf(" %c", DataList.DATA_Ext[i]);
            __TAB(3)
            printf("|");
            if (DataList.DATA_Attr == ATT_DIRECTORY)
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
            printf("%06dB ", DataList.DATA_FileSize);
            __TAB(3)
            printf("|");
            __TAB(3)
            printf(" %02d:%02d:%02d ", DataList.DATA_CrtDate, DataList.DATA_CrtMonth, DataList.DATA_CrtYear);
            __TAB(3)
            printf("|");
            __TAB(4)
            printf("%02d:%02d:%02d", DataList.DATA_CrtHour, DataList.DATA_CrtMin, DataList.DATA_CrtSec);
            __TAB(4)
            printf("|");
            NEWLINE __LINE
        }
        return retVal;
    }

    // if (printList.data.DATA_SubDir == 1)
    // {
    //     __TAB(4)
    //     printf("| %02d |", printList.data.DATA_Num);
    //     __TAB(4)
    //     printf(" .");
    //     __TAB(4)
    //     TAB printf("|");
    //     __TAB(9)
    //     TAB printf("|");
    //     __TAB(9)
    //     TAB printf("|");
    //     __TAB(9)
    //     TAB printf("|");
    //     __TAB(11)
    //     TAB printf("|");
    //     __TAB(12)
    //     printf("|");
    //     NEWLINE __LINE
    // }
    // else if (printList.data.DATA_SubDir == 2)
    // {
    //     __TAB(4)
    //     printf("| %02d |", printList.data.DATA_Num);
    //     __TAB(4)
    //     printf("..");
    //     __TAB(4)
    //     TAB printf("|");
    //     __TAB(9)
    //     TAB printf("|");
    //     __TAB(9)
    //     TAB printf("|");
    //     __TAB(9)
    //     TAB printf("|");
    //     __TAB(11)
    //     TAB printf("|");
    //     __TAB(12)
    //     printf("|");
    //     NEWLINE __LINE
    // }
    // else
    // {
    //     __TAB(4)
    //     printf("| %02d |", printList.data.DATA_Num);
    //     TAB for (i = 0; i < 8; i++)
    //         printf(" %c", printList.data.DATA_FileName[i]);
    //     TAB printf("|");
    //     __TAB(4)
    //     for (i = 0; i < 3; i++)
    //         printf(" %c", printList.data.DATA_Ext[i]);
    //     __TAB(3)
    //     printf("|");
    //     if (printList.data.DATA_Attr == ATT_DIRECTORY)
    //     {
    //         __TAB(2)
    //         printf(" File folder");
    //         __TAB(2)
    //         printf("|");
    //     }
    //     else
    //     {
    //         __TAB(4)
    //         printf("File");
    //         __TAB(4)
    //         printf("|");
    //     }
    //     __TAB(3)
    //     printf("%06dB ", printList.data.DATA_FileSize);
    //     __TAB(3)
    //     printf("|");
    //     __TAB(3)
    //     printf(" %02d:%02d:%02d ", printList.data.DATA_CrtDate, printList.data.DATA_CrtMonth, printList.data.DATA_CrtYear);
    //     __TAB(3)
    //     printf("|");
    //     __TAB(4)
    //     printf("%02d:%02d:%02d", printList.data.DATA_CrtHour, printList.data.DATA_CrtMin, printList.data.DATA_CrtSec);
    //     __TAB(4)
    //     printf("|");
    //     NEWLINE __LINE
    // }
    return retVal;
}

/* This function is to check the number inputed by user
 * The number is a positive integer from 0 to maxNumber */
static uint32_t APPMIDW_MenuInputNumberByUser(uint32_t maxNumber)
{
    uint32_t number = 0;
    do
    {
        printf("\n\n\t\"Note: Input '0' to exit.\"");
        printf("\n\tPlease input your Select from keyboard: ");
        fflush(stdin);
        scanf("%d", &number);
    } while ((number < 0) || (number > maxNumber));
    if(number == 0){exit;};
    return number - 1;
}
