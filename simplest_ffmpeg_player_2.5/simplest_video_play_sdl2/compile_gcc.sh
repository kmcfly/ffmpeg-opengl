#! /bin/sh
#��򵥵�SDL2������Ƶ�����ӣ�SDL2����RGB/YUV��----�����б���
#Simplest Video Play SDL2 (SDL2 play RGB/YUV) ----Compile in Shell 
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
gcc simplest_video_play_sdl2.cpp -g -o simplest_video_play_sdl2.out \
-I /usr/local/include -L /usr/local/lib -lSDL2main -lSDL2
