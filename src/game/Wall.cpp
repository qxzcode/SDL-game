#include "Wall.h"

#include "../util/SDL_util.h" // for drawing

using game::Wall;

bool Wall::update(double) {
    return false;
}

void Wall::draw() {
    util::setColor(0, 255, 255);
    util::strokeRect(x-rx, y-ry, rx*2, ry*2);
}