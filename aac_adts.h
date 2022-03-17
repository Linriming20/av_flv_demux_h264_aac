#ifndef __AAC_ADTS_H__
#define __AAC_ADTS_H__

#include <stdio.h>
#include <stdint.h>


#define AAC_ADTS_HEADER_SIZE    (7)


/************************************************************************
 * function describe: generate adts header
 * params:
 *   [dataSize]: size of this aac-adts.(include size of aac-adts header)
 *   [pOutBuf]: the function will fill the aac-adts header data in it, so
 * 				must be alloced memory before call this function.(out)
 *   [pOutBufLen]: length of pOutBuf, is 7(Bytes).(out)
 * return: 0:success other:error
 ************************************************************************/
int generateAdtsHeader(uint32_t dataSize, uint8_t *pOutBuf, uint32_t *pOutBufLen);


#endif /*  __AAC_ADTS_H__ */
