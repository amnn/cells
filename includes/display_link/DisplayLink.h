#ifndef DISPLAY_LINK_H
#define DISPLAY_LINK_H

#include "GL_includes.h"

#include "renderable/RenderEngine.h"

namespace Engine {

template <class Scr>
class DisplayLink {

protected:

    RenderEngine & _engine;
    Scr          & _screen;

public:

    DisplayLink() = delete;

    DisplayLink(
        RenderEngine & engine,
        Scr          & screen
    )
        : _engine(engine)
        , _screen(screen)
    {};

    virtual void draw_loop() = 0;
};

}; // namespace Engine

#endif
