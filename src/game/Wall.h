#pragma once

#include "Entity.h"

namespace game {
    
    class Wall: public Entity {
    public:
        Wall(double x, double y):Entity(x, y, 60, 60) {}
        
        virtual bool update(double dt);
        virtual void draw();
        
    protected:
        //...
        
    };
    
}