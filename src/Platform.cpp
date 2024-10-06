#include "../includes/Platform.h"

Platform::Platform(int x, int y, SDL_Renderer* renderer) {
    box.x = x*SCALE_FACTOR;
    box.y = (y-25)*SCALE_FACTOR;
    box.h = height;
    box.w = width;

    texture.setup(width,height,renderer);

    if(!texture.loadFromFile("platform.png")) {
        SDL_Log("Could not load platform.png texture!");
    }

}

void Platform::render() {
    texture.render(box.x,box.y);
}