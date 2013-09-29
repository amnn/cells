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

    int   iW     = int( width),
          iH     = int(height);

    try {
        Engine::PixelatedScr            screen     { iW, iH };
        Simulation::Simulation          simulation { iW, iH, {"1", "2", "3"} };
        Simulation::VisualisationEngine engine     { simulation };

        Engine::LockStepLink<Engine::PixelatedScr> link { engine, screen };

        screen.set_title("Cells");
        link.draw_loop();
    } catch(const char * msg) {
        cout << "Error: " << msg << endl;
    }

    return 0;
}
