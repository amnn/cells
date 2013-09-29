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
#include "simulation/Simulation.h"

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
   Simulation & sim
)
    : Engine::RenderEngine(
        static_cast<float>( sim.world_width( ) ),
        static_cast<float>( sim.world_height() ),
        0.1f,
        100.f
    )
{
    use_program( create_program() );
    add_child( create_canvas(sim) );

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
VisualisationEngine::create_canvas(Simulation & sim)
{
    int iW  = sim.world_width( ),
        iH  = sim.world_height(),
        dim =           iW * iH;

    float w = float(iW),
          h = float(iH);

    xy_t verts[4]{
        {-w/2, -h/2},
        { w/2, -h/2},
        {-w/2,  h/2},
        { w/2,  h/2}
    };

    auto vBuff     = std::make_shared<Engine::Buffer>(GL_ARRAY_BUFFER, 4, verts, GL_STATIC_DRAW);
    auto quadPoly  = std::make_shared<Engine::BufferPoly>(        *this, vBuff, xy_t::layout, 4);

    return std::shared_ptr<Engine::Renderable>( new Engine::BufferPoly::TextureInstance(quadPoly, sim.public_state()) );
}

}; // namespace Simulation
