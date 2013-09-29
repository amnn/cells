#ifndef SIM_STATE_H
#define SIM_STATE_H

#include <memory>

#include "GL_includes.h"

#include "texture/Texture.h"

namespace Simulation {

class SimState
{
    std::shared_ptr<Engine::Texture> _state;

public:
    SimState(GLsizei, GLsizei, int);

    std::shared_ptr<Engine::Texture> &
    state();

    void swap() const;

    bool
    should_close_window();

private:
    std::shared_ptr<Engine::Texture>
    create_state(GLsizei, GLsizei, int);
};

}; // namespace Simulation

#endif
