::��򵥵�SDL2������Ƶ�����ӣ�SDL2����RGB/YUV��----�����б���
::Simplest Video Play SDL2 (SDL2 play RGB/YUV) ----Compile in Cmd 
::
::������ Lei Xiaohua
::leixiaohua1020@126.com
::�й���ý��ѧ/���ֵ��Ӽ���
::Communication University of China / Digital TV Technology
::http://blog.csdn.net/leixiaohua1020
::
::VS2010 Environment
call "D:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
::include
@set INCLUDE=include;%INCLUDE%
::lib
@set LIB=lib;%LIB%
::compile and link
cl simplest_video_play_sdl2.cpp /MD /link SDL2.lib SDL2main.lib /SUBSYSTEM:WINDOWS
exit