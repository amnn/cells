#ifndef PIXELATED_SCR_H
#define PIXELATED_SCR_H

#include "GL_includes.h"

#include "GLFWScr.h"

class PixelatedScr : public GLFWScr {

    GLuint  _loResFBO, _buffs[2];
    GLsizei               _w, _h;

public:
    
    PixelatedScr( int, int ) throw ( const char * );
    ~PixelatedScr();
    
    void display_link( RenderEngine<PixelatedScr> * ) const;        

};

#endif