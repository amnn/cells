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

        static void layout( const ShaderProgram &p, Buffer &b )
        {
            b.register_attrib( p["vPos"], 2, GL_FLOAT, GL_FALSE, 0, 0 );
        }

    };
};

int main( int argc, char ** argv )
{

    if( argc < 2 ) { return 0; }

    float width  = strtof( argv[1], nullptr ),
          height = strtof( argv[2], nullptr );

    int   iW     = int(  width ),
          iH     = int( height );

    try {    

        RenderEngine<PixelatedScr> engine ( width, height, 0.1f, 100.f );
        shared_ptr<ShaderProgram>  prog (            new ShaderProgram );
        
        prog->attach_shader( { GL_VERTEX_SHADER,           "assets/sierp_vert.glsl" } );
        prog->attach_shader( { GL_FRAGMENT_SHADER, "assets/noise_frag_colored.glsl" } );

        prog->link();

        prog->uniform(  "MVP" );
        prog->attrib(  "vPos" );

        engine.scr().set_title( "Cells" );
        engine.use_program(        prog );

        xy verts[4] = {

            { 0.f,      0.f },
            { width,    0.f },
            { 0.f,   height },
            { width, height }

        };
        
        auto bitPattern = new GLuint[ iW * iH ];

        cout << "Using seed: " << Noise::perlin<1,6>( iW, iH, bitPattern ) << endl;

        Texture2D *pSierp = new Texture2D( GL_TEXTURE_RECTANGLE, 1, 
                                                   GL_R8UI, iW, iH );

        pSierp->param( GL_TEXTURE_MIN_FILTER,                     GL_NEAREST );
        pSierp->param( GL_TEXTURE_MAG_FILTER,                     GL_NEAREST );
        pSierp->param( GL_TEXTURE_WRAP_S,                   GL_CLAMP_TO_EDGE );
        pSierp->param( GL_TEXTURE_WRAP_T,                   GL_CLAMP_TO_EDGE );

        pSierp->image(        0, GL_RED_INTEGER, GL_UNSIGNED_INT, bitPattern );

        shared_ptr<Texture> sierp( pSierp );

        delete[] bitPattern;

        auto v        = make_shared< Buffer >(                GL_ARRAY_BUFFER, 
                                                     4, verts, GL_STATIC_DRAW );

        auto quadPoly = make_shared< BufferPoly >(   engine, v, xy::layout, 4 );
        
        shared_ptr<Renderable> quad0( new BufferPoly::TextureInstance( quadPoly, sierp ) );

        engine.add_child( quad0 );

        engine.look_at( 0.f, 0.f, 10.f,
                        0.f, 0.f,  0.f,
                                   1.f );

        engine.draw_loop();

    } catch( const char * msg ) {

        cout << "Error: " << msg << endl; 

    }    	

    return 0;
}