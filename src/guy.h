#ifndef __PLAYER_AGENT__H
#define __PLAYER_AGENT__H 

#include "enviro.h"
#include <string>

using namespace enviro;

double min(double a, double b) {
    if ( a < b ) { 
        return a;
    } else {
        return b;
    }
}

class GuyController : public Process, public AgentInterface {

    public:
    GuyController() : Process(), AgentInterface(), LEFT(false), RIGHT(false), JUMP(false), healthCounter(0) {
        //init health bar vector
        for (int i = 0; i <= 50; i +=5){
            health_len.push_back(std::to_string(i)); 
        }
    }

    double height() {
        return min(sensor_value(0), sensor_value(1));
    }

    double airborne() { 
        return height() > H_MIN;
    }

    void reset_health(){
        healthCounter = 1;
    }

    void init() {
        reset_health();
        prevent_rotation();
        watch("keydown", [&](Event& e) {
            if ( e.value()["client_id"] == get_client_id() ) {
                //auto k = e.value()["key"].get<std::string>();
                std::string k = e.value()["key"];
                //shoot bullet in appropiate dirrection
                if ( k == " " ) {
                    if (prevState == "right"){
                    Agent& bullet = add_agent("Bullet", 
                                                x() + 17*cos(angle()), 
                                                y() + 17*sin(angle()), 
                                                angle(), 
                                                BULLET_STYLE);
                        bullet.apply_force(50,0);
                    }
                    if (prevState == "left") {
                        Agent& bullet = add_agent("Bullet", 
                                                x() - 17*cos(-angle()), 
                                                y() - 17*sin(-angle()), 
                                                -angle(), 
                                                BULLET_STYLE);
                        bullet.apply_force(-50,0);
                    }
                    
                    //firing = true;
                } else if ( k == "w" && ! airborne() ) {
                    JUMP = true;
                } else if ( k == "a" ) {
                    LEFT = true;
                    prevState = "left";
                } else if ( k == "d" ) {
                    RIGHT = true;
                    prevState = "right";
                } 
            }
        });
        watch("keyup", [&](Event& e) {
            if ( e.value()["client_id"] == get_client_id() ) {
                std::string k = e.value()["key"];
                if ( k == "a" ) {
                    LEFT = false;
                } else if ( k == "d" ) {
                    RIGHT = false;
                }
            }
        });     
        

              
    }
    void start() {}
    void update() {
        if (sensor_value(0) > 900)
            teleport(0,100,0);


        double fx;
        double fy = JUMP ? JUMP_F : 0;
        if ( !airborne() ) {
            if ( RIGHT ) {
                vx = VEL_X;
            } if ( LEFT ) {
                vx = -VEL_X;
            } else if ( !RIGHT && !LEFT ) {
                vx = 0;
            }
            fx = -K_X*(velocity().x-vx);
        } else {
            if ( RIGHT ) {
                vx = 0.1*VEL_X;
            } if ( LEFT ) {
                vx = -0.1*VEL_X;
            }            
            fx = 0;
            fx = -K_X*(velocity().x-vx);
        }
        omni_apply_force(fx,G+fy);
        JUMP = false;

        //decorations
        if (LEFT){ 
            decoration = "R\"(<g>"
                "<rect x=-20 y=-40 width=" + health_len[10-healthCounter] + " height=5 fill=\"red\" />"
                "<circle cx=-5 cy=-3 r=2 style='fill:black'></circle>"
                "</g>)\"";  
        } else if (RIGHT){  
            decoration = "R\"(<g>"
                "<rect x=-20 y=-40 width=" + health_len[10-healthCounter] + " height=5 fill=\"red\" />"
                "<circle cx=5 cy=-3 r=2 style='fill:black'></circle>"
                "</g>)\"";   
        } else {   
            decoration = "R\"(<g>"
                "<rect x=-20 y=-40 width=" + health_len[10-healthCounter] + " height=5 fill=\"red\" />"
                "<circle cx=-5 cy=-3 r=2 style='fill:black'></circle>"
                "<circle cx=5 cy=-3 r=2 style='fill:black'></circle>"
                "</g>)\"";  
        }
        
        decorate(decoration);
        
        notice_collisions_with("Bullet", [&](Event &e) {
            remove_agent(e.value()["id"]);
            healthCounter++;
            if (healthCounter == 10){
                reset_health();
                teleport(0,135,0);
                healthCounter = 1;}
            }); 

    }
    void stop() {}

    int healthCounter;

    bool LEFT, RIGHT, JUMP;
    double vx;

    std::string prevState, decoration;
    vector<std::string> health_len;

    const double VEL_X = 25;
    const double JUMP_F = -2200;
    const double K_X = 15;
    const double G = 500; //600
    const double H_MIN = 1.0;
    const json BULLET_STYLE = { 
                   {"fill", "slategrey"}, 
                   {"stroke", "#888"}, 
                   {"strokeWidth", "5px"},
                   {"strokeOpacity", "0.25"}
               };

};

class Guy : public Agent {
    public:
    Guy(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    GuyController c;
    
};

DECLARE_INTERFACE(Guy)

#endif