#include <iostream>

#include "GL_includes.h"

#include "GLFWScr.h"

#include "PixelatedScr.h"

#define SF 4

// FIXME: Moving screen causes pixelated rendering to fail.
// FIXME: Minimising screen or being covered messes up swap interval.
 
PixelatedScr::PixelatedScr( GLsizei width, GLsizei height ) 
throw (                const char * ) 
: GLFWScr ( SF * width, SF * height ),
          _w { width }, _h { height }
{
   
    glViewport(           0, 0, _w, _h );

    glGenFramebuffers(  1, & _loResFBO ); 
    glGenRenderbuffers( 2, & _buffs[0] );
    
    glBindFramebuffer( GL_FRAMEBUFFER, _loResFBO );
    
    // Colour Buffer
    glBindRenderbuffer(    GL_RENDERBUFFER,        _buffs[0] ); 
    glRenderbufferStorage( GL_RENDERBUFFER, GL_RGBA8, _w, _h );

    glFramebufferRenderbuffer( GL_FRAMEBUFFER ,
                         GL_COLOR_ATTACHMENT0 ,
                              GL_RENDERBUFFER ,
                                    _buffs[0] );
    
    // Depth Buffer
    glBindRenderbuffer(    GL_RENDERBUFFER,                  _buffs[1] );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _w, _h );

    glFramebufferRenderbuffer( GL_FRAMEBUFFER ,
                          GL_DEPTH_ATTACHMENT ,
                              GL_RENDERBUFFER ,
                                    _buffs[1] );
   
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

PixelatedScr::~PixelatedScr() { 
    glDeleteFramebuffers(  1, & _loResFBO ); 
    glDeleteRenderbuffers( 2, & _buffs[0] );
}
    
void PixelatedScr::display_link( RenderEngine<PixelatedScr> * engine ) const
{

    double last  = glfwGetTime(),
           now,
           delta;

    do { 

        engine->thrd_req();
        glBindFramebuffer( GL_FRAMEBUFFER, _loResFBO );

        engine->render();

        glBindFramebuffer( GL_DRAW_FRAMEBUFFER,    0 );
        glDrawBuffer(                        GL_BACK );

        glBlitFramebuffer(                        0, 0,    _w,    _h,
                                                  0, 0, SF*_w, SF*_h,
                           GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 
                                                          GL_NEAREST );
        
        swap(); 
        
        now   = glfwGetTime();
        delta =    now - last;
        last  =           now;

        engine->tick( delta );


        engine->thrd_rel();
        
    } 
    while( glfwGetKey( _win, GLFW_KEY_ESCAPE ) != GLFW_PRESS && 
           !glfwWindowShouldClose( _win )
         );

}