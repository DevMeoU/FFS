/*** FATFSE LAYER ***/
#ifndef _FATFS_DRIVE_H_
#define _FATFS_FATFSE_H_

#include "../utilities/linked_list/linked_list.h"

/*
 * BS  => "BOOT sector"
 * BPB => "BIOS Parameter Block"
 */
/* Generic definition */
#define FATFS_ON                                        1U
#define FATFS_OFF                                       0U
#define FATFS_DEBUG_MODE                                FATFS_OFF
#define FATFS_BYTES_PER_SECTOR                          512U

/*** Boot Sector ***/
#define FATFS_NUMBER_OF_BOOT_SECTORS                    1U
#define FATFS_MAX_CLUSTER_OF_FATFS_12                   4085U
#define FATFS_MAX_CLUSTER_OF_FATFS_16                   65525U

/* FAT12/16/32 common field (offset from 0 to 35)(dec) */
#define FATFS_GENERIC_BS_JMP_BOOT_OFFSET                0x00
#define FATFS_GENERIC_BPB_BYTES_PER_SECTOR_OFFSET       0x0B
#define FATFS_GENERIC_BS_OEM_NAME_OFFSET                0x03
#define FATFS_GENERIC_BPB_SECTOR_PER_CLUSTER_OFFSET     0x0D
#define FATFS_GENERIC_BPB_RESERVE_SECTOR_CNT_OFFSET     0x0E
#define FATFS_GENERIC_BPB_NUM_FATFS_OFFSET              0x10
#define FATFS_GENERIC_BPB_ROOT_ENTRY_CNT_OFFSET         0x11
#define FATFS_GENERIC_BPB_TOTAL_SECTOR_16_OFFSET        0x13
#define FATFS_GENERIC_BPB_MEDIA_OFFSET                  0x15
#define FATFS_GENERIC_BPB_FATFS_SIZE_16_OFFSET          0x16
#define FATFS_GENERIC_BPB_SECTOR_PER_TRACK_OFFSET       0x18
#define FATFS_GENERIC_BPB_NUM_HEADS_OFFSET              0x32
#define FATFS_GENERIC_BPB_HIDDEN_SECTOR_OFFSET          0x1C
#define FATFS_GENERIC_BPB_TOTAL_SECTOR_32_OFFSET        0x20

/* FAT12/16/32 common field (length from 0 to 35)(dec) */
#define FATFS_GENERIC_BS_JMP_BOOT_LEN                   3U
#define FATFS_GENERIC_BS_OEM_NAME_LEN                   8U
#define FATFS_GENERIC_BPB_BYTES_PER_SECTOR_LEN          2U
#define FATFS_GENERIC_BPB_SECTOR_PER_CLUSTER_LEN        1U
#define FATFS_GENERIC_BPB_RESERVE_SECTOR_CNT_LEN        2U
#define FATFS_GENERIC_BPB_NUM_FAT_LEN                   1U
#define FATFS_GENERIC_BPB_ROOT_ENTRY_CNT_LEN            2U
#define FATFS_GENERIC_BPB_TOTAL_SECTOR_16_LEN           2U
#define FATFS_GENERIC_BPB_MEDIA_LEN                     1U
#define FATFS_GENERIC_BPB_FAT_SIZE_16_LEN               2U
#define FATFS_GENERIC_BPB_SECTOR_PER_TRACK_LEN          2U
#define FATFS_GENERIC_BPB_NUM_HEADS_LEN                 2U
#define FATFS_GENERIC_BPB_HIDDEN_SECTOR_LEN             4U
#define FATFS_GENERIC_BPB_TOTAL_SECTOR_32_LEN           4U
#define FATFS_GENERIC_UNKNOWS                           476U

/* Fields for FAT12/16 volumes (offset from 36) */
#define FATFS_12_16_BS_FATFSE_NUM_OFFSET                0x24
#define FATFS_12_16_BS_RESERVED_OFFSET                  0x25
#define FATFS_12_16_BS_BOOT_SIG_OFFSET                  0x26
#define FATFS_12_16_BS_VOL_ID_OFFSET                    0x27
#define FATFS_12_16_BS_VOL_LABEL_OFFSET                 0x2B
#define FATFS_12_16_BS_FILE_SYS_TYPE_OFFSET             0x36
#define FATFS_12_16_BS_BOOT_CODE_OFFSET                 0x3E
#define FATFS_12_16_BS_BOOT_SIGN_OFFSET                 0x1FE

/* Fields for FAT12/16 volumes (length from 36) */
#define FATFS_12_16_BS_FATFSE_NUM_LEN                   1U
#define FATFS_12_16_BS_RESERVED_LEN                     1U
#define FATFS_12_16_BS_BOOT_SIG_LEN                     1U
#define FATFS_12_16_BS_VOL_ID_LEN                       4U
#define FATFS_12_16_BS_VOL_LABEL_LEN                    11U
#define FATFS_12_16_BS_FILE_SYS_TYPE_LEN                8U
#define FATFS_12_16_BS_BOOT_CODE_LEN                    448U
#define FATFS_12_16_BS_BOOT_SIGN_LEN                    2U

/* Fields for FAT32 volumes (offset from 36) */
#define FATFS_32_BPB_FATFS_SIZE_32_OFFSET               0x24
#define FATFS_32_BPB_EXT_FLAGS_OFFSET                   0x28
#define FATFS_32_BPB_FS_VER_OFFSET                      0x2A
#define FATFS_32_BS_ROOT_CLUS_OFFSET                    0x2C
#define FATFS_32_BS_FS_INFO_OFFSET                      0x30
#define FATFS_32_BS_BK_BOOT_SEC_TYPE_OFFSET             0x32
#define FATFS_32_BS_RESERVED_1_OFFSET                   0x34
#define FATFS_32_BS_FATFSE_NUM_OFFSET                   0x40
#define FATFS_32_BS_RESERVED_2_OFFSET                   0x41
#define FATFS_32_BS_BOOT_SIG_OFFSET                     0x42
#define FATFS_32_BS_VOL_ID_OFFSET                       0x43
#define FATFS_32_BS_VOL_LABEL_OFFSET                    0x47
#define FATFS_32_BS_FILE_SYS_TYPE_OFFSET                0x52
#define FATFS_32_BS_BOOT_CODE_OFFSET                    0x5A
#define FATFS_32_BS_BOOT_SIGN_OFFSET                    0x1FE

/* Fields for FAT32 volumes (length from 36) */
#define FATFS_32_BPB_FATFS_SIZE_32_LEN                  4U
#define FATFS_32_BPB_EXT_FLAGS_LEN                      2U
#define FATFS_32_BPB_FS_VER_LEN                         2U
#define FATFS_32_BS_ROOT_CLUS_LEN                       4U
#define FATFS_32_BS_FS_INFO_LEN                         2U
#define FATFS_32_BS_BK_BOOT_SEC_TYPE_LEN                2U
#define FATFS_32_BS_RESERVED_1_LEN                      12U
#define FATFS_32_BS_FATFSE_NUM_LEN                      1U
#define FATFS_32_BS_RESERVED_2_LEN                      1U
#define FATFS_32_BS_BOOT_SIG_LEN                        1U
#define FATFS_32_BS_VOL_ID_LEN                          4U
#define FATFS_32_BS_VOL_LABEL_LEN                       11U
#define FATFS_32_BS_FILE_SYS_TYPE_LEN                   8U
#define FATFS_32_BS_BOOT_CODE_LEN                       420U
#define FATFS_32_BS_BOOT_SIGN_LEN                       2U

/* Define struct __attribute__ ((__packed__)) FAT12/16 Boot Format Sector*/
typedef struct __attribute__ ((__packed__)) _FATFS_12_16_BOOT
{
    uint8_t BS_JmpBoot[FATFS_GENERIC_BS_JMP_BOOT_LEN];                  /* 0-2 code to jump to bootstrap */
    uint8_t BS_OEMName[FATFS_GENERIC_BS_OEM_NAME_LEN];                  /* 3-10 OEM name/ version */
    uint8_t BPB_BytsPerSec[FATFS_GENERIC_BPB_BYTES_PER_SECTOR_LEN];     /* 11-12 byte per sector */
    uint8_t BPB_SecPerClus[FATFS_GENERIC_BPB_SECTOR_PER_CLUSTER_LEN];   /* 13  serctor per cluster */
    uint8_t BPB_RsvdSecCnt[FATFS_GENERIC_BPB_RESERVE_SECTOR_CNT_LEN];   /* 14-15 number of reserved sectors */
    uint8_t BPB_NumFATs[FATFS_GENERIC_BPB_NUM_FAT_LEN];                 /* 16 number of FAT copies */
    uint8_t BPB_RootEntCnt[FATFS_GENERIC_BPB_ROOT_ENTRY_CNT_LEN];       /* 17-18 number of root directory entries*/
    uint8_t BPB_TotSec16[FATFS_GENERIC_BPB_TOTAL_SECTOR_16_LEN];        /* 19-20 total of number sector in filesystem*/
    uint8_t BPB_Media[FATFS_GENERIC_BPB_MEDIA_LEN];                     /* 21 media descriptior type*/
    uint8_t BPB_FATSz16[FATFS_GENERIC_BPB_FAT_SIZE_16_LEN];             /* 22-23 sector per FAT, 0 for FAT32*/
    uint8_t BPB_SecPerTrk[FATFS_GENERIC_BPB_SECTOR_PER_TRACK_LEN];      /* 24-25 sector per track*/
    uint8_t BPB_NumHeads[FATFS_GENERIC_BPB_NUM_HEADS_LEN];              /* 26-27 number of heads*/
    uint8_t BPB_HiddSec[FATFS_GENERIC_BPB_HIDDEN_SECTOR_LEN];           /* 28-31 number of hidden sectors*/
    uint8_t BPB_TotSec32[FATFS_GENERIC_BPB_TOTAL_SECTOR_32_LEN];        /* 32-35 Total number of blocks in the entire disk*/
    uint8_t BS_FatNum[FATFS_12_16_BS_FATFSE_NUM_LEN];                   /* 36 Physical FATFSe number*/
    uint8_t BS_Reserved[FATFS_12_16_BS_RESERVED_LEN];                   /* 37 Reserved */
    uint8_t BS_BootSig[FATFS_12_16_BS_BOOT_SIG_LEN];                    /* 38 Extended Boot Record Signature*/
    uint8_t BS_VolID[FATFS_12_16_BS_VOL_ID_LEN];                        /* 39-42 Volume Serial Number*/
    uint8_t BS_VolLab[FATFS_12_16_BS_VOL_LABEL_LEN];                    /* 43-53 Volume Label*/
    uint8_t BS_FilSysType[FATFS_12_16_BS_FILE_SYS_TYPE_LEN];            /* 54-61 name of FAT type*/
    uint8_t BS_BootCode[FATFS_12_16_BS_BOOT_CODE_LEN];                  /* 62-509 bootstrap code*/
    uint8_t BS_BootSign[FATFS_12_16_BS_BOOT_SIGN_LEN];                  /* 510-511 signature 0x55 0xaa*/
} FATFS_BootFormat126_t;

/* Define struct __attribute__ ((__packed__)) FAT32 Boot Sector */
typedef struct __attribute__ ((__packed__)) _FAT32_BOOT
{
    uint8_t BS_JmpBoot[FATFS_GENERIC_BS_JMP_BOOT_LEN];                /* 0-2 code to jump to bootstrap */
    uint8_t BS_OEMName[FATFS_GENERIC_BS_OEM_NAME_LEN];                /* 3-10 OEM name/ version */
    uint8_t BPB_BytsPerSec[FATFS_GENERIC_BPB_BYTES_PER_SECTOR_LEN];   /* 11-12 byte per sector */
    uint8_t BPB_SecPerClus[FATFS_GENERIC_BPB_SECTOR_PER_CLUSTER_LEN]; /* 13  serctor per cluster */
    uint8_t BPB_RsvdSecCnt[FATFS_GENERIC_BPB_RESERVE_SECTOR_CNT_LEN]; /* 14-15 number of reserved sectors */
    uint8_t BPB_NumFATs[FATFS_GENERIC_BPB_NUM_FAT_LEN];               /* 16 number of FAT copies */
    uint8_t BPB_RootEntCnt[FATFS_GENERIC_BPB_ROOT_ENTRY_CNT_LEN];     /* 17-18 number of root directory entries*/
    uint8_t BPB_TotSec16[FATFS_GENERIC_BPB_TOTAL_SECTOR_16_LEN];      /* 19-20 total of number sector in filesystem*/
    uint8_t BPB_Media[FATFS_GENERIC_BPB_MEDIA_LEN];                   /* 21 media descriptior type*/
    uint8_t BPB_FATSz16[FATFS_GENERIC_BPB_FAT_SIZE_16_LEN];           /* 22-23 sector per FAT, 0 for FAT32*/
    uint8_t BPB_SecPerTrk[FATFS_GENERIC_BPB_SECTOR_PER_TRACK_LEN];    /* 24-25 sector per track*/
    uint8_t BPB_NumHeads[FATFS_GENERIC_BPB_NUM_HEADS_LEN];            /* 26-27 number of heads*/
    uint8_t BPB_HiddSec[FATFS_GENERIC_BPB_HIDDEN_SECTOR_LEN];         /* 28-31 number of hidden sectors*/
    uint8_t BPB_TotSec32[FATFS_GENERIC_BPB_TOTAL_SECTOR_32_LEN];      /* 32-35 Total number of blocks in the entire disk*/
    uint8_t BPB_FATSz32[FATFS_32_BPB_FATFS_SIZE_32_LEN];             /* 36-39 Logical sectors per FAT */
    uint8_t BPB_ExtFlags[FATFS_32_BPB_EXT_FLAGS_LEN];              /* 40-41 FATFSe description */
    uint8_t BPB_FSVer[FATFS_32_BPB_FS_VER_LEN];                    /* 42-43 Version */
    uint8_t BPB_RootClus[FATFS_32_BS_ROOT_CLUS_LEN];               /* 44-47 Cluster number of root start */
    uint8_t BPB_FSInfo[FATFS_32_BS_FS_INFO_LEN];                   /* 48-49 Logical sector number of FS Information Sector*/
    uint8_t BPB_BkBootSec[FATFS_32_BS_BK_BOOT_SEC_TYPE_LEN];       /* 50-51 First logical sector number of a copy of the three FAT32 boot sectors */
    uint8_t BPB_Reserved[FATFS_32_BS_RESERVED_1_LEN];              /* 52-63 Reserved */
    uint8_t BS_DrvNum[FATFS_32_BS_FATFSE_NUM_LEN];                  /* 64 Physical FATFSe Number */
    uint8_t BS_Reserved[FATFS_32_BS_RESERVED_2_LEN];               /* 65 Used for various purposes */
    uint8_t BS_BootSig[FATFS_32_BS_BOOT_SIG_LEN];                  /* 66 Extended boot signature */
    uint8_t BS_VolID[FATFS_32_BS_VOL_ID_LEN];                      /* 67-70 Volume Serial Number */
    uint8_t BS_VolLab[FATFS_32_BS_VOL_LABEL_LEN];                  /* 71-81 Volume Label */
    uint8_t BS_FilSysType[FATFS_32_BS_FILE_SYS_TYPE_LEN];          /* 82-89 File system type */
    uint8_t BS_BootCode32[FATFS_32_BS_BOOT_CODE_LEN];              /* 90-509 Bootstrap code */
    uint8_t BS_BootSign[FATFS_32_BS_BOOT_SIGN_LEN];                /* 510-511 Signature 0x55 0xaa */
} FATFS_BootFormat32_t;


/* Define struct __attribute__ ((__packed__)) FAT12/16 Boot Data Sector*/
typedef struct __attribute__ ((__packed__)) _FATFS_12_16_BOOT_DATA
{
    uint32_t u32_FirstRootClus;
    uint32_t u32_FirstDataClus;
    uint32_t u32_RootDirSector;
    uint16_t u16_BytsPerSec;	/* 11-12 byte per sector */
    uint8_t  u8_SecPerClus;		/* 13  serctor per cluster */
    uint16_t u16_RsvdSecCnt; 	/* 14-15 number of reserved sectors */
    uint8_t  u8_NumFATs;        /* 16 number of FAT copies */
    uint16_t u16_RootEntCnt;    /* 17-18 number of root directory entries*/
    uint16_t u16_TotSec16;      /* 19-20 total of number sector in filesystem*/
    uint16_t u16_FATSz16;       /* 22-23 sector per FAT, 0 for FAT32*/
    uint16_t u16_SecPerTrk;    	/* 24-25 sector per track*/
    uint32_t u32_TotSec32;      /* 32-35 Total number of blocks in the entire disk*/
    uint8_t  u8_FatNum;         /* 36 Physical FATFSe number*/
} FATFS_BootData126_t;

/* Define struct __attribute__ ((__packed__)) FAT12/16 Boot Data Sector*/
typedef struct __attribute__ ((__packed__)) _FATFS_32_BOOT_DATA
{
    uint32_t u32_FirstRootClus;
    uint32_t u32_FirstDataClus;
    uint32_t u32_RootDirSector;
    uint16_t u16_BytsPerSec;	/* 11-12 byte per sector */
    uint8_t  u8_SecPerClus;		/* 13  serctor per cluster */
    uint16_t u16_RsvdSecCnt; 	/* 14-15 number of reserved sectors */
    uint8_t  u8_NumFATs;        /* 16 number of FAT copies */
    uint16_t u16_RootEntCnt;    /* 17-18 number of root directory entries*/
    uint16_t u16_TotSec16;      /* 19-20 total of number sector in filesystem*/
    uint32_t u32_FATSz32;       /* 22-23 sector per FAT, 0 for FAT32*/
    uint32_t u32_TotSec32;      /* 32-35 Total number of blocks in the entire disk*/
    uint8_t  u8_FatNum;         /* 36 Physical FATFSe number*/
} FATFS_BootData32_t;

/* Define struct __attribute__ ((__packed__)) FAT12/16 Boot Data Sector*/
typedef struct __attribute__ ((__packed__)) _FATFS_BOOT_DATA {
    uint32_t FirstRootClus;
	uint32_t FirstDataClus;
	uint32_t RootDirSector;
	uint32_t BytePerSec;
	uint32_t SectorPerClus;
	uint32_t RsvdSecCnt;
	uint32_t NumFats;
	uint32_t RootEntCnt;
	uint32_t TotSec;
	uint32_t FatSz;
	uint32_t DataSec;
} FATFS_BootData_t;

/** Define FAT sector*/
/* GENERIC */
#define FATFS_BYTES_OF_ENTRY 32U
#define FATFS_ENTRYS_OF_SECTOR (16U)
/* FAT12 */
#define FATFS_12_EOC 0xFFF /* Entry end of chain for directory or file in FAT12 */

/* FAT16 */
#define FATFS_16_EOC 0xFFFF /* Entry end of chain for directory or file in FAT16 */

/** Define Root Directory */
#define DIR_NAME_LEN                                8U
#define DIR_EXTENTION_LEN                           3U
#define DIR_ATTRIBUTES_LEN                          1U
#define DIR_NTRES_LEN                               1U
#define DIR_CRT_TIME_TENTH_LEN                      1U
#define DIR_CRT_TIME_LEN                            2U
#define DIR_CRT_DATE_LEN                            2U
#define DIR_LST_ACC_DATE_LEN                        2U
#define DIR_FST_CLUS_HIGH_LEN                       2U
#define DIR_WRT_TIME_LEN                            2U
#define DIR_WRT_DATE_LEN                            2U
#define DIR_FST_CLUS_LOW_LEN                        2U
#define DIR_FILE_SIZE_LEN                           4U

/* Define struct __attribute__ ((__packed__)) Entry Format */
#define DIR_NUMBER_OF_BYTES_PER_ENTRY               32U

typedef struct __attribute__ ((__packed__)) _ENTRY_FORMAT
{
    uint8_t DIR_Name[DIR_NAME_LEN];                   /* 0-7   file name */
    uint8_t DIR_Ext[DIR_EXTENTION_LEN];               /* 8-10  file name extension */
    uint8_t DIR_Attr[DIR_ATTRIBUTES_LEN];             /* 11    file attributes */
    uint8_t DIR_NTRes[DIR_NTRES_LEN];                 /* 12-21 reserved */
    uint8_t DIR_CrtTimeTenth[DIR_CRT_TIME_TENTH_LEN]; /* 22-23 file time */
    uint8_t DIR_CrtTime[DIR_CRT_TIME_LEN];            /* 24-25 file date */
    uint8_t DIR_CrtDate[DIR_CRT_DATE_LEN];            /* 26-27 starting cluster number */
    uint8_t DIR_LstAccDate[DIR_LST_ACC_DATE_LEN];     /* 28-31 File size (0 for directories) */
    uint8_t DIR_FstClusHI[DIR_FST_CLUS_HIGH_LEN];     /* 28-31 File size (0 for directories) */
    uint8_t DIR_WrtTime[DIR_WRT_TIME_LEN];            /* 28-31 File size (0 for directories) */
    uint8_t DIR_WrtDate[DIR_WRT_DATE_LEN];            /* 28-31 File size (0 for directories) */
    uint8_t DIR_FstClusLO[DIR_FST_CLUS_LOW_LEN];      /* 28-31 File size (0 for directories) */
    uint8_t DIR_FileSize[DIR_FILE_SIZE_LEN];          /* 28-31 File size (0 for directories) */
} FATFS_EntryFormat_t;


/* Define Sub Node */
#define FATFS_SUB_PATH              0x2EU

/* Define File Atributes */
#define ATT_READ_ONLY               0x01U
#define ATT_HIDDEN_FILE             0x02U
#define ATT_SYSTEM_FILE             0x04U
#define ATT_VOLUME_LABEL            0x08U
#define ATT_LONG_FILE_NAME          0x0FU
#define ATT_DIRECTORY               0x10U
#define ATT_ARCHIVE_FLAG            0x20U

/* Define LFN */
#define LFN_SEQUENCE_NUMBER         1U
#define LFN_FILE_NAME_1             10U
#define LFN_FILE_ATTRIBUTES         1U
#define LFN_RESERVED_1              1U
#define LFN_CHECKSUM                1U
#define LFN_FILE_NAME_2             12U
#define LFN_RESERVED_2              2U
#define LFN_FILE_NAME_3             4U

/* struct __attribute__ ((__packed__))ure for LFN Entries format */
typedef struct __attribute__ ((__packed__)) _LFN_struct
{
    uint8_t LFN_sequenceNum[LFN_SEQUENCE_NUMBER];    /* 0    Sequence number and allocation status */
    uint8_t LFN_fileName1[LFN_FILE_NAME_1];          /* 1-10 File name characters(uincode) */
    uint8_t LFN_fileAttributes[LFN_FILE_ATTRIBUTES]; /* 11   File Attributes */
    uint8_t LFN_reserved1[LFN_RESERVED_1];           /* 12   Reserved */
    uint8_t LFN_checkSum[LFN_CHECKSUM];              /* 13   Checksum */
    uint8_t LFN_fileName2[LFN_FILE_NAME_2];          /* 14*/
    uint8_t LFN_reserved2[LFN_RESERVED_2];           /* */
    uint8_t LFN_fileName3[LFN_FILE_NAME_3];          /* */
} FATFS_LFNtypes_t;

/* struct __attribute__ ((__packed__))ure for name of file type LFN*/
typedef struct __attribute__ ((__packed__)) _LFN_NAME
{
    uint8_t filename1;
    uint8_t filename2;
    uint8_t filename3;
} FATFS_LFN_NameTypes_t;

/* Define FileName Atributes */
#define FATFS_FILENAME_NEVER_USED            0x00U
#define FATFS_FILENAME_DELETED_1             0xE5U
#define FATFS_FILENAME_DELETED_2             0x05U
#define FATFS_FILENAME_SPECIAL_ENTRY         0x2EU

/* Bit Mask of hours, minutes, seconds*/
#define FATFS_BIT_MASK_OF_HOURS              0XF800U
#define FATFS_BIT_MASK_OF_MINUTES            0x7E0U
#define FATFS_BIT_MASK_OF_SECONDS            0x1FU

/* Shift bit mask Hour, Minute, Second */
#define FATFS_SHIFT_HOURS                    0xBU
#define FATFS_SHIFT_MINUTES                  0x5U
#define FATFS_SHIFT_SECONDS                  0x00U

/* Calculate hours, minutes, seconds */
#define FATFS_CALC_HOURS(x) (((FATFS_BIT_MASK_OF_HOURS) & (x)) >> (FATFS_SHIFT_HOURS))
#define FATFS_CALC_MINUTES(x) (((FATFS_BIT_MASK_OF_MINUTES) & (x)) >> (FATFS_SHIFT_MINUTES))
#define FATFS_CALC_SECONDS(x) ((FATFS_BIT_MASK_OF_SECONDS) & (x) >> (FATFS_SHIFT_SECONDS))

/* Bit mask of year, month, day */
#define FATFS_BIT_MASK_OF_YEAR               0xFE00U
#define FATFS_BIT_MASK_OF_MONTH              0x1E0U
#define FATFS_BIT_MASK_OF_DAY                0x1FU

/* Shif bit mask year, month, day */
#define FATFS_SHIFT_YEAR                     9U
#define FATFS_SHIFT_MONTH                    5U
#define FATFS_SHIFT_DAY                      0x00U

#define BIN_YEAR_OFFSET_FROM                1980U
/* Calculate year, month, day */
#define FATFS_CALC_YEAR(y) (((FATFS_BIT_MASK_OF_YEAR) & (y)) >> (FATFS_SHIFT_YEAR))
#define FATFS_CALC_MONTH(y) (((FATFS_BIT_MASK_OF_MONTH) & (y)) >> (FATFS_SHIFT_MONTH))
#define FATFS_CALC_DAY(y) (((FATFS_BIT_MASK_OF_DAY) & (y)) >> (FATFS_SHIFT_DAY))

/* Define Enum FAT Types */
typedef enum _FATFS_TYPES
{
    ERROR = -1,
    FAT12 = 0U,
    FAT16 = 1U,
    FAT32 = 2U
} FATFS_FatTypes_t;

#define FATFS_CAL_SECTOR_INDEX(x)   ((x) >> 4U) /* Calculate index sector of root directory */

/*** Function Prototype */
/* This function is read FAT files */
int32_t FATFS_InitFile();
/* This function is read directory entry*/
int32_t FATFS_readDirectory(Node ** NodeData, bool isRoot);
/* This function is to read a file in FAT12 files */
int32_t FATFS_readFile(uint8_t * buff, uint32_t entryOfCLuster);



#endif /* _FATFS_DRIVE_H_ */
