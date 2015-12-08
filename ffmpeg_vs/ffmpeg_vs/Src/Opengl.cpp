#include "Opengl.h"


#include <stdio.h>

#include <stdlib.h>

//#define NULL (void*)0

GLint	 CDisplayOpenGl::ImageWidth=0;
GLint	 CDisplayOpenGl::ImageHeight=0;
GLint	 CDisplayOpenGl::PixelLength=0;
GLubyte* CDisplayOpenGl::PixelData=NULL;
CDisplayOpenGl *CDisplayOpenGl::m_pDisplayGlObject=NULL;
AUX_RGBImageRec  *CDisplayOpenGl::m_bmp=NULL;
bool CDisplayOpenGl::m_bReDrawFlag = false;

CDisplayOpenGl::CDisplayOpenGl()
{

}
CDisplayOpenGl::~CDisplayOpenGl()
{

}

bool CDisplayOpenGl::InitialDisplayWindow()
{
	return true;
}
static int day = 0;
void CDisplayOpenGl::myDisplay(void)
{
	
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75,1,1,40);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,-20,20,0,0,0,0,0,1);
	glColor3f(1.0f,0.0f,0.0f);
	glutSolidSphere(6.96,20,20);
	
	glColor3f(0.0f,0.0f,1.0f);
	glRotatef(day/360.0*360.0,0.0f,0.0f,-1.0f);
	glTranslatef(15,0.0f,0.0f);
	glutSolidSphere(1.5945,20,20);
	
	glColor3f(1.0f,1.0f,0.0f);
	glRotatef(day/30.0*360.0-day/360.0*360.0,0.0f,0.0f,-1.0f);
	glTranslatef(3.8,0.0f,0.0f);
	glutSolidSphere(0.4345,20,20);
	glFlush();

	 glutSwapBuffers();//双缓冲区
}
void CDisplayOpenGl::myIdle(void)
{
	day+=10;
	if(day >=360)
		day = 0;
	myDisplay();
}

int CDisplayOpenGl::CreateGLWindow(CDisplayOpenGl *pDisplayObject)
{

	//双缓冲技术
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(700,500);

	
	//glutInit(&argc, argv);
 
 	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);   //使用双缓存及RGB模型
 
	// glutInitWindowSize(400,300);
 
 	//glutInitWindowPosition(100,100);
 
 	glutCreateWindow("Display window");
	//glutDisplayFunc(&(this->myDisplay));
	
	//glutDisplayFunc(&myIdle);
//	AnalyseBmpFile("FireBoy.bmp");


    
	 
	CDisplayOpenGl::m_pDisplayGlObject = pDisplayObject;

//	GetBmpData("FireBoy.bmp");

	//dispalyBmpData();
	glutDisplayFunc(&dispalyBmpData);
	glutMainLoop();
	
	return 0;
}

bool CDisplayOpenGl::bDraw(unsigned char *pData)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glLoadIdentity();
    

    //更新纹理
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 768, 576, GL_RGB, GL_UNSIGNED_BYTE,pData);
        
    glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,-1.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f,-1.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f,1.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,1.0f);
    glEnd();
    glFlush();
	return true;
}


void CDisplayOpenGl::Line(GLint x0,GLint y0,GLint x1,GLint y1)
{
	glBegin(GL_LINES);
	glColor3f(2.0f,1.0f,0.0f);
	glVertex2f(-1.0f,0.0f);
	glVertex2f(1.0f,0.0f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex2f(0.0f,1.0f);
	glVertex2f(0.0f,-1.0f);
	glEnd();
	
	return ;

	glBegin(GL_LINE);
	glColor3f(2.0f,1.0f,0.0f);
	//glVertex2i(x0,y0);
	//glVertex2i(x1,y1);
	glVertex2f(-1.0f,1.0f);
	glVertex2f(1.0f,-.01f);

	glEnd();
}

void CDisplayOpenGl::AnalyseBmpFile(const char *pBmpFileName)
{
	// 打开文件
     FILE* pFile = fopen(pBmpFileName, "rb");
    if( pFile == 0 )
         exit(0);

     // 读取图象的大小信息
     fseek(pFile, 0x0012, SEEK_SET);
     fread(&ImageWidth, sizeof(ImageWidth), 1, pFile);
     fread(&ImageHeight, sizeof(ImageHeight), 1, pFile);

     // 计算像素数据长度
     PixelLength = ImageWidth * 3;
     while( PixelLength % 4 != 0 )
         ++PixelLength;
     PixelLength *= ImageHeight;

     // 读取像素数据
     PixelData = (GLubyte*)malloc(PixelLength);
     if( PixelData == 0 )
         exit(0);

     fseek(pFile, 54, SEEK_SET);
     fread(PixelData, PixelLength, 1, pFile);

     // 关闭文件
     fclose(pFile);

}

void CDisplayOpenGl::GetBmpData(const char *pBmpFileName)
{
m_bmp=new AUX_RGBImageRec;
	m_bmp=auxDIBImageLoadA(pBmpFileName);
}

void CDisplayOpenGl::dispalyBmpData()
{
	// 清除屏幕并不必要
     // 每次绘制时，画面都覆盖整个屏幕
     // 因此无论是否清除屏幕，结果都一样
     // glClear(GL_COLOR_BUFFER_BIT);
//      glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
	glRasterPos2f(0,0);
	//glRotatef(180,0,0,0);
     // 绘制像素
     #if 0
     glDrawPixels(m_pDisplayGlObject->ImageWidth, m_pDisplayGlObject->ImageHeight,GL_BGR_EXT, GL_UNSIGNED_BYTE, m_pDisplayGlObject->PixelData);
	#else
	while(1)
		{
			
			if(m_bReDrawFlag == true)
				{
					m_bReDrawFlag = false;
					Line(0,0,0,0);
					glDrawPixels(ImageWidth,ImageHeight,GL_BGR_EXT,GL_UNSIGNED_BYTE,PixelData);//GL_RGB
					
					 // 完成绘制
     				glutSwapBuffers();
			//		glRotatef(180,1,0,0); 
				}
			else
				{
					Sleep(10);
				}
		}
	#endif
    
}

