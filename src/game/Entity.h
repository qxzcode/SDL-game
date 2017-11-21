#pragma once

namespace game {
    
    class Entity {
    public:
        Entity(double x, double y, double rx, double ry):rx(rx),ry(ry),x(x),y(y) {}
        virtual ~Entity() {}
        
        virtual bool update(double dt) = 0;
        virtual void draw() = 0;
        
        double rx, ry;
        double x, y;
        double vx, vy;
        
    };
    
}