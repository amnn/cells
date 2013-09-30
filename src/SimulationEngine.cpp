#include <initializer_list>

#include "GL_includes.h"

#include "simulation/SimulationEngine.h"
#include "renderable/RenderEngine.h"

namespace Simulation {

SimulationEngine::SimulationEngine(
    GLsizei w,
    GLsizei h,
    std::initializer_list<const char *> l
)
    : Engine::RenderEngine(w, h, 0.1f, 100.f)
{};

}; // namespace Simulation
