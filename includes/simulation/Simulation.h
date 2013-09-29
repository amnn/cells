#ifndef SIMULATION_H
#define SIMULATION_H

#include <memory>
#include <initializer_list>

#include "GL_includes.h"

#include "display_link/LockStepLink.h"
#include "simulation/SimulationEngine.h"
#include "simulation/SimState.h"

namespace Simulation {

class Simulation : public Engine::LockStepLink<SimState>
{
    SimulationEngine _simEngine;
    SimState             _state;

public:

    Simulation(
        GLsizei w,
        GLsizei h,
        std::initializer_list<const char *> l
    )
        : LockStepLink (_engine, _state)
        , _simEngine   ( w, h, l )
        , _state       ( w, h, l.size() )
    {};

    const SimulationEngine &
    engine() const
    {
        return _simEngine;
    }

    std::shared_ptr<Engine::Texture> &
    public_state()
    {
        return _state.state();
    }

    int
    world_width() const
    {
        return static_cast<int>( _simEngine.width() );
    }

    int
    world_height() const
    {
        return static_cast<int>( _simEngine.height() );
    }
};

}; // namespace Simulation

#endif
