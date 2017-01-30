#include "ship.h"
#include "gfxnew.h"
#include <cmath>
#include <math.h>
#include <unistd.h>

using namespace std;

  ship::ship(){  // constructs intial valus of the ship stats, placement and speed
    fuel = 1000;
    xspeed = 0.3;
    yspeed = 0.3;
    score = 0;
    xposition = 75;
    yposition = 250;
  }
  ship::~ship(){}

  //Return Functions  // return named values to be used in main
  int ship::getfuel(){return fuel;}
  float ship::getxspeed(){return xspeed;}
  float ship::getyspeed(){return yspeed;}
  int ship::getscore(){return score;}
  float ship::gxposition(){return xposition;}
  float ship::gyposition(){return yposition;}

  //Void Functions
  void ship::setscore(int newscore){ score = newscore;} // updates score
  void ship::drawship(float rd,bool flame){  // takes in angle and flame
    float rad2,rad3;
    int fire = 20; // will be used as base for fill, set to 20 to not show
    if (flame == true){
      fire = 55;  // makes longer when need flame
    }
    rad2 = rd - M_PI/2;
    // works using trig, back from the tip of the nose. this was required to all
    // for ease of use, could move and rotate the whole instead of part by part
    // begins with rotation triangle, draws line out, then connects the two
    // open end points
    gfx_line(xposition,yposition,xposition+7*cos(rad2),yposition-7*sin(rad2));
    gfx_line(xposition,yposition,xposition-7*cos(rd),yposition+7*sin(rd));
    gfx_line(xposition-20*cos(rd+M_PI/6),yposition+20*sin(rd+M_PI/6),xposition-7*cos(rd),yposition+7*sin(rd));
    gfx_line(xposition+20*cos(rad2-M_PI/6),yposition-20*sin(rad2-M_PI/6),xposition+7*cos(rad2),yposition-7*sin(rad2));
    gfx_line(xposition+20*cos(rad2-M_PI/6),yposition-20*sin(rad2-M_PI/6),xposition-20*cos(rd+M_PI/6),yposition+20*sin(rd+M_PI/6));
    rad3 = (7*M_PI/4-rd);
    // used the fill polygon to create a filled flame that represents force
    XPoint mypts[] = {
      {(short)(xposition+20*cos(rad2-M_PI/6)),(short)(yposition-20*sin(rad2-M_PI/6))},
        {(short)(xposition-20*cos(rd+M_PI/6)),(short)(yposition+20*sin(rd+M_PI/6))},
          {(short)(xposition-fire*cos(rad3)),(short)(yposition-fire*sin(rad3))}};
    int sz = sizeof(mypts)/sizeof(XPoint);
    gfx_fill_polygon(mypts, sz);  // draw the filled polygon
  }
  void ship::newposition(){  // alters the place of the nose through each loop
    yspeed = yspeed*(0.99);
    xspeed = xspeed*(0.99);
    xposition = xposition + xspeed;
    yposition = yposition + yspeed + 0.1;
  }
  void ship::newspeed(double rad){  // if speed is adjusted, keeps in line with
    // the goemety and angle of the ship and accelerates as if it was pushed
    xspeed = xspeed + 0.2*cos(rad - (7*M_PI/4));
    yspeed = yspeed - 0.2*sin(rad- (7*M_PI/4));
    fuel = fuel - 1;
  }
  bool ship::checkcrash(float rd){  // checks the five sides to find if a corner
    // was crossed, this would, given the geometry of the ship all us to
    // more easily recognize a crash by checking if corner passed a line.
    bool crash = true;
    float rad2 = rd - M_PI/2;
    //Find y Value of line at that x point and compare to current xposition, using point slope form of line
    if(yposition > maxy(xposition)){crash = false;}
    if((yposition-7*sin(rad2)) > maxy(xposition+7*cos(rad2))){crash = false;}
    if((yposition+7*sin(rd)) > maxy(xposition-7*cos(rd))){crash = false;}
    if((yposition+20*sin(rd-M_PI/6)) > maxy(xposition-20*cos(rd+M_PI/6))){crash = false;}
    if((yposition-20*sin(rad2+M_PI/6)) > maxy(xposition+20*cos(rad2-M_PI/6))){crash = false;}

    return crash;
  }
  void ship::crashstats(){  // gives the change in stats if crashes with gas left
    // returns to original placement, and lowers gas as punishment
    xposition = 50;
    yposition = 175;
    fuel = fuel - 200;
    yspeed = 0.3;
    xspeed = 0.3;
  }
  void ship::explosion(float rd){  // animation given a collision, theta is
    // need to keep explosion in line with the postion of the ship
    int l = 0,count = 0;
    rd= (7*M_PI/4-rd);
    while (count < 15){  // flashes growing cirlce
      gfx_fill_circle(xposition,yposition,l);
      gfx_fill_circle(xposition-20*cos(rd),yposition-20*sin(rd),l);
      gfx_color(0,0,0);
      gfx_fill_circle(xposition,yposition,l);
      gfx_fill_circle(xposition-20*cos(rd),yposition-20*sin(rd),l);
      gfx_color(255,255,255);
      gfx_fill_circle(xposition,yposition,l);
      gfx_fill_circle(xposition-20*cos(rd),yposition-20*sin(rd),l);
      usleep(300);
      count ++;
      l++;
    }
  }
  void ship::endgame(){  // clears and then displays a fuel loss error, and
    // final score
    gfx_clear();
    gfx_flush();
    fuel = 0;  // makes sure fuel will never be negative if, for instance, fuel
    // ran out on a crash
    //Present the User Their Score
    char thefont[] = "-sony-fixed-medium-r-normal--24-170-100-100-c-120-iso8859-1";
    gfx_changefont(thefont);
    string sscore = to_string(score);
    char const *cscore = sscore.c_str();
    gfx_text(200, 400, "You Ran Out of Fuel, Thank You For Playing ");
    gfx_text(275, 430, "Your Final Score Was: ");
    gfx_text(575, 430, cscore);
    gfx_text(375, 500, "Press q to Quit");
  }
  float ship::maxy(float x){
    int p = 0;
    float mheight;
    float xvalues[] = {0, 50, 100, 125, 130, 135, 140, 145, 150, 275, 300, 350, 375, 375, 400, 450, 525, 550, 600, 610, 620, 650, 700, 750, 850, 875, 900, 920, 980, 1000};
    float yvalues[] = {400, 500, 500, 650, 660, 675, 680, 675, 650, 600, 600, 500, 450, 400, 400, 300, 475, 475, 550, 590, 600, 625, 625, 700, 700, 600, 450, 450, 440, 350};
    //Run through loop to find what line to compare it too
    while(xvalues[p] < x){p++;}
    //Compute y value of the Moon Surface
    mheight = (((yvalues[p-1]-yvalues[p])/(xvalues[p-1]-xvalues[p]))*(x - xvalues[p]) + yvalues[p]);
    return mheight;
  }
  bool ship::checkLand(){
    bool safe = false;
    // two arrays of the flat lines where a user can score points
    int xvalues[] = {50,100, 275,300, 375,400, 525,550, 650,700, 700,850,  900,920};
    int yvalues[] = {500,500, 600,600, 400,400, 475,475, 625,625, 700,700,  450,450};
    int i;
    for(i = 0; i<14;i += 2){
      // iterates through the lines given where the x and y of the ship is
      // if ship in valid x rangle, and is close enough to the surface, lands ship
      if(xposition < xvalues[i+1] && xposition > xvalues[i] && xspeed < 1){
        if(yposition >= yvalues[(i)]-25 && yposition < yvalues[(i)]-15){
          safe = true;  // returns que to start landing protocol
        }
        else{safe = false;} // if not close enough give no landing indication
      }
    }
    return safe;
  }
  void ship::landship(){  // landing protocol
    xspeed = 0; // stop the ship
    yspeed = 0;
    yposition = maxy(xposition) - 20; // pull the ship down to the landing
                                      // 20 used to allow game to be more easily played
                                      // takes advatge of the range alled in check within
                                      // checkLand (15-25)
  }
  void ship::landStats(){
    int i;
    // iterator used to assign score based on what landing the user used
    int xvalues[] = {50,100, 275,300, 375,400, 525,550, 650,700, 700,850,  900,920};
    int scores[] = {100, 50, 200, 150, 100, 50, 200};
    for(i = 0; i < 14; i+=2){
      if(xposition > xvalues[i] && xposition < xvalues[i+1]){
        score = score + scores[i/2];
      }
    }
    // resests the position of the ship
    xposition = 50;
    yposition = 175;
    yspeed = 0.3;
    xspeed = 0.3;
  }
  void ship::shipbounds(){ // makes sure in bounds
    int count = 0;
    if(xposition > 999 || xposition < 0 || yposition < 0){
      // it goes off the screen bring it back to the start
      xposition = 50;
      yposition = 175;
      fuel = fuel - 10; // lightly punish this to Incentize staying on board
      yspeed = 0.3;
      xspeed = 0.3;
      while(count < 1000){
        gfx_text(100, 100, "You Left Safe Landing Area");
        // let user know why they were moved
        usleep(1000);
        count ++;
      }
    }
    }
    void ship::crashscreen(int c){
      char thefont[] = "-misc-fixed-bold-r-semicondensed--13-120-75-75-c-60-iso8859-1";
      gfx_changefont(thefont);
      gfx_text(400, 190, "You Crashed");
      gfx_text(400, 230, "Press c to continue");
      switch (c) {
        case 1:
          gfx_text(330, 215, "Fuel pipe was damaged by collision, you lost 200 fuel before it was patched");
          break;
        case 2:
          gfx_text(350, 215, "You have created a mile wide crator, - 200 Fuel");
          break;
        case 3:
          gfx_text(350, 215, "Hyperdrive adaptor has been cracked, - 200 Fuel");
          break;
        default:
          gfx_text(330, 215, "Damage to the Ship's Fuel Reserves Results in loss of 200 units of Fuel");
          break;
        }

      }
