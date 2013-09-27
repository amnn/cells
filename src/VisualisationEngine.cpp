#include <iostream>
#include <memory>
#include <vector>

#include "GL_includes.h"

#include "screen/PixelatedScr.h"
#include "renderable/Renderable.h"
#include "renderable/RenderEngine.h"
#include "renderable/BufferPoly.h"
#include "texture/Texture.h"
#include "shader/ShaderProgram.h"
#include "buffer/Buffer.h"
#include "noise/Noise.h"

#include "simulation/VisualisationEngine.h"

namespace {
    struct xy_t {
        float vert[2];

        static void
        layout(
            const Engine::ShaderProgram & p,
            Engine::Buffer & b
        )
        {
            b.register_attrib(p["vPos"], 2, GL_FLOAT, GL_FALSE, 0, 0);
        }
    };

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
}

namespace Simulation {

void
VisualisationEngine::tick_callback(
    Engine::Renderable & e,
    const double       & d
)
{
    VisualisationEngine & engine = (VisualisationEngine &)e;
    glUniform1f( engine.prog()["time"], glfwGetTime() );
}

VisualisationEngine::VisualisationEngine(
    float w,
    float h
)
: Engine::RenderEngine(w, h, 0.1f, 100.f)
{
    use_program(  create_program() );
    add_child( create_canvas(w, h) );

    callback() = VisualisationEngine::tick_callback;
}

std::shared_ptr<Engine::ShaderProgram>
VisualisationEngine::create_program()
{
    auto prog = std::make_shared<Engine::ShaderProgram>();
    prog->attach_shader({ GL_VERTEX_SHADER,   "assets/identity_vert.glsl" });
    prog->attach_shader({ GL_FRAGMENT_SHADER, "assets/draw_sim_frag.glsl" });

    prog->link();
    prog->uniform("time");
    prog->attrib( "vPos");

    return prog;
}

std::shared_ptr<Engine::Renderable>
VisualisationEngine::create_canvas(float w, float h)
{
    xy_t verts[4]{
        {-w/2, -h/2},
        { w/2, -h/2},
        {-w/2,  h/2},
        { w/2,  h/2}
    };

    int iW  = static_cast<int>(w),
        iH  = static_cast<int>(h),
        dim =             iW * iH;

    auto vBuff     = std::make_shared<Engine::Buffer>(GL_ARRAY_BUFFER, 4, verts, GL_STATIC_DRAW);
    auto quadPoly  = std::make_shared<Engine::BufferPoly>(        *this, vBuff, xy_t::layout, 4);

    // DEMO DATA ONLY, will feed from SimulationEngine::_publicState in future.
    std::vector<GLuint> terrain    (dim),
                        energy     (dim),
                        population (dim);

    unsigned int seed = Engine::Noise::perlin<1,6>(iW, iH, terrain);
    Engine::Noise::diffuse<3,0>(        iW, iH, 5, energy, 7, seed);
    auto texture_data = interleave(                terrain, energy);

    std::cout << "Using seed: " << seed << std::endl;

    auto pSimState = new Engine::Texture3D(GL_TEXTURE_2D_ARRAY, 1, GL_RG32I, iW, iH, 9);
    std::shared_ptr<Engine::Texture> simState(pSimState);

    pSimState->param(GL_TEXTURE_MIN_FILTER,          GL_NEAREST);
    pSimState->param(GL_TEXTURE_MAG_FILTER,          GL_NEAREST);
    pSimState->param(GL_TEXTURE_WRAP_S,        GL_CLAMP_TO_EDGE);
    pSimState->param(GL_TEXTURE_WRAP_T,        GL_CLAMP_TO_EDGE);

    pSimState->sub_image( 0, 0, 0, 0, iW, iH, 1, GL_RG_INTEGER,
                         GL_UNSIGNED_INT, texture_data.data() );

    for(int i = 1; i <= 8; ++i)
    {
        Engine::Noise::diffuse<3,0>(iW, iH, 6, population, 4);
        pSimState->sub_image(0, 0, 0, i, iW, iH, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, population.data() );
    }

    return std::shared_ptr<Engine::Renderable>( new Engine::BufferPoly::TextureInstance(quadPoly, simState) );
}

}; // namespace Simulation
