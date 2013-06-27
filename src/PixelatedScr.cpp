#include "GL_includes.h"

#include "PixelatedScr.h"
	
PixelatedScr::PixelatedScr( int width, int height ) 
throw (     		   const char * ) 
: GLFWScr 	( 2 * width, 2 * height )
{

	glGenFrameBuffers( 1, &_loResFBO );
	// Create Render Buffer of appropriate size.
}

PixelatedScr::~PixelatedScr() { glDeleteFrameBuffers( 1, &_loResFBO ); }
	
void display_link( RenderEngine<PixelatedScr> * ) const
{
	// Rendering pass should swap and blit framebuffers.
}