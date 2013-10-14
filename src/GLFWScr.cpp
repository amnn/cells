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
GLFWScr::setup()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void
GLFWScr::swap() const
{
    glfwSwapBuffers(_win);
    glfwPollEvents();
}

bool
GLFWScr::should_close_window()
{
    return
        glfwGetKey(_win, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
        glfwWindowShouldClose(_win);
}

}; // namespace Engine
