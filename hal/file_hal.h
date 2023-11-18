/*** HW LAYER ***/
#ifndef _FILE_HAL_H_
#define _FILE_HAL_H_

#include <stdint.h>

/* This Function is used to open file */
int32_t HAL_FileInit(const char * const filePath);

/* This Function is used to close file */
int32_t HAL_DeInit(void);

/* Function is used to read a sector */
int32_t HAL_ReadSector(uint32_t index, uint8_t *buff);

/* Function is used to read multi sector */
int32_t HAL_ReadMultiSector(uint32_t index, uint32_t num, uint8_t *buff);

#endif /*_FILE_HAL_H_*/
