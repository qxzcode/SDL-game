#pragma once

#include <cstdint> // uint8_t

// forward-declare this
struct SDL_Renderer;

namespace util {
    
    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    void strokeRect(int x, int y, int w, int h);
    void fillRect(int x, int y, int w, int h);
    
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
    
}