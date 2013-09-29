#include <iostream>
#include <vector>

#include "GL_includes.h"

#include "simulation/SimState.h"
#include "texture/Texture.h"
#include "noise/Noise.h"

namespace {

    template <class T>
    std::vector<T>
    interleave(
        std::vector<T> & xs,
        std::vector<T> & ys,
        size_t           stride = 1
    )
    {
        std::vector<T> zs( xs.size() + ys.size() );

        auto itx = xs.begin(),
             ity = ys.begin();

        for
        (
            int k =  0;

            k     <  zs.capacity() &&
            itx   != xs.end()      &&
            ity   != ys.end();

            k     += stride+1
        )
            for(int l = 0; l <= stride; ++l)
                zs[k+l] = *(l == stride ? ity : itx)++;

        for(; itx != xs.end(); ++itx) zs.push_back(*itx);
        for(; ity != ys.end(); ++ity) zs.push_back(*ity);

        return zs;
    }

};

namespace Simulation {

SimState::SimState(
    GLsizei w,
    GLsizei h,
    int     popCount
)
    : _state { create_state(w, h, popCount) }
{}

std::shared_ptr<Engine::Texture> &
SimState::state()
{
    return _state;
}

void
SimState::swap() const
{
    return;
}

std::shared_ptr<Engine::Texture>
SimState::create_state(
    GLsizei w,
    GLsizei h,
    int     popCount
)
{
    int dim = w * h;

    std::vector<GLuint> terrain (dim),
                        energy  (dim);

    unsigned int seed = Engine::Noise::perlin<1,6>( w, h,    terrain        );
                        Engine::Noise::diffuse<3,0>(w, h, 5, energy, 7, seed);

    auto texture_data = interleave(terrain, energy);

    std::cout << "Using seed: " << seed << std::endl;

    auto pSimState = new Engine::Texture3D(GL_TEXTURE_2D_ARRAY, 1, GL_RG32I,
                                                         w, h, popCount + 1);

    pSimState->param(GL_TEXTURE_MIN_FILTER,         GL_NEAREST);
    pSimState->param(GL_TEXTURE_MAG_FILTER,         GL_NEAREST);
    pSimState->param(GL_TEXTURE_WRAP_S,       GL_CLAMP_TO_EDGE);
    pSimState->param(GL_TEXTURE_WRAP_T,       GL_CLAMP_TO_EDGE);

    pSimState->sub_image( 0, 0, 0, 0, w, h, 1, GL_RG_INTEGER,
                       GL_UNSIGNED_INT, texture_data.data() );

    for(int i = 1; i <= popCount; ++i)
    {
        std::vector<GLuint> population(dim);

        Engine::Noise::diffuse<3,0>(      w, h, 6, population, 4);
        pSimState->sub_image(0, 0, 0, i, w, h, 1, GL_RED_INTEGER,
                             GL_UNSIGNED_INT, population.data() );
    }

    return std::shared_ptr<Engine::Texture>(pSimState);
}

bool
SimState::should_close_window()
{
    return false;
}

}; // namespace Simulation
