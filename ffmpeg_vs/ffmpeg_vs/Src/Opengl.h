#ifndef _OPENGL_H
#define _OPENGL_H

#include <GL/glut.h>
#include <GL/glaux.h>



class CDisplayOpenGl{
public:
	CDisplayOpenGl();
	~CDisplayOpenGl();

	public:
		bool InitialDisplayWindow();
		int CreateGLWindow(CDisplayOpenGl *pDisplayObject);
		bool bDraw(unsigned char *pData);
	public:
		static void myDisplay();

		static void myIdle(void);
//2014-07-31
	public:
		static int m_day;
		static void Line(GLint x0,GLint y0,GLint x1,GLint y1);
		void AnalyseBmpFile(const char *pBmpFileName);

		 static void dispalyBmpData();

		 static void GetBmpData(const char *pBmpFileName);
	public:
		static GLint     ImageWidth;
		static GLint     ImageHeight;
		static GLint     PixelLength;
		static GLubyte* PixelData;
		static AUX_RGBImageRec  *m_bmp;
		static CDisplayOpenGl *m_pDisplayGlObject;

		static bool m_bReDrawFlag;
		
};
#endif
