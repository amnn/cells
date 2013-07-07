#include <iostream>
#include <memory>
#include <cstdlib>

#define GLM_SWIZZLE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GL_includes.h"

#include "RenderEngine.h"
#include "ShaderProgram.h"
#include "PixelatedScr.h"
#include "Buffer.h"
#include "BufferPoly.h"

using namespace std;

// TODO: Test Texture class init with data.
// TODO: Noise class (child of Texture).

struct xy {

    float vert[2];

    static void layout( const ShaderProgram &p, Buffer &b )
    {
        b.register_attrib( p["vPos"], 2, GL_FLOAT, GL_FALSE, 0, 0 );
    }

};

int main( int argc, char ** argv )
{

    if( argc < 2 ) { return 0; }

    float width  = strtof( argv[1], nullptr ),
          height = strtof( argv[2], nullptr );

    try {    

        RenderEngine<PixelatedScr> engine ( width, height, 0.1f, 100.f );
        shared_ptr<ShaderProgram>  prog (            new ShaderProgram );
        
        prog->attach_shader( { GL_VERTEX_SHADER,   "assets/sierp_vert.glsl" } );
        prog->attach_shader( { GL_FRAGMENT_SHADER, "assets/sierp_frag.glsl" } );

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
        
        auto v = make_shared< Buffer >(          GL_ARRAY_BUFFER, 
                                        4, verts, GL_STATIC_DRAW );

        auto quadPoly = make_shared< BufferPoly >( engine, v, xy::layout, 4 );
        shared_ptr<Renderable> quad0(  new BufferPoly::Instance( quadPoly ) );

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