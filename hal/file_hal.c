/* INCLUDE */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "file_hal.h"
/* INCLUDE */

/* DEFINITION */
#define HAL_DEFAULT_SECTOR_SIZE 512
/* DEFINITION */

/* GLOBAL VARIABLE */
FILE * g_fp = NULL; /* file pointer global */
const uint8_t * FileName;
/* GLOBAL VARIABLE */

/* CODE */
/* This Function is used to open file */
int32_t HAL_FileInit(const char * const filePath)
{
    int32_t retVal = -1; /* -1 is ERROR value */
    g_fp = fopen(filePath, "rb");
    if(g_fp != NULL)
    {
        retVal = 0; /* 0 is OK value */
    }
    return retVal;
}
/* This Function is used to close file */
int32_t HAL_DeInit(void)
{
    int32_t retVal = -1; /* -1 is ERROR value */
    if(g_fp != NULL)
    {
        fclose(g_fp);
        retVal = 0; /* 0 is OK value */
    }
    return retVal;
}
/* Function is used to read a sector */
int32_t HAL_ReadSector(uint32_t index, uint8_t * buff)
{
    int32_t retVal = -1; /* -1 is ERROR value */
    index *= 512;
    rewind(g_fp);
    
    if (g_fp != NULL)
    {
        if(fseek(g_fp, index, SEEK_SET) == 0)
        {
            fread(buff, 512, 1, g_fp); /* size of 1 sector = 512 byte*/
            retVal = 0; /* 0 is OK value */
        }
    }
    return retVal;
}
/* Function is used to read multi sector */
int32_t HAL_ReadMultiSector(uint32_t index, uint32_t num, uint8_t *buff)
{
    int32_t retVal = -1; /* -1 is ERROR value */
    uint32_t i = 0;
    for (; i < num; i++)
    {
        HAL_ReadSector(index, buff);
        retVal = 0; /* 0 is OK value */
    }
    return retVal;
}

/* CODE */
