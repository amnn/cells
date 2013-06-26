#ifndef GLFWSCR_H
#define GLFWSCR_H

#include "RenderEngine.h"

class GLFWScr {
    
public:

    GLFWScr( int , int ) throw( char const * );
    ~GLFWScr();

    GLFWScr            ( const GLFWScr  & ) = delete;
    GLFWScr &operator= ( const GLFWScr  & ) = delete;

    GLFWScr            ( GLFWScr       && )                {}
    GLFWScr &operator= ( GLFWScr       && ) { return *this; }

    void display_link( RenderEngine<GLFWScr> * ) const;

    void swap() const;

};

#endif