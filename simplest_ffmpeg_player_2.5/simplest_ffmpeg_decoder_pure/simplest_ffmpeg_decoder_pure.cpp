/**
 * ��򵥵Ļ���FFmpeg����Ƶ�������������棩
 * Simplest FFmpeg Decoder Pure
 *
 * ������ Lei Xiaohua
 * leixiaohua1020@126.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 *
 * ������ʵ������Ƶ����(֧��HEVC��H.264��MPEG2��)����ΪYUV���ݡ�
 * ������ʹ����libavcodec����û��ʹ��libavformat����
 * ����򵥵�FFmpeg��Ƶ���뷽��Ľ̡̳�
 * ͨ��ѧϰ�����ӿ����˽�FFmpeg�Ľ������̡�
 * This software is a simplest decoder based on FFmpeg.
 * It decode bitstreams to YUV pixel data.
 * It just use libavcodec (do not contains libavformat).
 * Suitable for beginner of FFmpeg.
 */

#include <stdio.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
};
#endif
#endif

#define USE_SWSCALE 0

//test different codec
#define TEST_H264  1
#define TEST_HEVC  0

int main(int argc, char* argv[])
{
	AVCodec *pCodec;
    AVCodecContext *pCodecCtx= NULL;
	AVCodecParserContext *pCodecParserCtx=NULL;

    FILE *fp_in;
	FILE *fp_out;
    AVFrame	*pFrame;
	
	const int in_buffer_size=4096;
	uint8_t in_buffer[in_buffer_size + FF_INPUT_BUFFER_PADDING_SIZE]={0};
	uint8_t *cur_ptr;
	int cur_size;
    AVPacket packet;
	int ret, got_picture;
	int y_size;


#if TEST_HEVC
	enum AVCodecID codec_id=AV_CODEC_ID_HEVC;
	char filepath_in[]="bigbuckbunny_480x272.hevc";
#elif TEST_H264
	AVCodecID codec_id=AV_CODEC_ID_H264;
	char filepath_in[]="bigbuckbunny_480x272.h264";
#else
	AVCodecID codec_id=AV_CODEC_ID_MPEG2VIDEO;
	char filepath_in[]="bigbuckbunny_480x272.m2v";
#endif

	char filepath_out[]="bigbuckbunny_480x272.yuv";
	int first_time=1;

#if USE_SWSCALE
	struct SwsContext *img_convert_ctx;
	AVFrame	*pFrameYUV;
	uint8_t *out_buffer;

#endif
	//av_log_set_level(AV_LOG_DEBUG);
	
	avcodec_register_all();

    pCodec = avcodec_find_decoder(codec_id);
    if (!pCodec) {
        printf("Codec not found\n");
        return -1;
    }
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx){
        printf("Could not allocate video codec context\n");
        return -1;
    }

	pCodecParserCtx=av_parser_init(codec_id);
	if (!pCodecParserCtx){
		printf("Could not allocate video parser context\n");
		return -1;
	}

    //if(pCodec->capabilities&CODEC_CAP_TRUNCATED)
    //    pCodecCtx->flags|= CODEC_FLAG_TRUNCATED; /* we do not send complete frames */
    
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Could not open codec\n");
        return -1;
    }
	//Input File
    fp_in = fopen(filepath_in, "rb");
    if (!fp_in) {
        printf("Could not open input stream\n");
        return -1;
    }
	//Output File
	fp_out = fopen(filepath_out, "wb");
	if (!fp_out) {
		printf("Could not open output YUV file\n");
		return -1;
	}

    pFrame = av_frame_alloc();
	av_init_packet(&packet);

	while (1) {

        cur_size = fread(in_buffer, 1, in_buffer_size, fp_in);
        if (cur_size == 0)
            break;
        cur_ptr=in_buffer;

        while (cur_size>0){

			int len = av_parser_parse2(
				pCodecParserCtx, pCodecCtx,
				&packet.data, &packet.size,
				cur_ptr , cur_size ,
				AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

			cur_ptr += len;
			cur_size -= len;

			if(packet.size==0)
				continue;

			//Some Info from AVCodecParserContext
			printf("[Packet]Size:%6d\t",packet.size);
			switch(pCodecParserCtx->pict_type){
				case AV_PICTURE_TYPE_I: printf("Type:I\t");break;
				case AV_PICTURE_TYPE_P: printf("Type:P\t");break;
				case AV_PICTURE_TYPE_B: printf("Type:B\t");break;
				default: printf("Type:Other\t");break;
			}
			printf("Number:%4d\n",pCodecParserCtx->output_picture_number);

			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, &packet);
			if (ret < 0) {
				printf("Decode Error.\n");
				return ret;
			}
			if (got_picture) {
#if USE_SWSCALE
				if(first_time){
					printf("\nCodec Full Name:%s\n",pCodecCtx->codec->long_name);
					printf("width:%d\nheight:%d\n\n",pCodecCtx->width,pCodecCtx->height);
					//SwsContext
					img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 
						pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 

					pFrameYUV=av_frame_alloc();
					out_buffer=(uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
					avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

					y_size=pCodecCtx->width*pCodecCtx->height;

					first_time=0;
				}
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, 
					pFrameYUV->data, pFrameYUV->linesize);

				fwrite(pFrameYUV->data[0],1,y_size,fp_out);     //Y 
				fwrite(pFrameYUV->data[1],1,y_size/4,fp_out);   //U
				fwrite(pFrameYUV->data[2],1,y_size/4,fp_out);   //V
#else
				int i=0;
				unsigned char* tempptr=NULL;
				tempptr=pFrame->data[0];
				for(i=0;i<pFrame->height;i++){
					fwrite(tempptr,1,pFrame->width,fp_out);     //Y 
					tempptr+=pFrame->linesize[0];
				}
				tempptr=pFrame->data[1];
				for(i=0;i<pFrame->height/2;i++){
					fwrite(tempptr,1,pFrame->width/2,fp_out);   //U
					tempptr+=pFrame->linesize[1];
				}
				tempptr=pFrame->data[2];
				for(i=0;i<pFrame->height/2;i++){
					fwrite(tempptr,1,pFrame->width/2,fp_out);   //V
					tempptr+=pFrame->linesize[2];
				}
#endif

				printf("Succeed to decode 1 frame!\n");
			}
		}

    }

	//Flush Decoder
    packet.data = NULL;
    packet.size = 0;
	while(1){
		ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, &packet);
		if (ret < 0) {
			printf("Decode Error.\n");
			return ret;
		}
		if (!got_picture)
			break;
		if (got_picture) {
			
#if USE_SWSCALE
			sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, 
				pFrameYUV->data, pFrameYUV->linesize);

			fwrite(pFrameYUV->data[0],1,y_size,fp_out);     //Y
			fwrite(pFrameYUV->data[1],1,y_size/4,fp_out);   //U
			fwrite(pFrameYUV->data[2],1,y_size/4,fp_out);   //V
#else
			int i=0;
			unsigned char* tempptr=NULL;
			tempptr=pFrame->data[0];
			for(i=0;i<pFrame->height;i++){
				fwrite(tempptr,1,pFrame->width,fp_out);     //Y 
				tempptr+=pFrame->linesize[0];
			}
			tempptr=pFrame->data[1];
			for(i=0;i<pFrame->height/2;i++){
				fwrite(tempptr,1,pFrame->width/2,fp_out);   //U
				tempptr+=pFrame->linesize[1];
			}
			tempptr=pFrame->data[2];
			for(i=0;i<pFrame->height/2;i++){
				fwrite(tempptr,1,pFrame->width/2,fp_out);   //V
				tempptr+=pFrame->linesize[2];
			}
#endif
			printf("Flush Decoder: Succeed to decode 1 frame!\n");
		}
	}

    fclose(fp_in);
	fclose(fp_out);

#if USE_SWSCALE
	sws_freeContext(img_convert_ctx);
	av_frame_free(&pFrameYUV);
#endif

	av_parser_close(pCodecParserCtx);

	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	av_free(pCodecCtx);

	return 0;
}

