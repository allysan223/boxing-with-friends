#ifndef __BULLET_AGENT__H
#define __BULLET_AGENT__H 

#include "enviro.h"

using namespace enviro;

class BulletController : public Process, public AgentInterface {

    public:
    BulletController() : Process(), AgentInterface() {}

    void init() {
        //Remove ghost if hit by bullet
        notice_collisions_with("Ghost", [&](Event &e) {
            remove_agent(e.value()["id"]);
            remove_agent(id());
        });           
    }
    void start() {}
    void update() {
        //remove bullet if hit by wall or object
        if (  abs(vx()) < 2 ) {
            remove_agent(id());
        }
    }
    void stop() {}

    double counter;

};

class Bullet : public Agent {
    public:
    Bullet(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    BulletController c;
};

DECLARE_INTERFACE(Bullet)

#endif