#pragma once

#include "Entity.h"

namespace game {
    
    class Derp: public Entity {
    public:
        Derp(double x, double y):Entity(x, y, 14, 14) {}
        
        virtual bool update(double dt);
        virtual void draw();
        
    protected:
        double moveTime = 0;
        
    };
    
}