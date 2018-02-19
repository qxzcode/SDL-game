#include "World.h"

#include <vector>
#include <cmath>
#include <limits>
static constexpr double inf = std::numeric_limits<double>::infinity();

using namespace game;

#include "Wall.h"

void World::addEntity(Entity* entity) {
    entities.push_front(std::unique_ptr<Entity>(entity));
}

void World::addEnvEntity(Entity* entity) {
    envEntities.push_front(std::unique_ptr<Entity>(entity));
}

// temporary physics/collision information for an Entity
struct physics_data {
    physics_data(Entity* e, double dt):entity(e) {
        updateBounds();
        v[0] = e->vx * dt;
        v[1] = e->vy * dt;
    }
    
    void updateBounds() {
        min[0] = entity->x - entity->rx;
        min[1] = entity->y - entity->ry;
        max[0] = entity->x + entity->rx;
        max[1] = entity->y + entity->ry;
    }
    
    void reset(double timeTo, double timeLeft) {
        collision = nullptr;
        collisionTime = inf;
        entity->x += v[0]*timeTo;
        entity->y += v[1]*timeTo;
        updateBounds();
        v[0] *= timeLeft;
        v[1] *= timeLeft;
    }
    
    Entity* entity;
    double min[2];
    double max[2];
    double v[2];
    
    physics_data* collision = nullptr;
    bool collisionOnX;
    double collisionTime = inf;
};

static double calcEntryExit(const physics_data& e1, const physics_data& e2, uint8_t axis, double* entry, double* exit) {
    *entry = +inf;
    *exit = -inf;
    double v = e1.v[axis]-e2.v[axis];
    if (v != 0) {
        *entry = (e2.min[axis] - e1.max[axis]) / v;
        *exit  = (e2.max[axis] - e1.min[axis]) / v;
        if (v < 0) std::swap(*entry, *exit);
    } else if (e1.min[axis] < e2.max[axis] && e1.max[axis] > e2.min[axis]) {
        std::swap(*entry, *exit);
    }
    return v;
}

constexpr double EPSILON = 1;
static void testCollision(physics_data& e1, physics_data& e2) {
    // find X entry/exit time
    double xEntry, xExit;
    double vx = calcEntryExit(e1, e2, 0, &xEntry, &xExit);
    
    // find Y entry/exit time
    double yEntry, yExit;
    double vy = calcEntryExit(e1, e2, 1, &yEntry, &yExit);
    
    // total entry/exit time
    double entryTime = std::max(xEntry, yEntry);
    double exitTime  = std::min(xExit,  yExit);
    
    double xDist = std::abs(vx)*entryTime;
    double yDist = std::abs(vy)*entryTime;
    
    if (entryTime > exitTime || (vx==0 && vy==0) || (xDist<-EPSILON || yDist<-EPSILON) || entryTime>1) {
        // no collision in this frame
    } else {
        if (entryTime < e1.collisionTime) {
            e1.collisionTime = entryTime;
            e1.collision = &e2;
            e1.collisionOnX = xEntry > yEntry;
        }
    }
}

static auto updateEntities(World::entity_list_t& entities, double dt) {
    std::vector<physics_data> pData;
    for (auto it = entities.begin(); it != entities.end(); ) {
        auto& e = *it;
        if (e->update(dt)) {
            // remove entity
            it = entities.erase_after(it);
        } else {
            pData.emplace_back(e.get(), dt);
            ++it;
        }
    }
    return pData;
}

void World::update(double dt) {
    // handle "soft" non-envionment collisions
    for (auto it1 = entities.begin(); it1 != entities.end(); it1++) {
        auto& e1 = *it1;
        auto it2 = it1;
        it2++;
        for (; it2 != entities.end(); it2++) {
            auto& e2 = *it2;
            double dx = e2->x-e1->x, rx = e1->rx+e2->rx;
            double dy = e2->y-e1->y, ry = e1->ry+e2->ry;
            if (std::abs(dx) < rx && std::abs(dy) < ry) {
                double d = std::hypot(dx, dy);
                double r = std::hypot(rx, ry);
                double force = 2000 * (1.0 - d/r);//r/d * 1000;
                dx *= force/d;
                dy *= force/d;
                e1->applyForce(-dx, -dy, dt);
                e2->applyForce(+dx, +dy, dt);
                // printf("E-E COLL\n");
            }
        }
    }
    
    // update entity logic & initialize collision information
    auto pData  = updateEntities(entities, dt);
    auto epData = updateEntities(envEntities, dt);
    
    // handle environment collisions
    do {
        // detect collisions and get the first one
        physics_data* first = nullptr;
        for (auto& pd : pData) {
            for (auto& epd : epData) {
                testCollision(pd, epd);
            }
            if (pd.collision && (!first || pd.collisionTime < first->collisionTime)) {
                first = &pd;
            }
        }
        
        if (first) {
            physics_data* e2 = first->collision;
            
            // move everything up to the point of the collision
            double timeTo = first->collisionTime;
            double timeLeft = 1.0 - timeTo;
            for (auto& pd : pData)  pd.reset(timeTo, timeLeft);
            for (auto& pd : epData) pd.reset(timeTo, timeLeft);
            
            // apply the collision by changing the entity's velocity
            if (first->collisionOnX) {
                first->entity->vx = e2->entity->vx;
                first->v[0] = e2->v[0];
            } else {
                first->entity->vy = e2->entity->vy;
                first->v[1] = e2->v[1];
            }
        } else {
            // no more collisions to handle
            break;
        }
    } while (true);
    
    // move entities to their final positions
    for (auto& pd : pData) {
        pd.entity->x += pd.v[0];
        pd.entity->y += pd.v[1];
    }
    for (auto& pd : epData) {
        pd.entity->x += pd.v[0];
        pd.entity->y += pd.v[1];
    }
}

void World::draw() {
    for (auto& e : envEntities) {
        e->draw();
    }
    for (auto& e : entities) {
        e->draw();
    }
}