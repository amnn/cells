#include <iostream>
#include <memory>
#include <thread>
#include <cstdlib>

#include "GL_includes.h"

#define GLM_SWIZZLE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderable.h"
#include "RenderEngine.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "BufferPoly.h"
#include "Buffer.h"
#include "Texture.h"
#include "PixelatedScr.h"

#define FOV_MAX    90.f
#define FOV        45.f
#define FOV_MIN    10.f
#define NCP        0.1f
#define FCP      1000.f
#define TRN_SPD   0.01f
#define ROT_SPD     1.f

using namespace std;

struct xy
{

    static void layout( Buffer<xy> &b )
    {

        b.register_attrib( ATTRIB_VERT, 2, GL_FLOAT, GL_FALSE, 0, 0 );

    }

    float vert[ 2 ];

};

void engine_callback( Renderable &, const double & );

int main( int argc, char ** argv )
{

    if( argc < 4 ) {
        cout << "Usage: cells [width] [height] mind1 mind2 [...mind]\n";
        return 0;
    }

    float width  = strtof( argv[1], nullptr ),
          height = strtof( argv[2], nullptr );

    try {

        RenderEngine<PixelatedScr> engine ( width, height, NCP, FCP );
        shared_ptr<ShaderProgram>  prog   (       new ShaderProgram );

        prog->attach_shader( { GL_VERTEX_SHADER,   "assets/vert.glsl" } );
        prog->attach_shader( { GL_FRAGMENT_SHADER, "assets/frag.glsl" } );

        prog->link();

        prog->uniform( "MVP" );

        engine.scr().set_title( "Cells" );
        engine.use_program(        prog );

        auto quad = new xy[4]
        {
            { { 0.f   ,    0.f } },
            { { width ,    0.f } },
            { { 0.f   , height } },
            { { width , height } }
        };

        auto elems = new GLuint[4] { 0, 1, 2, 3 };

        auto v = make_shared<     Buffer<xy> >(          GL_ARRAY_BUFFER, 
                                                 4, quad, GL_STATIC_DRAW );
        
        auto e = make_shared< Buffer<GLuint> >(  GL_ELEMENT_ARRAY_BUFFER, 
                                                4, elems, GL_STATIC_DRAW );

        auto bp_q0 = make_shared<BufferPoly>(                  v, e, 20 );
        shared_ptr<Renderable> in_q0( new BufferPoly::Instance( bp_q0 ) );

        delete[] quad; delete[] elems;

        engine.add_child( in_q0 );

        engine.look_at( 0.f, 0.f, 10.f,
                        0.f, 0.f,  0.f,
                                   1.f );

        engine.draw_loop();

    } catch( char const * msg )
    {
        cout << msg << endl;
    }

    return 0;
}