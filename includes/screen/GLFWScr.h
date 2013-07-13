#ifndef GLFWSCR_H
#define GLFWSCR_H

#include "GL_includes.h"

#include "renderable/RenderEngine.h"

class GLFWScr {


public:

    GLFWScr( int , int ) throw( char const * );
    ~GLFWScr();

    GLFWScr            ( const GLFWScr  & ) = delete;
    GLFWScr &operator= ( const GLFWScr  & ) = delete;

    GLFWScr            ( GLFWScr       && )                {}
    GLFWScr &operator= ( GLFWScr       && ) { return *this; }

    void set_title(    const char  	         * ) const;
    void display_link( RenderEngine<GLFWScr> * ) const;

    void swap() const;

protected:

	GLFWwindow *_win;
	
};

#endif