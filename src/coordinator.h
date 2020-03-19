#ifndef __COORDINATOR_AGENT__H
#define __COORDINATOR_AGENT__H 

#include "enviro.h"
#include <iostream>
#include <string>

using namespace enviro;
using namespace std;

class CoordinatorController : public Process, public AgentInterface {

    public:
    CoordinatorController() : Process(), AgentInterface(), playerCount(1), index(0) {
        //initialize color of players
        fillColor.push_back("lightcoral"); 
        fillColor.push_back("paleturquoise"); 
        fillColor.push_back("sandybrown"); 
    }

    void init() {
        watch("connection", [&](Event e) {
            if ( ! e.value()["client_id"].is_null() ) {
                std::cout << "Connection from " << e.value() << "\n";
                Agent& a = add_agent("Guy", x, 100, 0, {{"fill",fillColor[index]},{"stroke","black"}});
                a.set_client_id(e.value()["client_id"]);
                x += 50;
                a.label("P"+to_string(playerCount), -8, -20 );
                playerCount++;
                a.teleport(x,100,0);
            }
            index++;
            if (index == fillColor.size())
                index = 0;
        });
    }
    void start() {}
    void update() {}
    void stop() {}

    double x = -150;
    int playerCount, index;
    std::string healthID;
    vector<std::string> fillColor;

};

class Coordinator : public Agent {
    public:
    Coordinator(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    CoordinatorController c;
};

DECLARE_INTERFACE(Coordinator)

#endif