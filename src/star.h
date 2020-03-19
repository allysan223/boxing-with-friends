#ifndef __BUMPER_AGENT__H
#define __BUMPER_AGENT__H 

#include "enviro.h"

using namespace enviro;

class BumperController : public Process, public AgentInterface {

    public:
    BumperController() : Process(), AgentInterface() {}

    void init() {
        notice_collisions_with("Guy", [&](Event &e) {
            remove_agent(id());
            Agent& player = find_agent(e.value()["id"]); 
            std::cout << e.value()["id"] << "\n";
            e.value()["client_id"] == get_client_id();
            player.label("Winner!", -20, -20 );
        });  
        // notice_collisions_with("Player2", [&](Event &e) {
        //     remove_agent(id());
        //     Agent& player = find_agent(e.value()["id"]); 
        //     player.label("Winner!", -20, -20 );
        // });  
        std::cout << "star init done \n";
    }
    void start() {}
    void update() {}
    void stop() {}

};

class Bumper : public Agent {
    public:
    Bumper(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    BumperController c;
};

DECLARE_INTERFACE(Bumper)

#endif