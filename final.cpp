//Final Project, Moon Lander, Created by Connor and Ryan Green
#include <unistd.h>
#include <cstdlib>
#include <time.h>
#include <cmath>
#include "gfxnew.h"
#include "ship.h"
#include <string>

//Prototypes
void startscreen();
void background();
void displaystats(int, int, int, int);
void landscreen();

int main(){
  //Initialize Variables
  int win_wd = 1000, win_ht = 800, fuel, x, event ;
  char c;
  double radius = 7*M_PI/4, angle = 0, xspeed, yspeed, base;
  bool flame = false;
  ship mland;
  srand(time(NULL));

  gfx_open (win_wd, win_ht, "Moon Lander");
  gfx_clear();
  while(gfx_event_waiting() != 1 && c != 'q' && c != 's'){
    c = gfx_wait();
    startscreen();
  }
  gfx_flush();
  while( c!= 'q'){
    mland.newposition();
    background();
    displaystats(mland.getscore(), mland.getfuel(), 40*mland.getxspeed(), 40*mland.getyspeed() );
    mland.drawship(radius,flame);
    //Check to See if they are still on the Screen
    mland.shipbounds();
    //Find Orientation of Ship to See if Land is possible
    if (mland.checkLand()){
      base = mland.gyposition() - (mland.gyposition()-20*sin(radius));
      if(base > 13 && base < 15){
        while((event != gfx_event_waiting())){
          c = gfx_wait();
          while( c != 'q' && c != 'c'){
            landscreen();
            mland.landship();
            radius = -(7*M_PI/4);
            background();
            displaystats(mland.getscore(), mland.getfuel(), 40*mland.getxspeed(), 40*mland.getyspeed() );
          }
        }
        mland.landStats();
        radius = 7*M_PI/4;
      }
    }
    //Check To see if there is a crash
    if(!mland.checkcrash(radius)){
      if(mland.getfuel() <= 0){break;}
      //If user presses 'c', resets stats and lower fuel
      x = rand()%3+1;
      while(!gfx_event_waiting()){
        mland.explosion(radius);
        mland.crashscreen(x);
        radius = 7*M_PI/4;
      }
      while((event = gfx_event_waiting())){
        c = gfx_wait();
        if(event == 1 && (c == 'q' || c == 'c')){
          mland.crashstats();
        }
      }
    }
    //Check to see if fuel is still remaining
    while(mland.getfuel() <= 0){
      mland.endgame();
    }
    gfx_flush();
    usleep(20000);
    flame = false;
    angle = 0;
    while((event = gfx_event_waiting())){
      c = gfx_wait();
      if(event == 1){
        switch (c) {
          case 'a':
          case 'Q':
            angle = angle - 0.1;
            break;
          case 'd':
          case 'S':
            angle = angle + 0.1;
            break;
          case 'w':
          case 'R':
            flame = true;
            mland.newspeed(radius);
            break;
          case 'q':
            break;
         }
         radius = radius + angle;
      }
    }

}
}

void startscreen(){
  //Create A Screen with the Title and a message that says press 's' t begin
  //Moon Lander Title
  char thefont[] = "-sony-fixed-medium-r-normal--24-170-100-100-c-120-iso8859-1";
  char thefont2[] = "-misc-fixed-medium-r-normal-ja-13-120-75-75-c-120-iso10646-1";
  char mytext[] = "Moon Lander";
  int x_winctr, y_winctr;            // window's center
  int pxl_w, pxl_h, x_txt, y_txt;    // text string's parameters
  int x_r;   // rectangle's parameters
  int pad = 10;                      // padding for the rectangle
  gfx_changefont(thefont);
  // find the window's center point coordinates
  x_winctr = 1000/2;
  y_winctr = 800/2;
  // determine the text string's width and height in pixels
  pxl_w = gfx_textpixelwidth(mytext, thefont);
  pxl_h = gfx_textpixelheight(mytext, thefont);
  // display the text, dead center, by first determining its lower left corner
  x_txt = x_winctr - pxl_w/2;
  y_txt = y_winctr + pxl_h/2 - 100;
  gfx_text(x_txt, y_txt, mytext);
  // draw a rectangle around the text, dead center, with a given padding
  // first determine its top left corner's x,y coordinates
  x_r = x_txt - pad;
  // now draw the rectangle with the above four values
  gfx_rectangle(x_r, y_txt - pxl_h - pad, pxl_w + 2*pad, pxl_h + 2*pad);
  //Display Instructions
  gfx_changefont(thefont2);
  gfx_text(x_r - 30, 440, "Press 's' to Start");
  gfx_text(x_r - 150, 460, "Press 'q' at any time to quit the game");
  gfx_text(x_r - 120, 480, "Rotate the ship using 'a' and 'd'");
  gfx_text(x_r - 130, 500, "Press 'w' to turn on the thrusters");
}
void background(){
  int i;
  char thefont[] = "-misc-fixed-bold-r-semicondensed--13-120-75-75-c-60-iso8859-1";
  int xvalues[] = {0, 50, 100, 125, 130, 135, 140, 145, 150, 275, 300, 350, 375, 375, 400, 450, 525, 550, 600, 610, 620, 650, 700, 750, 850, 875, 900, 920, 980, 1000};
  int yvalues[] = {400, 500, 500, 650, 660, 675, 680, 675, 650, 600, 600, 500, 450, 400, 400, 300, 475, 475, 550, 590, 600, 625, 625, 700, 700, 600, 450, 450, 440, 350};
  //Creae the Surface
  for(i = 1; i <= 29; i++){
    gfx_line(xvalues[i-1], yvalues[i-1], xvalues[i], yvalues[i]);
  }
  //Display Incentives
  gfx_changefont(thefont);
  gfx_text(70, 515, "2X");
  gfx_text(383, 415, "4X");
  gfx_text(535, 490, "3X");
  gfx_text(670, 640, "2X");
  gfx_text(908, 465, "4X");
}
void displaystats(int score,int fuel,int xspeed,int yspeed){
  string sscore = to_string(score);
  char const *cscore = sscore.c_str();
  string sfuel = to_string(fuel);
  char const *cfuel = sfuel.c_str();
  string sxspeed = to_string(xspeed);
  char const *cxspeed = sxspeed.c_str();
  string syspeed = to_string(yspeed);
  char const *cyspeed = syspeed.c_str();
  //use smaller font for stats
  char thefont[] = "-misc-fixed-medium-r-semicondensed--12-110-75-75-c-60-iso8859-1";
  gfx_changefont(thefont);
  //Display Text
  gfx_text(50, 50, "Score: ");
  gfx_text(50, 75, "Fuel: ");
  gfx_text(900, 50, "X Speed: ");
  gfx_text(900, 75, "Y Speed: ");
  //Display Values
  gfx_text(95, 50, cscore );
  gfx_text(95, 75, cfuel );
  gfx_text(955, 50, cxspeed );
  gfx_text(955, 75, cyspeed );
}
void landscreen(){
  char thefont[] = "-misc-fixed-bold-r-semicondensed--13-120-75-75-c-60-iso8859-1";
  gfx_changefont(thefont);
  gfx_text(400, 190, "You Landed");
  gfx_text(360, 215, "Your Score has been updated");
  gfx_text(400, 230, "Press c to continue");
}
