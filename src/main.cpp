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

#define FOV_MAX   90.f
#define FOV       45.f
#define FOV_MIN   10.f
#define NCP       0.1f
#define FCP      100.f
#define TRN_SPD  0.01f
#define ROT_SPD    1.f

using namespace std;

struct xyzuv
{

    static void layout( Buffer<xyzuv> &b )
    {

        b.register_attrib( ATTRIB_VERT,  3, GL_FLOAT, GL_FALSE, sizeof( float )*3, 0 );

    }

    float vert[ 3 ];

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

        prog->attach_shader( Shader( GL_VERTEX_SHADER,   "assets/vert.glsl" ) );
        prog->attach_shader( Shader( GL_FRAGMENT_SHADER, "assets/frag.glsl" ) );

        prog->link();

        prog->uniform( "MVP" );

        engine.scr().set_title( "Cells" );
        engine.use_program(        prog );

        auto cube = new xyzuv[14]
        {
            { {  1.f,  1.f,  1.f } }, //0
            { { -1.f,  1.f,  1.f } }, //2
            { { -1.f,  1.f, -1.f } }, //7
            { {  1.f,  1.f, -1.f } }, //6
            { {  1.f, -1.f,  1.f } }, //1
            { {  1.f, -1.f,  1.f } }, //1
            { {  1.f, -1.f,  1.f } }, //1
            { { -1.f, -1.f,  1.f } }, //3
            { { -1.f, -1.f,  1.f } }, //3
            { { -1.f, -1.f,  1.f } }, //3
            { { -1.f, -1.f, -1.f } }, //4
            { { -1.f, -1.f, -1.f } }, //4
            { {  1.f, -1.f, -1.f } }, //5
            { {  1.f, -1.f, -1.f } }  //5

        };

        auto elems = new GLuint[20] { 4, 7, 0, 1, 3, 2, 12, 11, 6, 9, 
                                      65535, 5, 13, 0, 3, 
                                      65535, 2, 1, 10, 8 };

        auto v     = new Buffer<  xyzuv > (         GL_ARRAY_BUFFER, 14, 
                                                   cube, GL_STATIC_DRAW );

        auto e     = new Buffer< GLuint > ( GL_ELEMENT_ARRAY_BUFFER, 20, 
                                                  elems, GL_STATIC_DRAW );

        shared_ptr< Buffer<  xyzuv > > sp_v( v );
        shared_ptr< Buffer< GLuint > > sp_e( e );

        auto bp_c0 = shared_ptr<BufferPoly>( new BufferPoly( sp_v, sp_e, 20 ) );

        shared_ptr<Renderable> c0( new BufferPoly::Instance( bp_c0 ) );

        delete[] cube; delete[] elems;

        engine.add_child( c0 );

        c0->transform() = glm::scale( glm::mat4(1.f), glm::vec3(0.5f) );

        c0->transform() = glm::translate( c0->transform(), glm::vec3( 0.f,  0.f, 0.f ) );

        c0->callback()  = [](Renderable &r, const double &d) {

            glm::mat4 &mat = r.transform();

            mat = glm::rotate( mat, static_cast<float>(30*d), glm::vec3(1.f,1.f,0.f) );

        };

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