#include "GL_includes.h"

#include "renderable/RenderEngine.h"

namespace Engine {

class FixedTickStepLink
{

public:
    template <class Scr>
    void operator()(RenderEngine<Scr> & engine, Scr & screen)
    {
        double last                   = glfwGetTime();
        static const double TICK_STEP = 1.0/60.f;

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
