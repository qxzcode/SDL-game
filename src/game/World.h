#pragma once

#include <memory> // std::unique_ptr
#include <forward_list> // std::forward_list

#include "Entity.h"

namespace game {
    
    class World {
    public:
        void addEntity(Entity* entity);
        void addEnvEntity(Entity* entity);
        
        void update(double dt);
        void draw();
        
        using entity_list_t = std::forward_list<std::unique_ptr<Entity>>;
        
    protected:
        entity_list_t entities;
        entity_list_t envEntities;
        
    };
    
}