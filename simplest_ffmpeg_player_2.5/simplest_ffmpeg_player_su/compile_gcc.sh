#! /bin/sh
#��򵥵Ļ���FFmpeg����Ƶ������2(SDL������)----�����б���
#Simplest FFmpeg Player 2(SDL Update)----Compile in Shell 
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
gcc simplest_ffmpeg_player_su.cpp -g -o simplest_ffmpeg_player_su.out -I /usr/local/include -L /usr/local/lib \
-lSDL2main -lSDL2 -lavformat -lavcodec -lavutil -lswscale
