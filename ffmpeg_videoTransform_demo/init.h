//////////////////////////////////////////////////////////////////////////
//设备初始化程序
//2015年2月8日14:05:51
//////////////////////////////////////////////////////////////////////////
#ifndef _INIT_H
#define _INIT_H


#include <Windows.h>
#include <iostream>
#include "SDL_ttf.h"
using namespace std;
int thread_exit=0;
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

extern "C"
{
	//#include ""
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include "libavutil\opt.h"
#include "libavcodec\avcodec.h"
#include "libavutil\mathematics.h"
#include "libavutil\time.h"

	//SDL
#include "sdl/SDL.h"
#include "sdl/SDL_thread.h"
};

//定义全局变量
int thread_encode_flag=0;	//定义编码全局标志
int thread_flush_flag=0;	//定义flush全局标志
int thread_display_flag=0;	//定义刷新显示全局标志



int flush_encoder(AVFormatContext *fmt_ctx,unsigned int stream_index){
	int ret;
	int got_frame;
	AVPacket enc_pkt;
	if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities &CODEC_CAP_DELAY))
		return 0;
	while (1) {
		enc_pkt.data = NULL;
		enc_pkt.size = 0;
		av_init_packet(&enc_pkt);
		ret = avcodec_encode_video2 (fmt_ctx->streams[stream_index]->codec, &enc_pkt,NULL, &got_frame);
		av_frame_free(NULL);
		if (ret < 0)
			break;
		if (!got_frame)
		{
			ret=0;
			break;
		}
		printf("Flush Encoder: Succeed to encode 1 frame!\tsize:%5d\n",enc_pkt.size);
		/* mux encoded frame */
		ret = av_write_frame(fmt_ctx, &enc_pkt);	
	}
	return ret;
}

//SDL刷新线程
int sfp_refresh_thread(void *opaque)
{
	while (thread_exit==0) {
		SDL_Event event;
		event.type = SFM_REFRESH_EVENT;
		SDL_PushEvent(&event);
		SDL_Delay(125);//创建了一个线程，每隔40ms发送一个自定义的消息，告知主函数进行解码显示  fps=10

	}
	return 0;
}


//Show Device  显示设备名称
void show_dshow_device(){
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVDictionary* options = NULL;
	av_dict_set(&options,"list_devices","true",0);
	AVInputFormat *iformat = av_find_input_format("dshow");
	printf("Device Info=============\n");
	avformat_open_input(&pFormatCtx,"video=hehe",iformat,&options);		//打开输入流
	avformat_close_input(&pFormatCtx);									//关闭输入流
	printf("========================\n\n\n");
}

//Show Device Option  显示设备可配置参数
void show_dshow_device_option(){
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVDictionary* options = NULL;
	av_dict_set(&options,"list_options","true",0);
	av_dict_set(&options,"video_size","640x480",0);
	//av_dict_set(&options,"fps","30",0);
	//av_opt_set();
	AVInputFormat *iformat = av_find_input_format("dshow");
	printf("Device Option Info======\n");
	avformat_open_input(&pFormatCtx,"video=QuickCam Orbit/Sphere AF",iformat,&options);
	avformat_close_input(&pFormatCtx);									//关闭输入流
	printf("========================\n");
}

//Show VFW Device  显示VFW参数
void show_vfw_device(){
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVInputFormat *iformat = av_find_input_format("vfwcap");
	printf("VFW Device Info======\n");
	avformat_open_input(&pFormatCtx,"list",iformat,NULL);
	printf("=====================\n\n\n");
}


void ApplySurface(int x, int y, SDL_Surface* pSrc, SDL_Surface* pDest)  
{  
	SDL_Rect rect;  

	rect.x = x;  
	rect.y = y;  
	rect.w = pSrc->w;  
	rect.h = pSrc->h;  
	SDL_BlitSurface(pSrc, NULL, pDest, &rect);  
}  
#endif