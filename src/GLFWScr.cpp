#include <iostream>
#include <thread>
#include <cstdlib>

#include "GL_includes.h"

#include "screen/GLFWScr.h"

namespace Engine {

GLFWScr::GLFWScr(int width, int height) throw(const char *)
{
    glewExperimental = true;

    if(!glfwInit()) { throw("Failed to Initialize GLFW!"); }

    glfwWindowHint(GLFW_RESIZABLE,                             GL_FALSE);
    glfwWindowHint(GLFW_DEPTH_BITS,                                  32);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,                        3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,                        2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,                  GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);

    _win = glfwCreateWindow(width, height, "", NULL, NULL);

    if(!_win) { glfwTerminate(); throw("Failed to Open GLFW Window!"); }

    glfwMakeContextCurrent(        _win);
    glfwSetWindowUserPointer(_win, this);

    if(glewInit() != GLEW_OK) {  throw( "Failed to Initialize GLEW!"); }

    // To clear error produced by glewExperimental bug.
    while(GL_NO_ERROR != glGetError());

    glfwSetInputMode(_win, GLFW_STICKY_KEYS, GL_TRUE);
    glEnable(                          GL_DEPTH_TEST);
    glDepthMask(                             GL_TRUE);
    glDepthFunc(                           GL_LEQUAL);
}

GLFWScr::~GLFWScr()
{
    glfwTerminate();
}

void
GLFWScr::set_title(const char *title) const
{
    glfwSetWindowTitle(_win, title);
}

void
GLFWScr::display_link(RenderEngine<GLFWScr> *engine) const
{
    double last  = glfwGetTime();

    static const double TICK_STEP = 1.0/60;

    do {
        engine->thrd_req();
        engine->render(  );

        swap();

        double now   = glfwGetTime(),
               delta =    now - last;

        while(delta >= 0) {
            engine->tick( std::min(delta, TICK_STEP) );
            delta -= TICK_STEP;
        }

        last = glfwGetTime();
        engine->thrd_rel();
    }
    while( glfwGetKey(_win, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           !glfwWindowShouldClose(_win)
         );
}

void
GLFWScr::swap() const
{
    glfwSwapBuffers(_win);
    glfwPollEvents();
}

}; // namespace Engine
