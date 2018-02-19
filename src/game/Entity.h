#pragma once

namespace game {
    
    class Entity {
    public:
        Entity(double x, double y, double rx, double ry):rx(rx),ry(ry),x(x),y(y) {}
        virtual ~Entity() {}
        
        virtual bool update(double dt) = 0;
        virtual void draw() = 0;
        
        void applyForce(double fx, double fy, double dt) {
            double mass = 1;//rx*ry / (20*20);
            vx += dt * fx/mass;
            vy += dt * fy/mass;
        }
        
        double rx, ry;
        double x, y;
        double vx, vy;
        
    };
    
}