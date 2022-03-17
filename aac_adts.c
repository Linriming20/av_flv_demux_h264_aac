#include "aac_adts.h"


int generateAdtsHeader(uint32_t dataSize, uint8_t *pOutBuf, uint32_t *pOutBufLen) 
{
    int id = 0; // 0:MPEG_4
    int crc = 1;
    int profile = 1; // AAC LC
    int freqIdx = 0x4; // 44.1KHz
    int channel = 2; // Stereo

	if(!dataSize || !pOutBuf || !pOutBufLen)
	{
		printf("[%s:%d] Params invalid!\n", __FUNCTION__, __LINE__);
		return -1;
	}

    // fill in ADTS data
    pOutBuf[0] = (uint8_t) 0xFF;
    pOutBuf[1] = (uint8_t) (0xF0 | (id << 3) | crc);
    pOutBuf[2] = (uint8_t) ((profile << 6) |\
                            (freqIdx << 2)  |\
                            (channel >> 2));
    pOutBuf[3] = (uint8_t) (((channel & 0x03) << 6) | ((dataSize & 0x1800) >> 11));
    pOutBuf[4] = (uint8_t) ((dataSize & 0x7F8) >> 3);
    pOutBuf[5] = (uint8_t) (((dataSize & 0x07) << 5) | 0x1F);
    pOutBuf[6] = (uint8_t) 0xFC;

    *pOutBufLen = 7;

	return 0;
}


