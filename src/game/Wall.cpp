#include "Wall.h"

#include <cmath>

#include "../util/SDL_util.h" // for user input & drawing

using game::Wall;

static double t = 0;
bool Wall::update(double dt) {
    t += dt*0.25;
    // vx = 25*sin(t);
    // vy = 25*cos(t);
    return false;
}

void Wall::draw() {
    util::setColor(0, 255, 255);
    util::strokeRect(x-rx, y-ry, rx*2, ry*2);
}