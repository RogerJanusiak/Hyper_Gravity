#ifndef PLATFORM_H
#define PLATFORM_H

#include "../Utils/GlobalConstants.h"
#include "../TEng/Texture.h"

class Platform {

public:

    Platform(int x, int y, SDL_Renderer* renderer);
    void render() const;

    [[nodiscard]] SDL_Rect getPlatformRect() const { return platformRectangle; }

    static constexpr int height = 10;

private:
    const int width = scale(TILE_SIZE);

    SDL_Rect platformRectangle;
    Texture texture;

    SDL_Renderer* renderer;
};

#endif //PLATFORM_H
