//Ryan Green and Connor Green, Final Project
#include<iostream>
#include<iomanip>
#include<string>
#include<array>
#include <unistd.h>

using namespace std;

class ship {

public:
  //Contruct/ Destruct
  ship();
  ~ship();
  //Return Functions
  int getfuel(); // get fuel level
  float getxspeed(); // get speed in x direction
  float getyspeed(); // return speed in y direction
  int getscore();  // return player score
  float gxposition();  // get x position on board
  float gyposition();  // get y position on board
  bool checkcrash(float);  // check crash based on position of ship
  float maxy(float);  // may
  bool checkLand();  // check if ship is close and snow enough to land

  //Void Functions
  void crashscreen(int); //Displays the User Message when there is a crash
  void shipbounds();  // start postion of ship, if it goes out of the bounds of the game
  void landship();  // pulls ship in to the land if given the right approach
  void landStats();  // assigns point value and alters disp
  void endgame();  // ends the game if user exits when user runs out of fuel
  void crashstats();  // returns the negative impact of crash
  void explosion(float);  // animates the explosion given the angle of impact
  void newposition();  // changes placement based on speed each time
  void newspeed(double);  // gives the updated speed of x and y if arrows used
  void setscore(int);  // set score value when changed
  void drawship( float, bool);  // draws ship based on angle, and ignites flame if needed

private:
  int fuel;  // limter of gameplay
  double xspeed;  // delta x
  double yspeed;  // delta y
  int score; // user score based on landing
  double xposition; // x location of nose
  double yposition; // y location of nose
  float rd;  // angle ship is flying at
  bool flame;  // ignites as a result of acceleration
};
