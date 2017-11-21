#pragma once

#include "Entity.h"

namespace game {
    
    class Player: public Entity {
    public:
        Player(double x, double y):Entity(x, y, 20, 20) {}
        
        virtual bool update(double dt);
        virtual void draw();
        
    protected:
        //...
        
    };
    
}