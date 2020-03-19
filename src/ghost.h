#ifndef __GHOST_AGENT__H
#define __GHOST_AGENT__H 

#include "enviro.h"

using namespace enviro;

class GhostController : public Process, public AgentInterface {

    public:
    GhostController() : Process(), AgentInterface(), vx(20) {}

    void init() {
        counter = 0;
        prevent_rotation();
        //teleport player back to start if they run into the ghost
        notice_collisions_with("Guy", [&](Event &e) {
            Agent& player = find_agent(e.value()["id"]); 
            player.teleport(0,135,0);
        });   

        //Remove ghost if hit by bullet
        notice_collisions_with("Bullet", [&](Event &e) {
            remove_agent(e.value()["id"]); //remove bullet
            remove_agent(id()); //remove ghost
        });  

        //eyes for ghost
        decorate(R"(<g>
            <circle cx=-5 cy=-3 r=2 style='fill:black'></circle>
            <circle cx=5 cy=-3 r=2 style='fill:black'></circle></g>)");
    }

    void start() {}

    void update() {
        //apply force to robot
        double fx = -30*(velocity().x-vx);
        omni_apply_force(fx,0);
        //change direction when near edge
        if ( counter > 0 ) {
            counter--;
        }
        if ((x() < -128 || x() > 128) && counter == 0){
            vx = -vx;
            counter = 10;
        }
            

    }

    void stop() {}

    double vx;
    int counter;


};

class Ghost : public Agent {
    public:
    Ghost(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    GhostController c;
};

DECLARE_INTERFACE(Ghost)

#endif