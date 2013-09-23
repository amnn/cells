#include "GL_includes.h"

#include "display_link/DisplayLink.h"

#include "renderable/RenderEngine.h"

namespace Engine {

template <class Scr>
class LockStepLink : public DisplayLink<Scr>
{
public:

    using DisplayLink<Scr>::DisplayLink;

    void draw_loop()
    {
        double last = glfwGetTime();

        RenderEngine & engine = this->_engine;
        Scr          & screen = this->_screen;

        do {
            double now   = glfwGetTime(),
                   delta =    now - last;

            engine.thrd_req();
            engine.render(  );

            screen.swap();

            engine.tick( std::min(delta, 1.0/60) );
            engine.thrd_rel();

            last = now;
        } while( !screen.should_close_window() );
    }
};

};

