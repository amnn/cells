#include <initializer_list>
#include <memory>

#include "GL_includes.h"

#include "simulation/Simulation.h"
#include "display_link/LockStepLink.h"
#include "texture/Texture.h"

namespace Simulation {
Simulation::Simulation(
    GLsizei w,
    GLsizei h,
    std::initializer_list<const char *> l
)
    : LockStepLink (_engine, _state)
    , _simEngine   ( w, h, l )
    , _state       ( w, h, l.size() )
{};

SimulationEngine &
Simulation::engine()
{
    return _simEngine;
}

SimState &
Simulation::state()
{
    return _state;
}

std::shared_ptr<Engine::Texture> &
Simulation::public_state()
{
    return _state.state();
}

int
Simulation::world_width() const
{
    return static_cast<int>( _simEngine.width() );
}

int
Simulation::world_height() const
{
    return static_cast<int>( _simEngine.height() );
}

}; // namespace Simulation
