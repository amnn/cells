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

    Simulation(GLsizei, GLsizei, std::initializer_list<const char *>);

    SimulationEngine &
    engine();

    SimState &
    state();

    std::shared_ptr<Engine::Texture> &
    public_state();

    int
    world_width() const;

    int
    world_height() const;
};

}; // namespace Simulation

#endif
