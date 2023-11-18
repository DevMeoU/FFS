#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../hal/file_hal.h"
// #include "../utilities/linked_list/linked_list.h"
#include "fatfs_drive.h"

/* This function is read boot sector */
static int32_t FATFS_readBootSector();
/* This function is read index next cluster in FAT table */
static int32_t FATFS_readFATtable(Node *ListData, uint32_t *curPosInFatTable);
/* This function is to check the first byte of file name */
static int32_t FATFS_checkFileName(FATFS_EntryFormat_t * RootEntry);
/* This function is reverse byte data */
static int32_t FATFS_reverseMax4Byte(uint8_t *byte, uint32_t count);
/* This function is indicate boot infomation */
static void FATFS_IndicateBootInfo(const FATFS_BootFormat126_t *boot);
/* This function is log debuging */
static void FATFS_Log(uint8_t * buff, uint32_t size);

/* g_FatTypes Return Value in range [-1,0,1,2]*/
static uint8_t s_RawData[FATFS_BYTES_PER_SECTOR];
FATFS_FatTypes_t g_FatType = ERROR;
FATFS_BootData_t g_BootData =
    {
        .FirstRootClus = 0,
        .FirstDataClus = 0,
        .RootDirSector = 0,
        .BytePerSec = 0,
        .SectorPerClus = 0,
        .RsvdSecCnt = 0,
        .NumFats = 0,
        .RootEntCnt = 0,
        .TotSec = 0,
        .FatSz = 0,
        .DataSec = 0,
};

int32_t FATFS_InitFile()
{
    int32_t retVal = 0;              /* -1 is ERROR value */
                                     /* initialize variable */
    retVal = FATFS_readBootSector(); /* read boot sector */
#if (FATFS_DEBUG_MODE == FATFS_ON)
    (g_FatType == FAT12) ? printf("FAT 12\n") : (g_FatType == FAT16) ? printf("FAT 16\n")
                                            : (g_FatType == FAT32)   ? printf("FAT 32\n")
                                                                     : printf("ERROR\n");
#endif
    return retVal;
}

static int32_t FATFS_readBootSector()
{
    int32_t retVal = -1; /* -1 is ERROR value */
    uint32_t CountofClusters = 0;

    retVal = HAL_ReadMultiSector(0, 1, s_RawData); /* read 1 sector from sector 0 */
#if (FATFS_DEBUG_MODE == FATFS_ON)
    FATFS_IndicateBootInfo((FATFS_BootFormat126_t *)s_RawData);
#endif

    g_BootData.RootEntCnt = (uint32_t)FATFS_reverseMax4Byte(((FATFS_BootFormat126_t *)s_RawData)->BPB_RootEntCnt, FATFS_GENERIC_BPB_ROOT_ENTRY_CNT_LEN);   /* First, we determine the count of sectors occupied by the root directory */
    g_BootData.BytePerSec = (uint32_t)FATFS_reverseMax4Byte(((FATFS_BootFormat126_t *)s_RawData)->BPB_BytsPerSec, FATFS_GENERIC_BPB_BYTES_PER_SECTOR_LEN); /* we determine the bytes per sector */
    g_BootData.RootDirSector = (uint32_t)((g_BootData.RootEntCnt * FATFS_BYTES_OF_ENTRY) + (g_BootData.BytePerSec - 1)) / g_BootData.BytePerSec;            /* we determine sector of root directory */
    g_BootData.FatSz = (uint32_t)FATFS_reverseMax4Byte(((FATFS_BootFormat126_t *)s_RawData)->BPB_FATSz16, FATFS_GENERIC_BPB_TOTAL_SECTOR_16_LEN);          /* we determine the sector of FAT table */

    if (g_BootData.FatSz == 0)
    {
        g_BootData.RootEntCnt = (uint32_t)FATFS_reverseMax4Byte(((FATFS_BootFormat32_t *)s_RawData)->BPB_RootEntCnt, FATFS_GENERIC_BPB_ROOT_ENTRY_CNT_LEN);   /* First, we determine the count of sectors occupied by the root directory */
        g_BootData.BytePerSec = (uint32_t)FATFS_reverseMax4Byte(((FATFS_BootFormat32_t *)s_RawData)->BPB_BytsPerSec, FATFS_GENERIC_BPB_BYTES_PER_SECTOR_LEN); /* we determine the bytes per sector */
        g_BootData.RootDirSector = (uint32_t)((g_BootData.RootEntCnt * FATFS_BYTES_OF_ENTRY) + (g_BootData.BytePerSec - 1)) / g_BootData.BytePerSec;           /* we determine sector of root directory */
        g_BootData.FatSz = (uint32_t)FATFS_reverseMax4Byte(((FATFS_BootFormat32_t *)s_RawData)->BPB_FATSz32, FATFS_32_BPB_FATFS_SIZE_32_LEN);
    }

    g_BootData.TotSec = (uint32_t)FATFS_reverseMax4Byte(((FATFS_BootFormat126_t *)s_RawData)->BPB_TotSec16, FATFS_GENERIC_BPB_TOTAL_SECTOR_16_LEN); /* we determine the total sector of file */

    if (g_BootData.TotSec == 0)
    {
        g_BootData.TotSec = (uint32_t)FATFS_reverseMax4Byte(((FATFS_BootFormat32_t *)s_RawData)->BPB_TotSec32, FATFS_GENERIC_BPB_TOTAL_SECTOR_32_LEN);
    }

    if (g_BootData.TotSec != 0)
    {
        g_BootData.RsvdSecCnt = (uint32_t)FATFS_reverseMax4Byte(((FATFS_BootFormat126_t *)s_RawData)->BPB_RsvdSecCnt, FATFS_GENERIC_BPB_TOTAL_SECTOR_16_LEN); /* we determine the total sector of file */
        g_BootData.NumFats = (uint32_t)FATFS_reverseMax4Byte(((FATFS_BootFormat126_t *)s_RawData)->BPB_NumFATs, FATFS_GENERIC_BPB_NUM_FAT_LEN);               /* we determine the total sector of file */
        g_BootData.SectorPerClus = (uint32_t)FATFS_reverseMax4Byte(((FATFS_BootFormat126_t *)s_RawData)->BPB_SecPerClus, FATFS_GENERIC_BPB_SECTOR_PER_CLUSTER_LEN);
        g_BootData.FirstRootClus = (uint32_t)FATFS_NUMBER_OF_BOOT_SECTORS + (g_BootData.NumFats * g_BootData.FatSz); /* Calculate fist byte in root location */
        g_BootData.FirstDataClus = g_BootData.FirstRootClus + g_BootData.RootDirSector;                              /* Calculate the position of the first Data Area sector */
    }
    else
    {
        g_BootData.RsvdSecCnt = FATFS_reverseMax4Byte(((FATFS_BootFormat32_t *)s_RawData)->BPB_RsvdSecCnt, FATFS_GENERIC_BPB_TOTAL_SECTOR_16_LEN);                 /* we determine the total sector of file */
        g_BootData.NumFats = FATFS_reverseMax4Byte(((FATFS_BootFormat32_t *)s_RawData)->BPB_NumFATs, FATFS_GENERIC_BPB_NUM_FAT_LEN);                               /* we determine the total sector of file */
        g_BootData.SectorPerClus = (uint32_t)FATFS_reverseMax4Byte(((FATFS_BootFormat32_t *)s_RawData)->BPB_SecPerClus, FATFS_GENERIC_BPB_SECTOR_PER_CLUSTER_LEN); /* we determine the total sector of file */
        g_BootData.FirstRootClus = (uint32_t)FATFS_NUMBER_OF_BOOT_SECTORS + (g_BootData.NumFats * g_BootData.FatSz);                                               /* Calculate fist byte in root location */
        g_BootData.FirstDataClus = g_BootData.FirstRootClus + g_BootData.RootDirSector;                                                                            /* Calculate the position of the first Data Area sector */
    }

    g_BootData.DataSec = g_BootData.TotSec - (g_BootData.RsvdSecCnt + (g_BootData.NumFats * g_BootData.FatSz) + g_BootData.RootDirSector);

    CountofClusters = g_BootData.DataSec / g_BootData.SectorPerClus; /* Next, we determine the count of sectors in the data region of the volume */

    if (CountofClusters < FATFS_MAX_CLUSTER_OF_FATFS_12)
    {
        /* Volume is FAT12 */
        g_FatType = FAT12;
    }
    else if (CountofClusters > FATFS_MAX_CLUSTER_OF_FATFS_12 && CountofClusters < FATFS_MAX_CLUSTER_OF_FATFS_16)
    {
        /* Volume is FAT16 */
        g_FatType = FAT16;
    }
    else
    {
        /* Volume is FAT32 */
        g_FatType = FAT32;
    }
RT_READ_BOOT:
    return retVal;
}

static int32_t FATFS_readFATtable(Node *ListData, uint32_t *curPosInFatTable)
{
    int32_t retVAl = -1;
    uint32_t clusCount = 0;
    (*curPosInFatTable) = (ListData->data.DATA_FstClusLO * 1.5) + clusCount; /* current position in fat table */
    uint32_t nextPosInFatTable = 0;
    uint8_t *temp = (uint8_t *)malloc(3); /* 3 byte for 2 clus */

    if (((*curPosInFatTable) % 3) != 0)
    {
        (*curPosInFatTable) -= 1;
        // HAL_ReadMultiByte(1, (* curPosInFatTable), 1, temp, 3);
        nextPosInFatTable = ((((temp[1]) << 8) | (temp[0])) & 0xFFF);
    }
    else
    {
        // HAL_ReadMultiByte(1, (* curPosInFatTable), 1, temp, 3);
        nextPosInFatTable = (((temp[1]) >> 4) | ((temp[2]) << 8));
    }
    clusCount++;
    retVAl = clusCount;
    return retVAl;
}

int32_t FATFS_readDirectory(Node **NodeData, bool isRoot)
{
    int32_t retVal = -1U;
    uint32_t number = 0U;
    uint32_t i = 0U;
    uint32_t dateAfterReverse = 0U;
    uint32_t timeAfterReverse = 0U;
    uint32_t rootDirSectorPos = 0;
    LIST_Data_t ListData;
    /* allocation data entry */
    FATFS_EntryFormat_t * DataEntry = NULL;

    do
    {
        for (i = 0U; i < g_BootData.RootEntCnt; i++)
        {
            /* read root sector */
            if (isRoot == true)
            {
                rootDirSectorPos = g_BootData.FirstRootClus + FATFS_CAL_SECTOR_INDEX(i);
                
                HAL_ReadSector(rootDirSectorPos, s_RawData);

                DataEntry = (FATFS_EntryFormat_t *)(s_RawData + (i * FATFS_BYTES_OF_ENTRY));
                FATFS_Log(s_RawData, 512);

#if (FATFS_DEBUG_MODE == FATFS_ON)
                FATFS_Log((uint8_t *)DataEntry, FATFS_BYTES_OF_ENTRY);
                printf("rootDirSectorPos = %d\n", rootDirSectorPos);
                printf("\ni = %d\n\n", i);
#endif
                /* get entry valid */
                if ((DataEntry->DIR_Attr[0] != ATT_LONG_FILE_NAME) && (FATFS_checkFileName(DataEntry) == 0))
                {
                    number++; /* calcular number of DataEntry */
                    ListData.DATA_SubDir = 0;
                    ListData.DATA_Num = number;
                    memcpy(&ListData.DATA_FileName, DataEntry->DIR_Name, 8);
                    memcpy(&ListData.DATA_Ext, DataEntry->DIR_Ext, 3);
                    memcpy(&ListData.DATA_Attr, DataEntry->DIR_Attr, 1);
                    timeAfterReverse = FATFS_reverseMax4Byte(DataEntry->DIR_CrtTime, 2);
                    dateAfterReverse = FATFS_reverseMax4Byte(DataEntry->DIR_CrtDate, 2);

                    ListData.DATA_CrtHour = FATFS_CALC_HOURS(timeAfterReverse);
                    ListData.DATA_CrtMin = FATFS_CALC_MINUTES(timeAfterReverse);
                    ListData.DATA_CrtSec = FATFS_CALC_SECONDS(timeAfterReverse);

                    ListData.DATA_CrtDate = FATFS_CALC_DAY(dateAfterReverse);
                    ListData.DATA_CrtMonth = FATFS_CALC_MONTH(dateAfterReverse);
                    ListData.DATA_CrtYear = FATFS_CALC_YEAR(dateAfterReverse) + 1980;

                    ListData.DATA_FstClusLO = FATFS_reverseMax4Byte(DataEntry->DIR_FstClusLO, 2);
                    ListData.DATA_FileSize = FATFS_reverseMax4Byte(DataEntry->DIR_FileSize, 4);

                    /* add note */
                    Link_addLastNode(NodeData, ListData);
                }
            }
            //     else
            //     {
            //         totalReadEntry = (FATFS_readFATtable(* NodeData, &indexStartClus) * 512) / 32;
            //         indexStartClus = (g_BootData.FirstDataClus - 2) + (* NodeData)->data.DATA_FstClusLO;
            //         // HAL_ReadMultiByte(indexStartClus, (i * 32), 1, tempData, 32);
            //     }

            //     if(isRoot != true)
            //     {
            //         if((DataEntry->DIR_Name[0] == FATFS_SUB_PATH) && (DataEntry->DIR_Name[1] == FATFS_SUB_PATH))
            //         {
            //             number++; /* calcular number of DataEntry */
            //             ListData.DATA_SubDir = 2;
            //             ListData.DATA_Num = number;
            //             ListData.DATA_FstClusLO = FATFS_reverseMax4Byte(DataEntry->DIR_FstClusLO, 2);
            //             Link_addLastNode(NodeData, ListData);
            //         }
            //         else if(DataEntry->DIR_Name[0] == FATFS_SUB_PATH)
            //         {
            //             number++; /* calcular number of DataEntry */
            //             ListData.DATA_SubDir = 1;
            //             ListData.DATA_Num = number;
            //             ListData.DATA_FstClusLO = FATFS_reverseMax4Byte(DataEntry->DIR_FstClusLO, 2);
            //             Link_addLastNode(NodeData, ListData);
            //         }
            //     }
        }
        isRoot = false;
    } while (retVal != FATFS_12_EOC && retVal != -1 && isRoot != false);
    return number;
}

/* This function is to read a file in FAT12 files */
int32_t FATFS_readFile(uint8_t *buff, uint32_t entryOfCLuster)
{
    int32_t retVal = -1; /* 0 is OK value */
    uint32_t count = 0;
    /* Calculate the first sector's position of cluster */
    uint32_t firstSectorPosition = (entryOfCLuster - 2) * g_BootData.SectorPerClus + g_BootData.FirstDataClus;
    /* Calculte the first byte's position of cluster */
    uint32_t clusterPosition = firstSectorPosition;
    if ((entryOfCLuster != FATFS_12_EOC) && (g_FatType == FAT12))
    {
        /* Print all sectors in cluster */
        HAL_ReadMultiSector(clusterPosition, 1, buff);
        /* Determine the next cluster */
        /* In FAT12 Type, every 3 bytes represent 2 entries */
        /* Calculate the offset of next cluster's entry in FAT Area */
        uint32_t nextClusterOffset = (uint32_t)(3 * (entryOfCLuster) / 2); //
        uint32_t nextClusterPosition = 0;
        uint8_t doubleEntry[3] = {0};
        uint32_t nextClusterEntry = 0;
        /* In case nextClusterOffset is not divisible by 3, nextClusterEntry is at the middle byte's position,
         * and we will get the second entry */

        if ((nextClusterOffset % 3) != 0)
        {
            /* Calculate the first position of 3 bytes */
            nextClusterPosition = nextClusterOffset - 1;
            /* Move the file's poiter to the position */
            // HAL_ReadMultiByte(1, nextClusterPosition, 1, doubleEntry, 3); /* 3 byte */
            nextClusterEntry = (((doubleEntry[1]) >> 4) | ((doubleEntry[2]) << 4));
        }
        /* In case nextClusterOffset is divisible by 3, nextClusterEntry is at the first byte's position,
         * and we will get the first entry */
        else
        {
            /* Calculate the first position of 3 bytes */
            nextClusterPosition = nextClusterOffset;
            /* Move the file's poiter to the position */
            // HAL_ReadMultiByte(1, nextClusterPosition, 1, doubleEntry, 3);
            nextClusterEntry = ((((doubleEntry[1]) & 0x0F) << 8) | (doubleEntry[0]));
        }
        /* Assign data to entryOfCLuster */
        entryOfCLuster = nextClusterEntry;
        count++;
        return entryOfCLuster;
    }
    else if ((entryOfCLuster != FATFS_16_EOC) && (g_FatType == FAT16))
    {
        /* Print all sectors in cluster */
        HAL_ReadMultiSector(clusterPosition, 1, buff);
        /* Determine the next cluster */
        /* In FAT12 Type, every 3 bytes represent 2 entries */
        /* Calculate the offset of next cluster's entry in FAT Area */
        uint32_t nextClusterOffset = (uint32_t)(2 * (entryOfCLuster));
        uint32_t nextClusterPosition = nextClusterOffset;
        uint8_t doubleEntry[2] = {0};
        uint32_t nextClusterEntry = 0;
        /* In case nextClusterOffset is not divisible by 3, nextClusterEntry is at the middle byte's position,
         * and we will get the second entry */

        /* Move the file's poiter to the position */
        // HAL_ReadMultiByte(1, nextClusterPosition, 1, doubleEntry, 2); /* 2 byte */
        nextClusterEntry = FATFS_reverseMax4Byte(doubleEntry, 2);
        /* Assign data to entryOfCLuster */
        entryOfCLuster = nextClusterEntry;
        count++;
        return entryOfCLuster;
    }

    if ((entryOfCLuster == FATFS_12_EOC) || (entryOfCLuster == FATFS_16_EOC))
    {
        retVal = -1;
    }
    return retVal;
}

/* This function is to check the first byte of file name */
static int32_t FATFS_checkFileName(FATFS_EntryFormat_t * RootEntry)
{
    int32_t retVal = 0; /* 0 is OK value */
    switch (RootEntry->DIR_Name[0])
    {
    case FATFS_FILENAME_NEVER_USED:
        retVal = -1; /* -1 is ERROR value */
    case FATFS_FILENAME_DELETED_1:
        retVal = -1; /* -1 is ERROR value */
    case FATFS_FILENAME_DELETED_2:
        retVal = -1; /* -1 is ERROR value */
    case FATFS_FILENAME_SPECIAL_ENTRY:
        retVal = -1; /* -1 is ERROR value */
    }
    return retVal;
}

static int32_t FATFS_reverseMax4Byte(uint8_t *byte, uint32_t count)
{
    int32_t result = 0; /* -1 is ERROR value */
    if (count <= 4)
    {
        for (int i = count - 1; i >= 0; i--)
            result = (result << 8) | byte[i];
    }
    return result;
}

static void FATFS_IndicateBootInfo(const FATFS_BootFormat126_t *boot)
{
    printf("BS_JmpBoot: ");
    for (int i = 0; i < FATFS_GENERIC_BS_JMP_BOOT_LEN; i++)
    {
        printf("%02x ", boot->BS_JmpBoot[i]);
    }
    printf("\n");

    printf("BS_OEMName: ");
    for (int i = 0; i < FATFS_GENERIC_BS_OEM_NAME_LEN; i++)
    {
        printf("%c", boot->BS_OEMName[i]);
    }
    printf("\n");

    printf("BPB_BytsPerSec: %d\n", FATFS_reverseMax4Byte((uint8_t *)boot->BPB_BytsPerSec, FATFS_GENERIC_BPB_BYTES_PER_SECTOR_LEN));
    printf("BPB_SecPerClus: %d\n", FATFS_reverseMax4Byte((uint8_t *)boot->BPB_SecPerClus, FATFS_GENERIC_BPB_SECTOR_PER_CLUSTER_LEN));
    printf("BPB_RsvdSecCnt: %d\n", FATFS_reverseMax4Byte((uint8_t *)boot->BPB_RsvdSecCnt, FATFS_GENERIC_BPB_RESERVE_SECTOR_CNT_LEN));
    printf("BPB_NumFATs: %d\n", FATFS_reverseMax4Byte((uint8_t *)boot->BPB_NumFATs, FATFS_GENERIC_BPB_NUM_FAT_LEN));
    printf("BPB_RootEntCnt: %d\n", FATFS_reverseMax4Byte((uint8_t *)boot->BPB_RootEntCnt, FATFS_GENERIC_BPB_ROOT_ENTRY_CNT_LEN));
    printf("BPB_TotSec16: %d\n", FATFS_reverseMax4Byte((uint8_t *)boot->BPB_TotSec16, FATFS_GENERIC_BPB_TOTAL_SECTOR_16_LEN));
    printf("BPB_Media: %d\n", FATFS_reverseMax4Byte((uint8_t *)boot->BPB_Media, FATFS_GENERIC_BPB_MEDIA_LEN));
    printf("BPB_FATSz16: %d\n", FATFS_reverseMax4Byte((uint8_t *)boot->BPB_FATSz16, FATFS_GENERIC_BPB_FAT_SIZE_16_LEN));
    printf("BPB_SecPerTrk: %d\n", FATFS_reverseMax4Byte((uint8_t *)boot->BPB_SecPerTrk, FATFS_GENERIC_BPB_SECTOR_PER_TRACK_LEN));
    printf("BPB_NumHeads: %d\n", FATFS_reverseMax4Byte((uint8_t *)boot->BPB_NumHeads, FATFS_GENERIC_BPB_NUM_HEADS_LEN));
    printf("BPB_HiddSec: %d\n", FATFS_reverseMax4Byte((uint8_t *)boot->BPB_HiddSec, FATFS_GENERIC_BPB_HIDDEN_SECTOR_LEN));
    printf("BPB_TotSec32: %d\n", FATFS_reverseMax4Byte((uint8_t *)boot->BPB_TotSec32, FATFS_GENERIC_BPB_TOTAL_SECTOR_32_LEN));
    printf("BS_FatNum: %d\n", FATFS_reverseMax4Byte((uint8_t *)boot->BS_FatNum, FATFS_12_16_BS_FATFSE_NUM_LEN));
    printf("BS_Reserved: ");
    for (int i = 0; i < FATFS_12_16_BS_RESERVED_LEN; i++)
    {
        printf("%02x ", boot->BS_Reserved[i]);
    }
    printf("\n");

    printf("BS_BootSig: ");
    for (int i = 0; i < FATFS_12_16_BS_BOOT_SIG_LEN; i++)
    {
        printf("%02x ", boot->BS_BootSig[i]);
    }
    printf("\n");

    printf("BS_VolID: ");
    for (int i = 0; i < FATFS_12_16_BS_VOL_ID_LEN; i++)
    {
        printf("%c", boot->BS_VolID[i]);
    }
    printf("\n");

    printf("BS_VolLab: ");
    for (int i = 0; i < FATFS_12_16_BS_VOL_LABEL_LEN; i++)
    {
        printf("%c", boot->BS_VolLab[i]);
    }
    printf("\n");
}

static void FATFS_Log(uint8_t * buff, uint32_t size)
{
    int i = 0U;
    printf("\n");
    for (i = 0U; i < size; i++)
    {
        printf("%02X ", buff[i]);
    }
    printf("\n");
}