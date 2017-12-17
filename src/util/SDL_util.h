#pragma once

#include <cstdint> // uint8_t

// forward-declare these
struct SDL_Renderer;
struct SDL_Window;

namespace util {
    
    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    void strokeRect(int x, int y, int w, int h);
    void fillRect(int x, int y, int w, int h);
    void setDrawOffset(int x, int y);
    
    int winWidth();
    int winHeight();
    
    void processEvents();
    double getDeltaTime();
    double getTotalTime();
    
    namespace key {
        bool pressed(int scancode);
        
        bool moveUp();
        bool moveDown();
        bool moveLeft();
        bool moveRight();
    }
    
    extern SDL_Renderer* renderer;
    extern SDL_Window* window;
    
}