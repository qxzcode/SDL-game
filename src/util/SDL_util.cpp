#include "SDL_util.h"

#include <SDL.h>

SDL_Renderer* util::renderer;
SDL_Window* util::window;

void util::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

static int xOff = 0, yOff = 0;

void util::strokeRect(int x, int y, int w, int h) {
    SDL_Rect rect = {x+xOff, y+yOff, w, h};
    SDL_RenderDrawRect(renderer, &rect);
}

void util::fillRect(int x, int y, int w, int h) {
    SDL_Rect rect = {x+xOff, y+yOff, w, h};
    SDL_RenderFillRect(renderer, &rect);
}

void util::setDrawOffset(int x, int y) {
    xOff = x;
    yOff = y;
}


int util::winWidth() {
    int width;
    SDL_GetWindowSize(window, &width, nullptr);
    return width;
}

int util::winHeight() {
    int height;
    SDL_GetWindowSize(window, nullptr, &height);
    return height;
}


void util::processEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                //e.key.keysym.
                break;
        }
    }
}


static double totalTime = 0.0;

double util::getDeltaTime() {
    static Uint64 last = SDL_GetPerformanceCounter();
    Uint64 now = SDL_GetPerformanceCounter();
    double delta = (now - last) / double(SDL_GetPerformanceFrequency());
    last = now;
    totalTime += delta;
    return delta;
}

double util::getTotalTime() {
    return totalTime;
}

bool util::key::pressed(int scancode) {
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    return state[scancode];
}

bool util::key::moveUp()    { return pressed(SDL_SCANCODE_W); }
bool util::key::moveDown()  { return pressed(SDL_SCANCODE_S); }
bool util::key::moveLeft()  { return pressed(SDL_SCANCODE_A); }
bool util::key::moveRight() { return pressed(SDL_SCANCODE_D); }