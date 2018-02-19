#include "Derp.h"

#include <cmath>

#include "../util/SDL_util.h" // for user input & drawing
#include "../util/util.h"

using game::Derp;

constexpr double SPEED = 200;

bool Derp::update(double dt) {
    moveTime -= dt;
    if (moveTime <= 0 && false) {
        double a = util::rand()*2*util::PI;
        vx = SPEED*cos(a);
        vy = SPEED*sin(a);
        moveTime = 1.0 + util::rand()*2.5;
    }
    double f = pow(0.004, dt);
    vx *= f;
    vy *= f;
    return false;
}

void Derp::draw() {
    util::setColor(110, 255, 0);
    util::fillRect(x-rx, y-ry, rx*2, ry*2); 
}