#ifndef GLFWSCR_H
#define GLFWSCR_H

#include "GL_includes.h"

#include "renderable/RenderEngine.h"

namespace Engine {

class GLFWScr {

public:

    GLFWScr(int , int) throw(const char *);
    ~GLFWScr();

    GLFWScr            (const GLFWScr  &) = delete;
    GLFWScr &operator= (const GLFWScr  &) = delete;

    GLFWScr            (GLFWScr       &&)                {}
    GLFWScr &operator= (GLFWScr       &&) { return *this; }

    void set_title(              const char *) const;

    void swap() const;

    bool should_close_window();

protected:

	GLFWwindow *_win;

};

}; // namespace Engine

#endif
