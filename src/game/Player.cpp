#include "Player.h"

#include <cmath>

#include "../util/SDL_util.h" // for user input & drawing

using game::Player;

constexpr double SQRT1_2 = 0.707106781186547524401;

bool Player::update(double dt) {
    double mx = 0;
    if (util::key::moveLeft())  mx--;
    if (util::key::moveRight()) mx++;
    double my = 0;
    if (util::key::moveUp())   my--;
    if (util::key::moveDown()) my++;
    if (mx != 0 && my != 0) {
        mx *= SQRT1_2;
        my *= SQRT1_2;
    }
    const double amt = 3000;
    vx += mx*amt*dt;
    vy += my*amt*dt;
    double f = pow(0.02, dt);
    vx *= f;
    vy *= f;
    return false;
}

void Player::draw() {
    util::setColor(255, 110, 0);
    util::fillRect(x-rx, y-ry, rx*2, ry*2); 
}