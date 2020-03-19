#ifndef __COORDINATOR_AGENT__H
#define __COORDINATOR_AGENT__H 

#include "enviro.h"
#include <iostream>
#include <string>

using namespace enviro;
using namespace std;

class CoordinatorController : public Process, public AgentInterface {

    public:
    CoordinatorController() : Process(), AgentInterface(), playerCount(1), index(0), ghostID(0) {
        //initialize colors of player to rotate with
        fillColor.push_back("lightcoral"); 
        fillColor.push_back("paleturquoise"); 
        fillColor.push_back("sandybrown"); 

        respawnCounter = 50;
    }

    void init() {
        //Watches for new connection and adds a player if so
        watch("connection", [&](Event e) {
            if ( ! e.value()["client_id"].is_null() ) {
                std::cout << "Connection from " << e.value() << "\n";
                Agent& a = add_agent("Guy", x, 100, 0, {{"fill",fillColor[index]},{"stroke","black"}});
                //sets unique client-id
                a.set_client_id(e.value()["client_id"]);
                x += 50;
                //label for player name
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
    void update() {
        //respawn ghost a little bit after it has been shot
        if (!agent_exists(ghostID)){
            respawnCounter--;
            if (respawnCounter == 0){
                Agent& ghost = add_agent("Ghost",0,80,0,GHOST_STYLE);
                ghostID = ghost.get_id();
                respawnCounter = 50;
            }
        }
    }
    void stop() {}

    double x = -150;
    int playerCount, index, ghostID, respawnCounter;
    std::string healthID;
    vector<std::string> fillColor;
    const json GHOST_STYLE = { 
                   {"fill", "ghostwhite"}, 
                   {"stroke", "black"}, 
                   {"strokeWidth", "2px"},
                   {"strokeOpacity", "0.25"}
               };

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