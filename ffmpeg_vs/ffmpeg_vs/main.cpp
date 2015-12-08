#include "./libavformat/avformat.h"

#include "cmdutils.h"

#include "Windows.h" 


#include <stdio.h>
#include "Src/Opengl.h"

CDisplayOpenGl g_CDisplayOpengl;

int CreateBmp(const char *filename, uint8_t *pRGBBuffer, int width, int height, int bpp)  
{  
	 BITMAPFILEHEADER bmpheader;  
	 BITMAPINFOHEADER bmpinfo;  
	 FILE *fp = NULL;  
	  
	 fp = fopen(filename,"wb");  
	 if( fp == NULL )  
	 {  
	  return -1;  
	 }  
	  
	 bmpheader.bfType = ('M' <<8)|'B';  
	 bmpheader.bfReserved1 = 0;  
	 bmpheader.bfReserved2 = 0;  
	 bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
	 bmpheader.bfSize = bmpheader.bfOffBits + width*height*bpp/8;  
	  
	 bmpinfo.biSize = sizeof(BITMAPINFOHEADER);  
	 bmpinfo.biWidth = width;  
	 bmpinfo.biHeight = 0 - height;  
	 bmpinfo.biPlanes = 1;  
	 bmpinfo.biBitCount = bpp;  
	 bmpinfo.biCompression = BI_RGB;  
	 bmpinfo.biSizeImage = 0;  
	 bmpinfo.biXPelsPerMeter = 100;  
	 bmpinfo.biYPelsPerMeter = 100;  
	 bmpinfo.biClrUsed = 0;  
	 bmpinfo.biClrImportant = 0;  
	  
	 fwrite(&bmpheader,sizeof(BITMAPFILEHEADER),1,fp);  
	 fwrite(&bmpinfo,sizeof(BITMAPINFOHEADER),1,fp);  
	 fwrite(pRGBBuffer,width*height*bpp/8,1,fp);  
	 fclose(fp);  
	 fp = NULL;  
	  
	 return 0;  
}  
int SaveVideoFile(const char *fileName,uint8_t *pRgbBuffer, int width, int height, int bpp)
{
	FILE *fp = NULL;  
	  
	 fp = fopen(fileName,"at+");  
	 if( fp == NULL )  
	 {  
	  return -1;  
	 }  
	    
	 fwrite(pRgbBuffer,width*height*bpp/8,1,fp);  
	 fclose(fp);  
	 fp = NULL;  
	  
	 return 0;  
}

//OPengl 显示线程
DWORD WINAPI DisplayVideo(HANDLE Thread)
{
	DWORD dwThreadId;
	unsigned int iCout = 0;

	g_CDisplayOpengl.CreateGLWindow(&g_CDisplayOpengl);
			

	return 0;
}
DWORD WINAPI DecodeVideo(HANDLE Thread)
{
		//打开视频文件
	AVFormatContext *pFormatCtx;
	const char *filename = "video.avi";
	int videoStream=-1;
	int i;
	AVFrame *pFrameRGB;
	AVFrame *pFrame;

	
	uint8_t *buffer;
	int numBytes;

	AVCodec *pCodec;
	AVCodecContext *pCodecCtx;

	int frameFinished;
    AVPacket packet;

	unsigned int bmpi=0;
	char BmpName[40] ={0};
	//注册所有的编解码器
	//av_register_all();
	avcodec_init();
	av_register_all();
	avcodec_register_all();
	
	while(1)
		{
		 //打开文件
		if(av_open_input_file(&pFormatCtx,filename,NULL,0,NULL)!=0)
			{//这一步会用有效的信息把 AVFormatContext填满
				printf("####error: av_Open_input_file error \n");
				return 0;
			}
		// Retrieve stream information
		if(av_find_stream_info(pFormatCtx)<0)
			return -1; // Couldn't find stream information
		//输出文件的音视频流信息
		dump_format(pFormatCtx, 0, filename, 0);
	
		videoStream=-1;
		for(i=0; i<pFormatCtx->nb_streams; i++)
			{
				 if(pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO)//CODEC_TYPE_VIDEO
					{
					 videoStream=i;
						break;
					}
			}
		if(videoStream==-1)
		 return -1; // Didn't find a video stream
	
		 //找到解码器
		pCodecCtx=pFormatCtx->streams[videoStream]->codec;
		pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
		
		if(pCodec==NULL) 
			{
				fprintf(stderr, "Unsupported codec!\n");
				return -1; // Codec not found
			}
		if(pCodec->capabilities&CODEC_CAP_TRUNCATED)
			pCodecCtx->flags|=CODEC_FLAG_TRUNCATED;
		// 打开解码器
		if(avcodec_open(pCodecCtx, pCodec)<0)
		  return -1; // Could not open codec
	
		 
	
	
	
	
		// Allocate video frame
		pFrame=avcodec_alloc_frame();
		// Allocate an AVFrame structure
		pFrameRGB=avcodec_alloc_frame();
		if(pFrameRGB==NULL)
			return -1;
		// Determine required buffer size and allocate buffer
		numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,pCodecCtx->height);
		buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
	
		// Assign appropriate parts of buffer to image planes in pFrameRGB
	// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
	// of AVPicture
		avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,pCodecCtx->width, pCodecCtx->height);
	
	//	CreateBmp("SaveBmp.bmp", buffer,pCodecCtx->width, pCodecCtx->height, 24);
		i=0;
	
	while(av_read_frame(pFormatCtx, &packet)>=0) 
		{
			  //是视频帧吗
			  if(packet.stream_index==videoStream) 
				{
				//ffmpeg中的avcodec_decode_video2()的作用是解码一帧视频数据。输入一个压缩编码的结构体AVPacket，输出一个解码后的结构体AVFrame
				avcodec_decode_video(pCodecCtx, pFrame, &frameFinished,packet.data, packet.size);
			   
				// Did we get a video frame?
				if(frameFinished) 
					{
						//yuv数据
						pFrame->data[0] += pFrame->linesize[0] * (pCodecCtx->height - 1);
					    pFrame->linesize[0] *= -1;
					    pFrame->data[1] += pFrame->linesize[1] * (pCodecCtx->height / 2 - 1);
					    pFrame->linesize[1] *= -1;
					    pFrame->data[2] += pFrame->linesize[2] * (pCodecCtx->height / 2 - 1);
					    pFrame->linesize[2] *= -1;
					    
					 //   sws_scale (pSwsCtx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
					// Convert the image from its native(YUV FMT) format to RGB
						img_convert((AVPicture *)pFrameRGB, PIX_FMT_BGR24,(AVPicture*)pFrame, pCodecCtx->pix_fmt,pCodecCtx->width, pCodecCtx->height);
	
						//这个函数的使用本质上是为已经分配的空间的结构体AVPicture挂上一段用于保存数据的空间，这个结构体中有一个指针数组data[4]，挂在这个数组里
			//			avpicture_fill((AVPicture *)pFrame, buffer, PIX_FMT_BGR24,pCodecCtx->width, pCodecCtx->height);
						
						sprintf(BmpName,"%s_%d.bmp","SaveBmp",bmpi);
						bmpi++;
	//					CreateBmp(BmpName, buffer,pCodecCtx->width, pCodecCtx->height, 24);
						CDisplayOpenGl::ImageWidth = pCodecCtx->width;
						CDisplayOpenGl::ImageHeight = pCodecCtx->height;
						CDisplayOpenGl::PixelData = buffer;

						CDisplayOpenGl::m_bReDrawFlag = true;
						Sleep(100);
						//SaveVideoFile("SaveVideo.rgb24", buffer,pCodecCtx->width, pCodecCtx->height, 24);
						// Save the frame to disk
					   // if(++i<=5)
						//	SaveFrame(pFrameRGB, pCodecCtx->width,
						 // 		  pCodecCtx->height, i);
					}
			  }
	
				// Free the packet that was allocated by av_read_frame
			  av_free_packet(&packet);
		}
		avcodec_close(pCodecCtx);
		av_close_input_file(pFormatCtx);
		}
	return 0;	
}

int main(int argc,char **argv)
{

	//创建显示
	DWORD dwThreadId;
	CreateThread(NULL,0,DisplayVideo,NULL,0,&dwThreadId);

	CreateThread(NULL,0,DecodeVideo,NULL,0,&dwThreadId);
	
	while(1)
	{
		Sleep(1000);
	}

}

