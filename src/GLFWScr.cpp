#include <iostream>
#include <thread>
#include <cstdlib>

#include "GL_includes.h"

#include "GLFWScr.h"


GLFWScr::GLFWScr( int width, int height ) throw( char const * )
{
    glewExperimental = true;

    if( !glfwInit() ) { throw( "Failed to Initialize GLFW!" ); }
    glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE,                      GL_TRUE );
    glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR,                        3 );
    glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR,                        2 );
    glfwOpenWindowHint( GLFW_OPENGL_PROFILE,       GLFW_OPENGL_CORE_PROFILE );

    if( !glfwOpenWindow( width, height, 0,0,0,0, 32,0, GLFW_WINDOW ) )
    {
        glfwTerminate(); throw( "Failed to Open GLFW Window!" );
    }

    if( glewInit() != GLEW_OK ) { throw( "Failed to Initialize GLEW!" ); }

    // To clear error produced by glewExperimental bug. 
    while( GL_NO_ERROR != glGetError() ); 

    glfwEnable(   GLFW_STICKY_KEYS );
    glEnable(        GL_DEPTH_TEST );
    glDepthMask(           GL_TRUE );
    glDepthFunc(         GL_LEQUAL );

}

GLFWScr::~GLFWScr()
{
    glfwTerminate();
}

void GLFWScr::set_title( const char *title ) const
{    
    glfwSetWindowTitle( title );
}

void GLFWScr::display_link( RenderEngine<GLFWScr> *engine ) const
{

    double last  = glfwGetTime(),
           now,
           delta;
    
    do { 

        engine->thrd_req(); 
        engine->render(  );

        swap(); 

        now   = glfwGetTime();
        delta =    now - last;
        last  =           now;

        engine->tick( delta );


        engine->thrd_rel();

    } 
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ) );

}

void GLFWScr::swap() const { glfwSwapBuffers(); }
