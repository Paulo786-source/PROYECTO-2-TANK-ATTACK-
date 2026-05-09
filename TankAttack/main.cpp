#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include "Renderer.h"
#include "Map.h"
#include "Cell.h"


int main() {
    srand(time(nullptr));
    Map map;
    Renderer renderer;
    renderer.initialize();

    while (renderer.windowOpen()) {
        renderer.beginFrame();
        renderer.drawMap(map);
        renderer.endFrame();
    }

    renderer.close();
    return 0;
}