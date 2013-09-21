#include "GL_includes.h"

#include "renderable/RenderEngine.h"

namespace Engine {

class LockStepLink
{

public:
    template <class Scr>
    void operator()(RenderEngine<Scr> & engine, Scr & screen)
    {
        double last = glfwGetTime();

        do {
            double now   = glfwGetTime(),
                   delta =    now - last;

            engine.thrd_req();
            engine.render(  );

            screen.swap();

            engine.tick( std::min(delta, 1.0/60) );
            engine.thrd_rel(  );

            last = now;
        } while( !screen.should_close_window() );
    }
};

};

