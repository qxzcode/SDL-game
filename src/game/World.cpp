#include "World.h"

#include <vector>
#include <cmath>
#include <limits>
static constexpr double inf = std::numeric_limits<double>::infinity();

using namespace game;

#include "Wall.h"

void World::addEntity(std::unique_ptr<Entity> entity) {
    entities.push_front(std::move(entity));
}

void World::addEnvEntity(std::unique_ptr<Entity> entity) {
    envEntities.push_front(std::move(entity));
}

// temporary physics/collision information for an Entity
struct physics_data {
    physics_data(Entity* e, double dt):entity(e) {
        min[0] = e->x - e->rx;
        min[1] = e->y - e->ry;
        max[0] = e->x + e->rx;
        max[1] = e->y + e->ry;
        v[0] = e->vx * dt;
        v[1] = e->vy * dt;
    }
    
    void reset(double timeTo, double timeLeft) {
        collision = nullptr;
        collisionTime = inf;
        entity->x += v[0]*timeTo;
        entity->y += v[1]*timeTo;
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

static double calcEntryExit(physics_data& e1, physics_data& e2, uint8_t axis, double* entry, double* exit) {
    *entry = +inf;
    *exit = -inf;
    double v = e1.v[axis]-e2.v[axis];
    if (v != 0) {
        *entry = (e2.min[axis] - e1.max[axis]) / v;
        *exit  = (e2.max[axis] - e1.min[axis]) / v;
        if (v < 0) std::swap(*entry, *exit);
        if (*entry > *exit) printf("gghkkhggh\n");
    } else if (e1.min[axis] < e2.max[axis] && e1.max[axis] > e2.min[axis]) {
        std::swap(*entry, *exit);
    }
    return v;
}

extern const double EPSILON;
const double EPSILON = 1;
static void testCollision(physics_data& e1, physics_data& e2) {
    // find X entry/exit time
    double xEntry, xExit;
    double vx = calcEntryExit(e1, e2, 0, &xEntry, &xExit);
    
    // find y entry/exit time
    double yEntry, yExit;
    double vy = calcEntryExit(e1, e2, 1, &yEntry, &yExit);
    
    double entryTime = std::max(xEntry, yEntry);
    double exitTime  = std::min(xExit,  yExit);
    //printf("%.17f  \t%.17f\n", e1.min[1], e2.max[1]);
    
    
    double xDist = std::abs(vx)*entryTime;
    double yDist = std::abs(vy)*entryTime;
    // if ((yDist>-EPSILON && yDist<0) && entryTime<exitTime) {
    //     bool isOnX = xEntry > yEntry;
    //     printf("\n---- totoald ----\n");
    //     printf("entry=%f  \texit=%f\n", entryTime, exitTime);
    //     printf("X: entry=%f  \texit=%f\n", xEntry, xExit);
    //     printf("xDist=%f  \tyDist=%f\n", xDist, yDist);
    //     printf("colliding axis = %c\n", isOnX? 'X' : 'Y');
    //     static_cast<Wall*>(e2.entity)->doot = 3;
    // }
    // printf("%d %d %d\n", entryTime<0, xDist<EPSILON, yDist<EPSILON);
    // if (entryTime<0 && !(xDist<EPSILON || yDist<EPSILON))
        
    
    
    if (entryTime > exitTime || (vx==0 && vy==0) || (xDist<-EPSILON || yDist<-EPSILON) || entryTime>1) {
        // no collision in this frame
    } else {
        if (entryTime < e1.collisionTime) {
            e1.collisionTime = entryTime;
            e1.collision = &e2;
            e1.collisionOnX = xEntry > yEntry;
        }
        //printf("COLLISION\n");
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
    // update entity logic & initialize collision information
    auto pData  = updateEntities(entities, dt);
    auto epData = updateEntities(envEntities, dt);
    
    int count = 0;
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
        
        if (first) {count++;
            physics_data* e2 = first->collision;static_cast<Wall*>(e2->entity)->doot = 3;
            // log object positions and [stuf]
            // printf("%.22f, %.22f\n", e2->entity->x, e2->entity->y);
            
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
    } while (true);//if (count > 0) printf("collisions: %d\n", count);
    
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