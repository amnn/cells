#include <cstdlib>

#define GLM_SWIZZLE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GL_includes.h"

#include "display_link/LockStepLink.h"
#include "screen/PixelatedScr.h"
#include "simulation/VisualisationEngine.h"

using namespace std;

int
main(int argc, char ** argv)
{
    if(argc < 2) return 0;

    float width  = strtof(argv[1], nullptr),
          height = strtof(argv[2], nullptr);

    try {
        Engine::LockStepLink            link;
        Engine::PixelatedScr            screen { int(width), int(height) };
        Simulation::VisualisationEngine engine { width,           height };

        screen.set_title("Cells");
        link(engine, screen);
    } catch(const char * msg) {
        cout << "Error: " << msg << endl;
    }

    return 0;
}
