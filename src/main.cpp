#include <iostream>
#include <memory>
#include <thread>

#include "GL_includes.h"

#define GLM_SWIZZLE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderable.h"
#include "RenderEngine.h"
#include "ShaderProgram.h"
#include "BufferPoly.h"
#include "Buffer.h"
#include "Texture.h"
#include "GLFWScr.h"

#define FOV_MAX   90.f
#define FOV       45.f
#define FOV_MIN   10.f
#define NCP       0.1f
#define FCP      100.f
#define SCR_W   1024.f
#define SCR_H    768.f
#define SCR_C_X 1024/2
#define SCR_C_Y  768/2
#define TRN_SPD  0.01f
#define ROT_SPD    1.f

using namespace std;

/* TODO

* Tween class
* New BufferPoly constructor initialise from file.

*/

struct xyzuv
{

    static void layout( Buffer<xyzuv> &b )
    {

        b.register_attrib( ATTRIB_VERT,  3, GL_FLOAT, GL_FALSE, sizeof( float )*3, 0 );

    }

    float vert[ 3 ];

};

void engine_callback( Renderable &, const double & );

int main( int, char ** )
{

    try {

        RenderEngine<GLFWScr>     engine             ( SCR_W, SCR_H, FOV, NCP, FCP );
        shared_ptr<ShaderProgram> prog   ( new ShaderProgram( "assets/vert.glsl", 
                                                              "assets/frag.glsl" ) );

        engine.use_program( prog );

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

        shared_ptr<Renderable> tiger( new BufferPoly::Instance( bp_c0 ) );

        delete[] cube; delete[] elems;

        engine.add_child( tiger );

        tiger->transform() = glm::scale( glm::mat4(1.f), glm::vec3(0.5f) );

        tiger->transform() = glm::translate( tiger->transform(), glm::vec3( 0.f,  0.f, 0.f ) );

        tiger->callback() = [](Renderable &r, const double &d) {

            glm::mat4 &mat = r.transform();

            mat = glm::rotate( mat, static_cast<float>(30*d), glm::vec3(1.f,1.f,0.f) );

        };

        engine.callback() = &engine_callback;

        engine.draw_loop();

    } catch( char const * msg )
    {
        cout << msg << endl;
    }

    return 0;
}

void engine_callback( Renderable &r, const double &d )
{

    static int   lastMW  =        glfwGetMouseWheel();
    static float fov     =                        FOV;
    static glm::vec3 eye           ( 0.f, 0.f, 10.f ), 
                     center        (            0.f ), 
                     up            ( 0.f, 1.f,  0.f ),
                     xAxis         ( 1.f, 0.f,  0.f );

    // Zoom on mouse wheel.
    int mw               =        glfwGetMouseWheel();
    fov                 +=                lastMW - mw;
    int x, y;               glfwGetMousePos( &x, &y );
    int dx               =                x - SCR_C_X,
        dy               =                y - SCR_C_Y;

    if      ( fov < FOV_MIN ) fov = FOV_MIN;
    else if ( fov > FOV_MAX ) fov = FOV_MAX;

    if ( glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS )
    {
        // pan
        glm::mat4 trans = glm::translate( glm::mat4(1.f), glm::vec3(

            -dx * TRN_SPD,
             dy * TRN_SPD,
                      0.f

        ) );

        eye    = glm::vec3( trans * glm::vec4( eye,    1.f ) );
        center = glm::vec3( trans * glm::vec4( center, 1.f ) );


    } else {
        // rotate

        glm::mat4 xRot = glm::rotate( glm::mat4(1.f), ROT_SPD*dy, xAxis );
        up             = glm::vec3(      xRot * glm::vec4(    up, 0.f ) );
        glm::mat4 yRot = glm::rotate( glm::mat4(1.f), ROT_SPD*dx,    up );
        xAxis          = glm::vec3(      yRot * glm::vec4( xAxis, 0.f ) );

        eye            = center + glm::vec3( yRot * xRot * glm::vec4( eye - center, 0.f ) );

    }

    // Reset on middle mouse button click.
    if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_MIDDLE ) == GLFW_PRESS )
    {
        fov    =                         FOV;
        eye    = glm::vec3( 0.f, 0.f, 10.f );
        center = glm::vec3(            0.f );
        up     = glm::vec3( 0.f, 1.f,  0.f );
        xAxis  = glm::vec3( 1.f, 0.f,  0.f );
    }

    glm::mat4 proj = glm::perspective( fov, SCR_W/SCR_H, NCP, FCP ),
              view = glm::lookAt( eye, center, up );

    r.transform()  = proj * view;

    lastMW = mw; glfwSetMousePos( SCR_C_X, SCR_C_Y );

}