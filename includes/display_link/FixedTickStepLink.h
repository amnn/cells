#include "GL_includes.h"

#include "display_link/DisplayLink.h"

#include "renderable/RenderEngine.h"

namespace Engine {

template <class Scr>
class FixedTickStepLink : public DisplayLink<Scr>
{
public:

    using DisplayLink<Scr>::DisplayLink;

    void draw_loop()
    {
        double last                   = glfwGetTime();
        static const double TICK_STEP = 1.0/60.f;

        RenderEngine & engine = this->_engine;
        Scr          & screen = this->_screen;

        do {
            engine.thrd_req();
            engine.render(  );

            screen.swap();

            double now   = glfwGetTime(),
                   delta =    now - last;

            while(delta >= 0)
            {
                engine.tick( std::min(delta, TICK_STEP) );
                delta -= TICK_STEP;
            }

            last = glfwGetTime();
            engine.thrd_rel(  );
        } while( !screen.should_close_window() );
    }
};

};
