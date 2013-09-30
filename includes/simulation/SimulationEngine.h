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
        GLsizei,
        GLsizei,
        std::initializer_list<const char *>
    );
};

}; // namespace Simulation

#endif
