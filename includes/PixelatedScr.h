#ifndef PIXELATED_SCR_H
#define PIXELATED_SCR_H

#include "GL_includes.h"

#include "GLFWScr.h"

class PixelatedScr : public GLFWScr {

	GLuint _loResFBO;

public:
	
	PixelatedScr( int, int ) throw ( const char * );
	
	void display_link( RenderEngine<PixelatedScr> * ) const;		

};

#endif