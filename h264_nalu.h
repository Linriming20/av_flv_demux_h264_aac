#ifndef __H264_H__
#define __H264_H__


typedef enum{
	NALU_TYPE_SLICE    = 0x1,
	NALU_TYPE_DPA      = 0x2,
	NALU_TYPE_DPB      = 0x3,
	NALU_TYPE_DPC      = 0x4,
	NALU_TYPE_IDR      = 0x5,
	NALU_TYPE_SEI      = 0x6,
	NALU_TYPE_SPS      = 0x7,
	NALU_TYPE_PPS      = 0x8,
	NALU_TYPE_AUD      = 0x9,
	NALU_TYPE_EOSEQ    = 0xa,
	NALU_TYPE_EOSTREAM = 0xb,
	NALU_TYPE_FILL     = 0xc,
}h264_nalu_type_e;



#endif /* __H264_H__ */
