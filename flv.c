#include "h264_nalu.h"
#include "aac_adts.h"
#include "flv.h"


int flv_demux_h264_aac(const char *flvFileName)
{
	FILE *fpH264 = NULL;
	FILE *fpAAC  = NULL;
	FILE *fpFLV  = NULL;
	uint8_t *flvBuf = NULL;
	// note!!!: it maybe happen the 'Segmentation fault', as 1024 is too long for 'sps', but it maybe save the 'sei' or other unused data if first nalu isn't 'sps'.
	uint8_t spsBuf[64] = {0};
	uint8_t ppsBuf[64] = {0};
	uint32_t spsLen = 0;
	uint32_t ppsLen = 0;
	int ret = -1;
	uint32_t flvTagDataSize = 0;
	PT_FlvTag ptFlvTag = NULL;

	const char *h264FileName = "out_video.h264";
	const char *aacFileName = "out_audio.aac";

	/* open file */
	fpFLV  = fopen(flvFileName,  "rb");
	if (!fpFLV)
	{
		printf("open %s error!\n", flvFileName);
	}
	fpH264 = fopen(h264FileName, "wb");
	if (!fpH264)
	{
		printf("open %s error!\n", h264FileName);
	}
	fpAAC  = fopen(aacFileName,  "wb");
	if (!fpAAC)
	{
		printf("open %s error!\n", aacFileName);
	}

	/* alloc memory */
	flvBuf = (uint8_t *)malloc(MAX_FLV_BUF_SIZE);
	if (!flvBuf)
	{
		printf("malloc for 'flvBuf' error!\n");
	}

	/* parse and save some data first. */
	fread(flvBuf, SIZE_FLV_HEADER, 1, fpFLV);
	ret = strncmp((const char *)flvBuf, "FLV", 3);
	if(ret != 0)
	{
		printf("\033[31m It's not a FLV file!\n\033[0m");
		return -1;
	}
	DEBUG("\033[32m"
		  "=====================================\n"
		  "Signature: '%c' '%c' '%c'\n"
		  "Version  : 0x%02x\n"
		  "Audio    : %s\n"
		  "Video    : %s\n"
		  "=====================================\n"
		  "\033[0m",
		  flvBuf[0], flvBuf[1], flvBuf[2], flvBuf[3], (flvBuf[4] & 0x04) ? "yes" : "no", (flvBuf[4] & 0x01) ? "yes" : "no");

	fread(flvBuf, SIZE_PREVIOUS_TAG_SIZE, 1, fpFLV);
	DEBUG("\033[37m[Pre Tag Size]: %d \033[0m\n", (flvBuf[0] << 24) |\
												   (flvBuf[1] << 16) |\
												   (flvBuf[2] <<  8) |\
												    flvBuf[3]);

	while(1)
	{
		ret = fread(flvBuf, SIZE_FLV_TAG_HEADER, 1, fpFLV);
		if (ret <= 0)
		{
			DEBUG(">>> FLV file end! <<<\n");
			break;
		}
		ptFlvTag = (PT_FlvTag)flvBuf;

		flvTagDataSize = (ptFlvTag->flvheader.DataSize[0] << 16) |\
						 (ptFlvTag->flvheader.DataSize[1] << 8) |\
						  ptFlvTag->flvheader.DataSize[2];
		fread(ptFlvTag->flvdata, flvTagDataSize, 1, fpFLV);

		switch (ptFlvTag->flvheader.TagType)
		{
			case FLVTAG_TYPE_SCRIPT: {
				DEBUG("\033[34m[Script Tag] \033[0m\n");
				/* nothing to do! */
				break;
			}
			case FLVTAG_TYPE_AUDIO: {
				char *packetTypeStr = NULL;
				int aacPacketType = ptFlvTag->flvdata[1];

				switch(aacPacketType)
				{
					case AAC_PACKET_TYPE_SEQUENCE_HEADER:
						packetTypeStr = "[AAC sequence header]";
						/* nothing to do! */
						break;
					case AAC_PACKET_TYPE_RAW:
						packetTypeStr = "[AAC raw]";

						uint8_t headerbuf[AAC_ADTS_HEADER_SIZE] = {0};
						uint32_t headerbufLen = 0;
						uint32_t datasize = ((ptFlvTag->flvheader.DataSize[0] << 16) |\
											 (ptFlvTag->flvheader.DataSize[1] << 8) |\
											 ptFlvTag->flvheader.DataSize[2]);
						generateAdtsHeader(AAC_ADTS_HEADER_SIZE + (datasize - 2), headerbuf, &headerbufLen);

						fwrite(headerbuf, headerbufLen, 1, fpAAC);
						fwrite(&ptFlvTag->flvdata[2], datasize - 2, 1, fpAAC);
						break;
					default:
						printf("[%s:%d] Unknown AAC packet type!", __FUNCTION__, __LINE__);
						goto exit;
				}
				DEBUG("\033[36m[Audio Tag] \t %s\033[0m\n", packetTypeStr);
				break;
			}
			case FLVTAG_TYPE_VIDEO: {
				char *packetTypeStr = NULL;
				char *frameTypeStr = NULL;
				uint8_t startCode[4] = {0x00, 0x00, 0x00, 0x01};
				int isKeyFrame = 0;
				int type = 0;

				isKeyFrame = (ptFlvTag->flvdata[0] & 0xF0) == 0x10 ? 1 : 0;
				type = ptFlvTag->flvdata[1];

				switch(type)
				{
					case AVC_PACKET_TYPE_SEQUENCE_HEADER:
						packetTypeStr = "[AVC squence header]";
						frameTypeStr = "[key frame]";

						spsLen = ((ptFlvTag->flvdata[11] << 8) | ptFlvTag->flvdata[12]);
						memcpy(spsBuf, &ptFlvTag->flvdata[13], spsLen);

						//printf("spsLen: %d\n", spsLen);
						//for(int i = 0; i < spsLen; i++)
						//	printf("0x%02x ", spsBuf[i]);
						//printf("\n");

						ppsLen = ((ptFlvTag->flvdata[13 + spsLen + 1] << 8) | ptFlvTag->flvdata[13+spsLen+2]);
						memcpy(ppsBuf, &ptFlvTag->flvdata[13 + spsLen + 3], ppsLen);

						//printf("ppsLen: %d\n", ppsLen);
						//for(int i = 0; i < ppsLen; i++)
						//	printf("0x%02x ", ppsBuf[i]);
						//printf("\n");

						break;
					case AVC_PACKET_TYPE_NALU:
						packetTypeStr = "[AVC NALU]";

						/* one or more NALU. */
						uint32_t naluDataSize = 0;
						uint32_t curPos = 0;

						curPos = 5;
						naluDataSize = (ptFlvTag->flvdata[curPos + 0] << 24) |\
									   (ptFlvTag->flvdata[curPos + 1] << 16) |\
									   (ptFlvTag->flvdata[curPos + 2] << 8) |\
										ptFlvTag->flvdata[curPos + 3];
						if(isKeyFrame)
						{
							frameTypeStr = "[key frame]";

							if((ptFlvTag->flvdata[curPos + 4] & 0x1f) == NALU_TYPE_SEI &&\
							   (ptFlvTag->flvdata[curPos + 5] & 0x1f) == 0x05/* SEI: payloadType: user_data_unregistered() */)
							{
								/* has SEI */
								/* SEI */
								fwrite(startCode, sizeof(startCode), 1, fpH264);
								fwrite(&ptFlvTag->flvdata[curPos + 4], naluDataSize, 1, fpH264);

								/* SPS */
								fwrite(startCode, sizeof(startCode), 1, fpH264);
								fwrite(spsBuf, spsLen, 1, fpH264);

								/* PPS */
								fwrite(startCode, sizeof(startCode), 1, fpH264);
								fwrite(ppsBuf, ppsLen, 1, fpH264);

								/* update the second NALU data size. */
								curPos += (4 + naluDataSize);
								naluDataSize = (ptFlvTag->flvdata[curPos + 0] << 24) |\
											   (ptFlvTag->flvdata[curPos + 1] << 16) |\
											   (ptFlvTag->flvdata[curPos + 2] << 8) |\
												ptFlvTag->flvdata[curPos + 3];
								/* IDR */
								fwrite(startCode, sizeof(startCode), 1, fpH264);
								fwrite(&ptFlvTag->flvdata[curPos + 4], naluDataSize, 1, fpH264);
							}
							else
							{
								/* SPS */
								fwrite(startCode, sizeof(startCode), 1, fpH264);
								fwrite(spsBuf, spsLen, 1, fpH264);

								/* PPS */
								fwrite(startCode, sizeof(startCode), 1, fpH264);
								fwrite(ppsBuf, ppsLen, 1, fpH264);

								/* IDR */
								fwrite(startCode, sizeof(startCode), 1, fpH264);
								fwrite(&ptFlvTag->flvdata[curPos + 4], naluDataSize, 1, fpH264);
							}

						}
						else
						{
							frameTypeStr = "[inter frame]";

							/* SLICE */
							fwrite(startCode, sizeof(startCode), 1, fpH264);
							fwrite(&ptFlvTag->flvdata[curPos + 4], naluDataSize, 1, fpH264);
						}
						break;
					case AVC_PACKET_TYPE_END_OF_SEQUENCE:
						packetTypeStr = "[AVC end of squence]";
						frameTypeStr = "[key frame]";
						break;
					default:
						printf("[%s:%d] Unknown AVC packet type!", __FUNCTION__, __LINE__);
						goto exit;
				}
				DEBUG("\033[35m[Video Tag] \t %s \t %s\033[0m\n", packetTypeStr, frameTypeStr);
				break;
			}
			default: {
				printf("\033[33m[Unknown Tag]\033[0m\n");
				break;
			}
		}

		fread(flvBuf, SIZE_PREVIOUS_TAG_SIZE, 1, fpFLV);
		DEBUG("\033[37m[PreTagSize] \t %d\033[0m\n", (flvBuf[0] << 24) |\
													 (flvBuf[1] << 16) |\
													 (flvBuf[2] <<	8) |\
													  flvBuf[3]);

	}

exit:
	if(fpFLV) 	fclose(fpFLV);
	if(fpH264) 	{fflush(fpH264); fclose(fpH264);}
	if(fpAAC) 	{fflush(fpAAC); fclose(fpAAC);}
	if(flvBuf)  free(flvBuf);

	return 0;
}

