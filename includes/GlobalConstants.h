#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

constexpr double SCALE_FACTOR = 1.5;

inline int scale(const double toScale) {
    return toScale*SCALE_FACTOR;
}

const int WINDOW_WIDTH = scale(800);
const int WINDOW_HEIGHT = scale(480);

const int TILE_SIZE = 80;
const int TILE_SIZE_SCALED = scale(80);

const float ACCELERATION = scale(5000);

constexpr int JOYSTICK_DEAD_ZONE = 18000;

#endif
