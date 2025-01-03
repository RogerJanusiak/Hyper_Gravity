#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

constexpr double SCALE_FACTOR = 2.5;
constexpr double UI_SCALE_FACTOR = 1*SCALE_FACTOR;
constexpr double PLAYER_UI_SCALE_FACTOR = 1*SCALE_FACTOR;

inline int scale(const double toScale) { return static_cast<int>(toScale*SCALE_FACTOR); }
inline int scaleUI(const double toScale) {  return static_cast<int>(toScale*UI_SCALE_FACTOR);  }
inline int scalePlayerUI(const double toScale) {  return static_cast<int>(toScale*PLAYER_UI_SCALE_FACTOR);  }

constexpr int numberOfEnemies = 5;

const int WINDOW_WIDTH = scale(640);
const int WINDOW_HEIGHT = scale(360);

const int TILE_SIZE = 40;
const int TILE_SIZE_SCALED = scale(40);

const float ACCELERATION = scale(2500);
const float TERMINAL_VELOCITY = scale(500);

constexpr int JOYSTICK_DEAD_ZONE = 18000;

const int groundPoundRadius = scale(75);
const int maxEnemyHealth = 5;

#endif
