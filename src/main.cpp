#include <iostream>
#include <memory>
#include <cstdlib>

#define GLM_SWIZZLE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GL_includes.h"

#include "buffer/Buffer.h"
#include "noise/Noise.h"
#include "renderable/BufferPoly.h"
#include "renderable/RenderEngine.h"
#include "screen/PixelatedScr.h"
#include "shader/ShaderProgram.h"
#include "texture/Texture.h"

using namespace    std;
using namespace engine;

namespace {
    struct xy {
        float vert[2];

        static void
        layout(const ShaderProgram & p, Buffer & b)
        {
            b.register_attrib(p["vPos"], 2, GL_FLOAT, GL_FALSE, 0, 0);
        }
    };

    template <class T>
    unique_ptr<T[]>
    interleave(
        unique_ptr<T[]> &  xs,
        unique_ptr<T[]> &  ys,
        size_t             count,
        size_t             stride = 1
    )
    {
        size_t          len { count * (stride + 1) };
        unique_ptr<T[]> zs            { new T[len] };

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
};

int
main(int argc, char ** argv)
{
    if(argc < 2) return 0;

    float width  = strtof(argv[1], nullptr),
          height = strtof(argv[2], nullptr);

    int   iW     = int( width),
          iH     = int(height);

    try {
        RenderEngine<PixelatedScr> engine (width, height, 0.1f, 100.f);
        shared_ptr<ShaderProgram>  prog (           new ShaderProgram);

        prog->attach_shader({ GL_VERTEX_SHADER,            "assets/vert.glsl" });
        prog->attach_shader({ GL_FRAGMENT_SHADER, "assets/draw_sim_frag.glsl" });

        prog->link();

        prog->uniform("MVP");
        prog->attrib("vPos");

        engine.scr().set_title("Cells");
        engine.use_program(       prog);

        xy verts[4] = {
            {0.f,      0.f},
            {width,    0.f},
            {0.f,   height},
            {width, height}
        };

        unique_ptr<GLuint[]> terrain { new GLuint[iW * iH] },
                             energy  { new GLuint[iW * iH] };

        unsigned int seed = Noise::perlin<1,6>( iW, iH, terrain);
        Noise::diffuse<3,0>(               iW, iH, 5, energy, 7);
        auto texture_data = interleave(terrain, energy, iW * iH);

        cout << "Using seed: " << seed << endl;

        Texture3D *pSimState = new Texture3D(GL_TEXTURE_2D_ARRAY, 1,
                                               GL_RG32UI, iW, iH, 2);

        shared_ptr<Texture> simState(pSimState);

        pSimState->param(GL_TEXTURE_MIN_FILTER,          GL_NEAREST);
        pSimState->param(GL_TEXTURE_MAG_FILTER,          GL_NEAREST);
        pSimState->param(GL_TEXTURE_WRAP_S,        GL_CLAMP_TO_EDGE);
        pSimState->param(GL_TEXTURE_WRAP_T,        GL_CLAMP_TO_EDGE);

        pSimState->sub_image( 0, 0, 0, 0, iW, iH, 1, GL_RG_INTEGER,
                               GL_UNSIGNED_INT, texture_data.get() );

        auto v        = make_shared<Buffer>(             GL_ARRAY_BUFFER,
                                                4, verts, GL_STATIC_DRAW);

        auto quadPoly = make_shared<BufferPoly>(engine, v, xy::layout, 4);

        shared_ptr<Renderable> quad0( new BufferPoly::TextureInstance(quadPoly, simState) );

        engine.add_child(quad0);

        engine.look_at(0.f, 0.f, 10.f,
                       0.f, 0.f,  0.f,
                                  1.f);

        engine.draw_loop();
    } catch(const char * msg) {
        cout << "Error: " << msg << endl;
    }

    return 0;
}
