#! /bin/sh
#��򵥵Ļ���FFmpeg����Ƶ�������������棩----�����б���
#Simplest FFmpeg Decoder Pure----Compile in Shell 
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
gcc simplest_ffmpeg_decoder_pure.cpp -g -o simplest_ffmpeg_decoder_pure.out -I /usr/local/include -L /usr/local/lib \
-lavcodec -lavutil -lswscale
