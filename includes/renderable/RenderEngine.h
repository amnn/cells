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

namespace Engine {

class RenderEngine : public RenderGroup
{

    bool                            _term;
    std::mutex                      _access;
    std::vector<std::thread>        _aux;
    std::shared_ptr<ShaderProgram>  _prog;
    glm::mat4                       _proj;
    float                           _width,
                                    _height;

    RenderEngine(float w, float h);

public:

    RenderEngine
    (
      float w,
      float h,
      float fov,
      float ncp,
      float fcp
    );

    RenderEngine
    (
      float  w,
      float  h,
      float ncp,
      float fcp
    );

    ~RenderEngine();

    std::vector< std::thread > & aux();
    bool                         term()   const;
    const ShaderProgram        & prog()   const;
    float                        width()  const;
    float                        height() const;

    void thrd_req();
    void thrd_rel();

    void
    use_program(std::shared_ptr<ShaderProgram> &);

    void
    use_program(std::shared_ptr<ShaderProgram> &&);

    void
    look_at
    (
      float cX, float cY, float cZ,
      float pX, float pY, float pZ,
      float                    dir
    );

    using RenderGroup::render;

    void
    render() const;

};

}; // namespace Engine

#endif
