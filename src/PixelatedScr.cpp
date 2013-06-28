#include <iostream>

#include "GL_includes.h"

#include "GLFWScr.h"

#include "PixelatedScr.h"

#define SF 2
 
PixelatedScr::PixelatedScr( GLsizei width, GLsizei height ) 
throw (                const char * ) 
: GLFWScr   ( SF * width, SF * height ),
          _w { width }, _h { height }
{
    
    GLint firstFBO; glGetIntegerv( GL_FRAMEBUFFER_BINDING, &firstFBO );
    std::cout << "First FBO " << firstFBO << std::endl;

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
   
    GLenum res = glCheckFramebufferStatus( GL_FRAMEBUFFER );

    std::cout << res << " " << GL_FRAMEBUFFER_COMPLETE << std::endl;

    GLenum err = glGetError(); std::cout << err << " " << GL_NO_ERROR << std::endl;

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

        glBindFramebuffer( GL_DRAW_FRAMEBUFFER,         0 );

        glBlitFramebuffer(                          0, 0, _w  ,   _h,
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
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ) );

}