#ifndef VISUALISTION_ENGINE_H
#define VISUALISTION_ENGINE_H

#include <memory>

#include "GL_includes.h"

#include "screen/PixelatedScr.h"
#include "renderable/Renderable.h"
#include "renderable/RenderEngine.h"
#include "renderable/BufferPoly.h"
#include "shader/ShaderProgram.h"

namespace Simulation {

class VisualisationEngine : public Engine::RenderEngine
{
public:

    VisualisationEngine(float, float);

private:

    static void
    tick_callback(Engine::Renderable &, const double &);

    std::shared_ptr<Engine::ShaderProgram>
    create_program();

    std::shared_ptr<Engine::Renderable>
    create_canvas(float, float);

};

}; // namespace Simulation

#endif
