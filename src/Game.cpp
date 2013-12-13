#include <unistd.h>
#include <string>
#include <sstream>
#include <iostream>

#include <cmath>
#include <thread>

#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Game.h"
#include "Config.h"
#include "Device.h"
#include "Exceptions.h"

#include "Unit.h"
#include "Squadron.h"
#include "Window.h"
#include "Path.h"
#include "Player.h"

#include <Eigen/Dense>
using namespace Eigen;
#define X 0
#define Y 1

using namespace std;

// http://wiki.libsdl.org/SDLKeycodeLookup
// 0x40000000 + 0x00000200 = 0x3FFFFE39
#define SDL_KEY_CONVERT(k) (k > 0x40000000 ? k - 0x3FFFFF39 : k)

#define KEY_QUIT   999

#define KEY_LSHIFT SDL_KEY_CONVERT(SDLK_LSHIFT)
#define KEY_RSHIFT SDL_KEY_CONVERT(SDLK_RSHIFT)
#define KEY_LCTRL  SDL_KEY_CONVERT(SDLK_LCTRL)
#define KEY_RCTRL  SDL_KEY_CONVERT(SDLK_RCTRL)
#define KEY_RSHIFT SDL_KEY_CONVERT(SDLK_RSHIFT)
#define KEY_ESCAPE SDL_KEY_CONVERT(SDLK_ESCAPE)
#define KEY_TAB    SDL_KEY_CONVERT(SDLK_TAB)
#define KEY_PAUSE  SDL_KEY_CONVERT(SDLK_p)

#define KEY_0      SDL_KEY_CONVERT(SDLK_0)
#define KEY_1      SDL_KEY_CONVERT(SDLK_1)
#define KEY_2      SDL_KEY_CONVERT(SDLK_2)
#define KEY_3      SDL_KEY_CONVERT(SDLK_3)
#define KEY_4      SDL_KEY_CONVERT(SDLK_4)
#define KEY_5      SDL_KEY_CONVERT(SDLK_5)
#define KEY_6      SDL_KEY_CONVERT(SDLK_6)
#define KEY_7      SDL_KEY_CONVERT(SDLK_7)
#define KEY_8      SDL_KEY_CONVERT(SDLK_8)
#define KEY_9      SDL_KEY_CONVERT(SDLK_9)



Game::Game()
{
   for (int i=0; i<250; i++)
   {
      this->v_displayTime.push_back(0);
      this->v_textTime.push_back(0);
      this->v_workTime.push_back(0);
   }
}

Game::~Game()
{
}

bool Game::init()
{
   //Start our window
   try
   {
      Window::init("SpaceLife");
   }
   catch (const runtime_error &e)
   {
      cout << e.what() << endl;
      Window::quit();
      return false;
   }

   try
   {
      //Load the image
      //string imgFile = "ship1.bmp";
      //this->texture_ship_1 = Window::loadImage(imgFile);
      //Load the font and message
      string fontFile = "font.dat";
      this->font_12 = Window::loadFont(fontFile, 12);

   }
   catch (const runtime_error &e)
   {
      //Catch error and crash
      cout << e.what() << endl;
      Window::quit();
      return false;
   }

   this->player = new Player();
   this->UM     = new UnitManager();

   int i;
   for (i=0; i<SDL_MAX_KEY; i++)
   {
      this->key[i] = 0;
   }
   for (i=0; i<SDL_MAX_MOUSE; i++)
   {
      this->mouse[i] = 0;
   }

   return true;

}

bool Game::quit()
{
   Window::quit();
   return true;
}

void Game::display()
{
   unsigned int ticStart = SDL_GetTicks();
   unsigned int i;

   //RENDERING
   Window::clear();

   // display selection
   if (this->mouseActive)
   {
      Window::drawShip(this->mousePosition, 0x121222, false);
   }

   for (auto &u : this->UM->ships)
   {
      // Window::draw(this->texture_ship_1, rect, NULL, 0);
      Window::drawShip(u->getRect(),
                       u->getColor(),
                       u->selected);
   }

   // draw selection path
   for (auto &u : this->player->selection)
   {
      for (auto &p : u->getPath()->nodes)
      {
         Window::drawPoint(p[X], p[Y]);
      }
   }

   this->displayTime = SDL_GetTicks() - ticStart;
   this->displayText();
   this->textTime = SDL_GetTicks() - ticStart - this->displayTime;

   /** TIME DEBUG **/
   this->v_displayTime.erase(this->v_displayTime.begin());
   this->v_textTime.erase(this->v_textTime.begin());
   this->v_workTime.erase(this->v_workTime.begin());

   this->v_displayTime.push_back(this->displayTime);
   this->v_textTime.push_back(this->textTime);
   this->v_workTime.push_back(this->UM->workTime);


   SDL_Rect r;
   for (i = 0; i < this->v_displayTime.size(); i++)
   {
      r.x = 5 + 3*i;
      r.y = 5;
      r.w = 2;
      r.h = this->v_workTime[i];
      if (this->v_workTime[i] < 15)
         Window::drawShip(r, 0x00FF00, false);
      else if (this->v_workTime[i] < 19)
         Window::drawShip(r, 0xFF9900, false);
      else
         Window::drawShip(r, 0xFF0000, false);
   }
   /** END TIME DEBUG **/

   // render
   Window::present();


}

void Game::displayText()
{
   SDL_Texture  *msg;
   SDL_Rect     pos;
   SDL_Color color = { 128, 128, 128 };
   ostringstream text("");

   text << "SpaceLife" << endl;
   text << "UM work time : " << this->UM->workTime << " ms" << endl;
   text << "Units : " << this->UM->ships.size() << endl;

   pos = {10, 10};
   msg = Window::renderText(text.str().c_str(), this->font_12, color);
   Window::draw(msg, pos, NULL);
   SDL_DestroyTexture(msg);
}

void Game::play()
{
   SDL_Event event;
   Unit * u;

   /***********************************************************/
   /** DEBUG : Generate units                                **/
   /***********************************************************/
   #define RANDPOS (100 + rand()%600)
   int i = 0;

   Squadron * s8 = new Squadron();
   int nbUnit = 200;
   for (int j = 0; j < nbUnit; j++)
   {
      u = this->UM->addUnit(new Unit(i, RANDPOS, RANDPOS, 0xFFFFFF / nbUnit * j));
      s8->addUnit(u);
      i++;
   }
   s8->setAction(UNIT_ACTION_NONE, nullptr);


   // PATH
   /*
   Path p1;
   p1.type = PATH_CYCLIC; // PATH_ONE_WAY // PATH_TWO_WAY
   p1.addNode(100, 100);
   p1.addNode(700, 100);
   p1.addNode(100, 700);
   p1.addNode(700, 700);
   Squadron * s8 = new Squadron();
   for (int j = 0; j < 10; j++)
   {
      this->ships.push_back(new Unit(i, RANDPOS, RANDPOS, 0xFF0000));
      s8->addUnit(this->ships[i]);
      i++;
   }
   s8->setPath(p1);
   s8->setAction(UNIT_ACTION_PATH, nullptr);
   */

   this->UM->manage();


   /***********************************************************/
   /**                                                       **/
   /***********************************************************/

   // purge queue events
   while(SDL_PollEvent(&event));

   this->endGame     = 0;
   this->pauseGame   = 0;
   this->mouseActive = 0;

   int tic = 0; // debug
   while (this->endGame == 0)
   {
      // get user action
      this->UIgetAction(event);

      // threat user action
      this->UIHandleAction();

      // display
      this->display();

      // wait
      SDL_Delay(20);

      tic++;
   }
}


