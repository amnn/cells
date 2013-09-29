#ifndef SIMULATION_ENGINE_H
#define SIMULATION_ENGINE_H

#include <initializer_list>

#include "GL_includes.h"

#include "renderable/RenderEngine.h"

namespace Simulation {

class SimulationEngine : public Engine::RenderEngine
{

public:
    SimulationEngine(
        GLsizei                             w,
        GLsizei                             h,
        std::initializer_list<const char *> l
    )
        : Engine::RenderEngine(w, h, 0.1f, 100.f)
    {}

};

}; // namespace Simulation

#endif
