#include <iostream>
#include <memory>

#include "GL_includes.h"

#include "renderable/Renderable.h"
#include "renderable/RenderEngine.h"
#include "renderable/BufferPoly.h"
#include "texture/Texture.h"
#include "screen/PixelatedScr.h"
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
    std::unique_ptr<T[]>
    interleave(
        std::unique_ptr<T[]> &  xs,
        std::unique_ptr<T[]> &  ys,
        size_t                 count,
        size_t                 stride = 1
    )
    {
        size_t               len { count * (stride + 1) };
        std::unique_ptr<T[]> zs            { new T[len] };

        int i = 0, j = 0;
        for(int k = 0; k < len; k += stride+1)
        {
            int l;
            for(l = 0; l < stride; ++l) zs[k+l] = xs[i+l];

            zs[k+l] = ys[j];
            i += stride;
            ++j;
        }

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
: Engine::RenderEngine<Engine::PixelatedScr>(w, h, 0.1f, 100.f)
{
    use_program(  create_program() );
    add_child( create_canvas(w, h) );

    scr().set_title(        "Cells");

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

    int iW = static_cast<int>(w),
        iH = static_cast<int>(h);

    auto vBuff     = std::make_shared<Engine::Buffer>(GL_ARRAY_BUFFER, 4, verts, GL_STATIC_DRAW);
    auto quadPoly  = std::make_shared<Engine::BufferPoly>(        *this, vBuff, xy_t::layout, 4);

    // DEMO DATA ONLY, will feed from SimulationEngine::_publicState in future.
    std::unique_ptr<GLuint[]> terrain    { new GLuint[iW * iH] },
                              energy     { new GLuint[iW * iH] },
                              population { new GLuint[iW * iH] };

    unsigned int seed = Engine::Noise::perlin<1,6>(iW, iH, terrain);
    Engine::Noise::diffuse<3,0>(        iW, iH, 5, energy, 7, seed);
    auto texture_data = interleave(       terrain, energy, iW * iH);

    std::cout << "Using seed: " << seed << std::endl;

    auto pSimState = new Engine::Texture3D(GL_TEXTURE_2D_ARRAY, 1, GL_RG32I, iW, iH, 9);
    std::shared_ptr<Engine::Texture> simState(pSimState);

    pSimState->param(GL_TEXTURE_MIN_FILTER,          GL_NEAREST);
    pSimState->param(GL_TEXTURE_MAG_FILTER,          GL_NEAREST);
    pSimState->param(GL_TEXTURE_WRAP_S,        GL_CLAMP_TO_EDGE);
    pSimState->param(GL_TEXTURE_WRAP_T,        GL_CLAMP_TO_EDGE);

    pSimState->sub_image( 0, 0, 0, 0, iW, iH, 1, GL_RG_INTEGER,
                           GL_UNSIGNED_INT, texture_data.get() );

    for(int i = 1; i <= 8; ++i)
    {
        Engine::Noise::diffuse<3,0>(iW, iH, 6, population, 4);
        pSimState->sub_image(0, 0, 0, i, iW, iH, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, population.get() );
    }

    return std::shared_ptr<Engine::Renderable>( new Engine::BufferPoly::TextureInstance(quadPoly, simState) );
}

}; // namespace Simulation
