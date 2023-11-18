#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../hal/file_hal.h"
//#include "../utilities/linked_list/linked_list.h"
#include "fatfs_drive.h"

/* This function is read boot sector */
static int32_t FATFS_readBootSector();
/* This function is read index next cluster in FAT table */
static int32_t FATFS_readFATtable(Node * ListData, uint32_t * curPosInFatTable);
/* This function is to check the first byte of file name */
static int32_t FATFS_checkFileName(FATFS_EntryFormat_t RootEntry);
/* This function is reverse byte data */
static int32_t FATFS_reverseMax4Byte(uint8_t *byte, uint32_t count);


/* g_FatTypes Return Value in range [-1,0,1,2]*/
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
    int32_t retVal = 0; /* -1 is ERROR value */
    /* initialize variable */
	retVal = FATFS_readBootSector(); /* read boot sector */
    return retVal;
}

static int32_t FATFS_readBootSector()
{
    int32_t retVal = -1; /* -1 is ERROR value */
	uint32_t CountofClusters = 0;
	FATFS_BootData126_t BootData126 =
	{
		.u32_FirstRootClus = 0,
		.u32_FirstDataClus = 0,
		.u32_RootDirSector = 0,
		.u16_BytsPerSec = 0,
		.u8_SecPerClus = 0,
		.u16_RsvdSecCnt = 0,
		.u8_NumFATs = 0,
		.u16_RootEntCnt = 0,
		.u16_TotSec16 = 0,
		.u16_FATSz16 = 0,
		.u32_TotSec32 = 0
	};

	FATFS_BootData32_t BootData32 =
    {
        .u32_FirstRootClus = 0,
        .u32_FirstDataClus = 0,
    	.u32_RootDirSector = 0,
        .u16_BytsPerSec = 0,
        .u8_SecPerClus = 0,
        .u16_RsvdSecCnt = 0,
        .u8_NumFATs = 0,
        .u16_RootEntCnt = 0,
        .u16_TotSec16 = 0,
    	.u32_FATSz32 = 0,
        .u32_TotSec32 = 0,
        .u8_FatNum = 0,
    };


    FATFS_BootFormat126_t * boot_126 = (FATFS_BootFormat126_t *)malloc(sizeof(FATFS_BootFormat126_t));
    FATFS_BootFormat32_t * boot_32 = (FATFS_BootFormat32_t *)malloc(sizeof(FATFS_BootFormat32_t));

    uint8_t * data = (uint8_t *)malloc(sizeof(FATFS_BootFormat126_t)); /* 1 sector = 512 byte */
	retVal = HAL_ReadMultiSector(0, 1, data);                         /* read 1 sector from sector 0 */

    memcpy(boot_126, data, sizeof(FATFS_BootFormat126_t));
	memcpy(boot_32, boot_126, sizeof(FATFS_BootFormat32_t));
    free(data);

    BootData126.u16_RootEntCnt = FATFS_reverseMax4Byte(boot_126->BPB_RootEntCnt, FATFS_GENERIC_BPB_ROOT_ENTRY_CNT_LEN); /* First, we determine the count of sectors occupied by the root directory */
    BootData126.u16_BytsPerSec = FATFS_reverseMax4Byte(boot_126->BPB_BytsPerSec, FATFS_GENERIC_BPB_BYTES_PER_SECTOR_LEN); /* we determine the bytes per sector */
	BootData126.u32_RootDirSector = ((BootData126.u16_RootEntCnt * FATFS_BYTE_OF_ENTRY) + (BootData126.u16_BytsPerSec - 1)) / BootData126.u16_BytsPerSec; /* we determine sector of root directory */
    BootData126.u16_FATSz16 = FATFS_reverseMax4Byte(boot_126->BPB_FATSz16, FATFS_GENERIC_BPB_TOTAL_SECTOR_16_LEN); /* we determine the sector of FAT table */

	g_BootData.RootEntCnt = (uint32_t)BootData126.u16_RootEntCnt;
	g_BootData.BytePerSec = (uint32_t)BootData126.u16_BytsPerSec;
	g_BootData.RootDirSector = (uint32_t)BootData126.u32_RootDirSector;
	g_BootData.FatSz = (uint32_t)BootData126.u16_FATSz16;

    if (g_BootData.FatSz == 0)
    {
		BootData32.u16_RootEntCnt = FATFS_reverseMax4Byte(boot_32->BPB_RootEntCnt, FATFS_GENERIC_BPB_ROOT_ENTRY_CNT_LEN); /* First, we determine the count of sectors occupied by the root directory */
		BootData32.u16_BytsPerSec = FATFS_reverseMax4Byte(boot_32->BPB_BytsPerSec, FATFS_GENERIC_BPB_BYTES_PER_SECTOR_LEN); /* we determine the bytes per sector */
		BootData32.u32_RootDirSector = ((BootData32.u16_RootEntCnt * FATFS_BYTE_OF_ENTRY) + (BootData32.u16_BytsPerSec - 1)) / BootData32.u16_BytsPerSec; /* we determine sector of root directory */
		BootData32.u32_FATSz32 = FATFS_reverseMax4Byte(boot_32->BPB_FATSz32, FATFS_32_BPB_FATFS_SIZE_32_LEN);

		g_BootData.RootEntCnt = (uint32_t)BootData32.u16_RootEntCnt;
		g_BootData.BytePerSec = (uint32_t)BootData32.u16_BytsPerSec;
		g_BootData.SectorPerClus = (uint32_t)BootData32.u32_RootDirSector;
		g_BootData.FatSz = (uint32_t)BootData32.u32_FATSz32;
    }

    BootData126.u16_TotSec16 = FATFS_reverseMax4Byte(boot_126->BPB_TotSec16, FATFS_GENERIC_BPB_TOTAL_SECTOR_16_LEN); /* we determine the total sector of file */
	g_BootData.TotSec = (uint32_t)BootData126.u16_TotSec16;

    if (g_BootData.TotSec == 0)
    {
        BootData32.u32_TotSec32 = FATFS_reverseMax4Byte(boot_32->BPB_TotSec32, FATFS_GENERIC_BPB_TOTAL_SECTOR_32_LEN);
		g_BootData.TotSec = (uint32_t)BootData32.u32_TotSec32;
    }

    if(boot_126 != NULL)
    {
        BootData126.u16_RsvdSecCnt = FATFS_reverseMax4Byte(boot_126->BPB_RsvdSecCnt, FATFS_GENERIC_BPB_TOTAL_SECTOR_16_LEN); /* we determine the total sector of file */
		BootData126.u8_NumFATs = FATFS_reverseMax4Byte(boot_126->BPB_NumFATs, FATFS_GENERIC_BPB_NUM_FAT_LEN); /* we determine the total sector of file */
		BootData126.u8_SecPerClus = FATFS_reverseMax4Byte(boot_126->BPB_SecPerClus, FATFS_GENERIC_BPB_SECTOR_PER_CLUSTER_LEN);
		BootData126.u32_FirstRootClus = FATFS_NUMBER_OF_BOOT_SECTORS + (BootData126.u8_NumFATs * g_BootData.FatSz); /* Calculate fist byte in root location */
		BootData126.u32_FirstDataClus = BootData126.u32_FirstRootClus + BootData126.u32_RootDirSector; /* Calculate the position of the first Data Area sector */

        g_BootData.SectorPerClus = (uint32_t)BootData126.u8_SecPerClus;
		g_BootData.RsvdSecCnt = (uint32_t)BootData126.u16_RsvdSecCnt;
		g_BootData.NumFats = (uint32_t)BootData126.u8_NumFATs;
		g_BootData.FirstRootClus = (uint32_t)BootData126.u32_FirstRootClus;
		g_BootData.FirstDataClus = (uint32_t)BootData126.u32_FirstDataClus;
	}
    else
    {
        BootData32.u16_RsvdSecCnt = FATFS_reverseMax4Byte(boot_32->BPB_RsvdSecCnt, FATFS_GENERIC_BPB_TOTAL_SECTOR_16_LEN); /* we determine the total sector of file */
		BootData32.u8_NumFATs = FATFS_reverseMax4Byte(boot_32->BPB_NumFATs, FATFS_GENERIC_BPB_NUM_FAT_LEN); /* we determine the total sector of file */
		BootData32.u8_SecPerClus = (boot_32->BPB_SecPerClus, FATFS_GENERIC_BPB_SECTOR_PER_CLUSTER_LEN); /* we determine the total sector of file */
        BootData32.u32_FirstRootClus = FATFS_NUMBER_OF_BOOT_SECTORS + (BootData32.u8_FatNum * g_BootData.FatSz); /* Calculate fist byte in root location */
		BootData32.u32_FirstDataClus = BootData32.u32_FirstRootClus + BootData32.u32_RootDirSector; /* Calculate the position of the first Data Area sector */

        g_BootData.SectorPerClus = (uint32_t)BootData32.u8_SecPerClus;
        g_BootData.RsvdSecCnt = (uint32_t)BootData32.u16_RsvdSecCnt;
		g_BootData.NumFats = (uint32_t)BootData32.u8_NumFATs;
		g_BootData.FirstRootClus = (uint32_t)BootData32.u32_FirstRootClus;
		g_BootData.FirstDataClus = (uint32_t)BootData32.u32_FirstDataClus;
	}

    g_BootData.DataSec = g_BootData.TotSec - (g_BootData.RsvdSecCnt + (g_BootData.NumFats * g_BootData.FatSz) + g_BootData.RootDirSector);


    CountofClusters = g_BootData.DataSec / g_BootData.SectorPerClus; /* Next, we determine the count of sectors in the data region of the volume */

    if (CountofClusters < FATFS_MAX_CLUSTER_OF_FATFS_12)
    {
        /* Volume is FAT12 */
        g_FatType = FAT12;
		free(boot_32);
    }
    else if (CountofClusters > FATFS_MAX_CLUSTER_OF_FATFS_12 && CountofClusters < FATFS_MAX_CLUSTER_OF_FATFS_16)
    {
        /* Volume is FAT16 */
        g_FatType = FAT16;
		free(boot_32);
    }
    else
    {
        /* Volume is FAT32 */
        g_FatType = FAT32;
    }
    RT_READ_BOOT:
    return retVal;
}

static int32_t FATFS_readFATtable(Node * ListData, uint32_t * curPosInFatTable)
{
    int32_t retVAl = -1;
    uint32_t clusCount = 0;
    (* curPosInFatTable) = (ListData->data.DATA_FstClusLO * 1.5) + clusCount; /* current position in fat table */
    uint32_t nextPosInFatTable = 0;
    uint8_t * temp = (uint8_t *) malloc (3); /* 3 byte for 2 clus */

    if(((* curPosInFatTable) % 3) != 0)
    {
        (* curPosInFatTable) -= 1;
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

int32_t FATFS_readDirectory(Node ** NodeData, bool isRoot)
{
	int32_t retVal = -1;
    uint32_t number = 0;
	uint32_t i = 0;
    uint32_t dateAfterReverse = 0;
	uint32_t timeAfterReverse = 0;
    uint32_t totalReadEntry = g_BootData.RootDirSector;
    uint32_t indexStartClus = g_BootData.FirstRootClus;
	uint8_t * tempData = (uint8_t *) malloc (512);
	LIST_Data_t ListData;
    /* allocation data entry */
	FATFS_EntryFormat_t * DataEntry = (FATFS_EntryFormat_t *) malloc (sizeof(FATFS_EntryFormat_t));
	
    do{
        for(i = 0; i < totalReadEntry; i++)
        {
            /* read 32 byte entry */
            if(isRoot == true){
                // HAL_ReadMultiByte(indexStartClus, (i * 32), 1, tempData, 32);
            }
            else
            {
                totalReadEntry = (FATFS_readFATtable(* NodeData, &indexStartClus) * 512) / 32;
                indexStartClus = (g_BootData.FirstDataClus - 2) + (* NodeData)->data.DATA_FstClusLO;
                // HAL_ReadMultiByte(indexStartClus, (i * 32), 1, tempData, 32);
            }
            /* copy from tempData to DataEntry format */
            memcpy(DataEntry, tempData, 32);

            if(isRoot != true)
            {
                if((DataEntry->DIR_Name[0] == FATFS_SUB_PATH) && (DataEntry->DIR_Name[1] == FATFS_SUB_PATH))
                {
                    number++; /* calcular number of DataEntry */
                    ListData.DATA_SubDir = 2;
                    ListData.DATA_Num = number;
                    ListData.DATA_FstClusLO = FATFS_reverseMax4Byte(DataEntry->DIR_FstClusLO, 2);
                    Link_addLastNode(NodeData, ListData);
                }
                else if(DataEntry->DIR_Name[0] == FATFS_SUB_PATH)
                {
                    number++; /* calcular number of DataEntry */
                    ListData.DATA_SubDir = 1;
                    ListData.DATA_Num = number;
                    ListData.DATA_FstClusLO = FATFS_reverseMax4Byte(DataEntry->DIR_FstClusLO, 2);
                    Link_addLastNode(NodeData, ListData);
                }
            }
            /* get entry valid */
             if((DataEntry->DIR_Attr[0] != ATT_LONG_FILE_NAME) && (FATFS_checkFileName(* DataEntry) == 0))
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

                /* addNote */
                Link_addLastNode(NodeData, ListData);
            }
        }
        isRoot = false;
    } while (retVal != FATFS_12_EOC && retVal != -1 && isRoot != false);
    return number;
}

/* This function is to read a file in FAT12 files */
int32_t FATFS_readFile(uint8_t * buff, uint32_t entryOfCLuster)
{
    int32_t retVal = -1; /* 0 is OK value */
    uint32_t count = 0;
    /* Calculate the first sector's position of cluster */
    uint32_t firstSectorPosition = (entryOfCLuster - 2) * g_BootData.SectorPerClus + g_BootData.FirstDataClus;
    /* Calculte the first byte's position of cluster */
    uint32_t clusterPosition = firstSectorPosition;
    if((entryOfCLuster != FATFS_12_EOC) && (g_FatType == FAT12))
    {
        /* Print all sectors in cluster */
        HAL_ReadMultiSector(clusterPosition, 1, buff);
        /* Determine the next cluster */
        /* In FAT12 Type, every 3 bytes represent 2 entries */
        /* Calculate the offset of next cluster's entry in FAT Area */
        uint32_t nextClusterOffset = (uint32_t)(3 * (entryOfCLuster)/2);//
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
    else if((entryOfCLuster != FATFS_16_EOC) && (g_FatType == FAT16))
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
    
    if((entryOfCLuster == FATFS_12_EOC) || (entryOfCLuster == FATFS_16_EOC))
    {
        retVal = -1;
    }
    return retVal;
}

/* This function is to check the first byte of file name */
static int32_t FATFS_checkFileName(FATFS_EntryFormat_t RootEntry)
{
    int32_t retVal = 0; /* 0 is OK value */
    switch (RootEntry.DIR_Name[0])
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
