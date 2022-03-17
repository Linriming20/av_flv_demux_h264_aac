#ifndef __FLV_H__
#define __FLV_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "flv_format.h"

#ifdef ENABLE_DEBUG
    #define DEBUG(fmt, args...)     printf(fmt, ##args)
#else
    #define DEBUG(fmt, args...)
#endif


#define MAX_FLV_BUF_SIZE    (1*1024*1024) 	/* 1 MBytes */
#define MAX_ADTS_SIZE 			   (1024) 	/* 1K Bytes */
#define MAX_NALU_SIZE   	(1*1024*1024) 	/* 1M Bytes */



int flv_demux_h264_aac(const char *flvFileName);



#endif /* __FLV_H__ */

