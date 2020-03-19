# boxing-with-friends
A local multiplayer platform/shooter game
===

Overview & Goals:
---
Boxing With Friends is an interactive local multiplayer game where the player's goal is to be the first to get to the star and be crowned winner. Players are controlled using keyboard inputs to move around the map and shoot bullets. Each player has a health bar, which when empty, teleports them back to the start of the map. Run into the ghost and it'll also bring the player back to the start. Players can also shoot the ghost, but after a few seconds it'll also respawn back into the map.

Catch the star and you win!

The goal of this project was to utilize various functions of ENVIRO such as:
 - Motion Control: Apply force to agents and teleportation
 - Collisions: Manages when players 'collide' with a ghost or bullet
 - Connection: Allow multiple players to join
 - Agent Management: Adding and removing agents
 - Styling: Adding labels and decorations to agents
 

Addressing Key Challenges
---
Some of the challenges faced were:
 - Dynamically creating the health bars as player had appeared in the game. Initally, when I had developed the game it was a 2-player game where both players were controlled off the same computer using different keyboard inputs and a health bar was dedicated for each player was displayed above the map. When adding the 'Connection' feature of ENVIRO, it became more difficult to manage. Instead, I had created the health bar as a decoration to the agent that is now displayed above the player themselves. 
 - Another challenge was managing bullets from the player, when the player move towards a certain direction that should also be the direction the bullet should shoot. This was done using a variable `prevState` which would keep track of the direction and shoot the bullet accordingly. The bullet is also continously reading its velocity, and once its velocity less than the velocity it was shot out at, that meant it hit a wall or object and should be removed.
 - Setting the decorations on the player was also a little tricky, as decorations are being updated depending on the player's motions and health status, which all had to be coordinated all under a single decoration string. 

Project Structure
---
The following structure was used for this project:
```
Makefile
config.json
defs/
    Contains the .JSON files for defining the various agents used
lib/
    Stores the shared objects (.so files) that are generated from the make file
src/
    Makefile
    Contains all the .h and .cc files for the agents
```

Installing & Running
---
This process assumes the user has docker installed and ENVIRO running. This program was written and tested at version 1.61.

Once in Docker, navigate to the code directory and run the following commands:
```
esm start
make clean
make 
enviro
```

Then navigate to `http://localhost` and a map should be displayed with your player added into the game.

To have other players join your game session, look up the local IP address on your computer and have your friends input that into their browser. You could also open `http://localhost` in another browser to start another player instance.

Controlling your player
---
Keyboard Inputs:

 `W`: Jump
 
 `A`: Move Left
 
 `D`: Move Right
 
 SPACEBAR: Shoot Bullet
 

Acknowledgements
---
This code runs heavily off of ENVIRO (https://github.com/klavinslab/enviro) and would like to thank Professor Klavins for his simulator and guidance and enthusiam for this project, Also to the TA's for their help and guidance. 

This project was also completed as part of the University of Washington curriculum EE P 520: Software Engineering for Embedded Applications
