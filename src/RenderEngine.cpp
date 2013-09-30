#include <memory>
#include <vector>
#include <thread>

#include "GL_includes.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "renderable/RenderEngine.h"

#include "renderable/RenderGroup.h"

namespace Engine {

RenderEngine::RenderEngine(float w, float h)
    : RenderGroup ()
    , _term       { false }
    , _proj       { 1.f }
    , _width      { w }
    , _height     { h }
{}

RenderEngine::RenderEngine(
    float w,
    float h,
    float fov,
    float ncp,
    float fcp
)
    : RenderEngine(w, h)
{
    _proj = glm::perspective(fov, w/h, ncp, fcp);
}

RenderEngine::RenderEngine(
    float w,
    float h,
    float ncp,
    float fcp
)
    : RenderEngine(w, h)
{
    _proj = glm::ortho(0.f, w, 0.f, h, ncp, fcp);
}

RenderEngine::~RenderEngine()
{
    _term = true;

    for(std::thread & th : _aux) th.join();
}

std::vector<std::thread> &
RenderEngine::aux()
{
    return _aux;
}

bool
RenderEngine::term() const
{
    return _term;
}

const ShaderProgram &
RenderEngine::prog() const
{
    return *_prog;
}

float
RenderEngine::width() const
{
    return _width;
}

float
RenderEngine::height() const
{
    return _height;
}

void
RenderEngine::thrd_req()
{
    _access.lock();
}

void
RenderEngine::thrd_rel()
{
    _access.unlock();
}

void
RenderEngine::use_program(std::shared_ptr<ShaderProgram> & p)
{
    _prog = p;
    _prog->use();
}

void
RenderEngine::use_program(std::shared_ptr<ShaderProgram> && p)
{
    _prog = p;
    _prog->use();
}

void
RenderEngine::look_at(
    float cX, float cY, float cZ,
    float pX, float pY, float pZ,
    float                    dir
)
{
    _local = glm::lookAt( glm::vec3(cX,   cY,  cZ),
                          glm::vec3(pX,   pY,  pZ),
                          glm::vec3(0.f, dir, 0.f) );
}

void
RenderEngine::render() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _prog->use(); render(*_prog, _proj);
}

}; //namespace Engine
