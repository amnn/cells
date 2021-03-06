#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <cstdlib>
#include <utility>
#include <mutex>
#include <thread>
#include <vector>

#include "GL_includes.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "renderable/RenderGroup.h"
#include "shader/ShaderProgram.h"

namespace engine {

template< class Scr >
class RenderEngine : public RenderGroup
{

    bool                             _term;
    std::mutex                     _access;
    std::vector<std::thread>          _aux;
    Scr                            _screen;
    std::shared_ptr<ShaderProgram>   _prog;
    glm::mat4                        _proj;

    RenderEngine(float _w, float _h)
    throw(const char *)
    : RenderGroup               (),
      _term              { false },
      _screen { (int)_w, (int)_h },
      _proj                { 1.f }
    {}

public:

    RenderEngine
    (
      float _w,
      float _h,
      float fov,
      float ncp,
      float fcp
    ) : RenderEngine(_w, _h)
    {
        _proj  = glm::perspective(fov, _w/_h, ncp, fcp);
    }

    RenderEngine
    (
      float  _w,
      float  _h,
      float ncp,
      float fcp
    ) : RenderEngine(_w, _h)
    {
        _proj  = glm::ortho(0.f, _w, 0.f, _h, ncp, fcp);
    }

    ~RenderEngine()
    {
        _term = true;

        for(std::thread & th : _aux) th.join();
    }

    std::vector< std::thread > & aux()        { return    _aux; }
    Scr                        & scr()        { return _screen; }
    bool                       & term()       { return   _term; }
    const ShaderProgram        & prog() const { return  *_prog; }

    void thrd_req()  {           _access.lock(  ); }
    void thrd_rel()  {           _access.unlock(); }

    void draw_loop() { _screen.display_link(this); }

    void
    use_program(std::shared_ptr<ShaderProgram> & p)
    {
        _prog = p;
        _prog->use();
    }

    void
    look_at
    (
      float cX, float cY, float cZ,
      float pX, float pY, float pZ,
      float                    dir
    )
    {
        _local = glm::lookAt( glm::vec3(cX,   cY,  cZ),
                              glm::vec3(pX,   pY,  pZ),
                              glm::vec3(0.f, dir, 0.f) );
    }

    using RenderGroup::render;

    void
    render() const
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render(*_prog, _proj);
    }
};

}; // namespace engine

#endif
