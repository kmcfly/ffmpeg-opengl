::��򵥵Ļ���FFmpeg����Ƶ�������������棩----�����б���
::Simplest FFmpeg Decoder Pure----Compile in Cmd 
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
cl simplest_ffmpeg_decoder_pure.cpp /link avcodec.lib avutil.lib swscale.lib ^
/OPT:NOREF
exit