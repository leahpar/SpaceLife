
#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Game.h"
#include "Config.h"
#include "Device.h"

#include "Player.h"

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


/**
 * Quit game
 * set flag endGame
 */
void Game::UIQuit()
{
   this->endGame = 1;
}

/**
 * Pause game
 * set flag pauseGame and stop unit manager
 */
void Game::UIPause(bool pause)
{
   if (pause)
   {
      this->key[KEY_PAUSE] = 0;
      this->pauseGame = 1;
      this->UM->setPause();
   }
   else
   {
      this->key[KEY_PAUSE] = 0;
      this->pauseGame = 0;
      this->UM->manage();
   }
}

/**
 * Escape key action
 * Deselect player's selection
 */
void Game::UIEscape()
{
   this->player->deselect();
}

/**
 * Right button clic
 * Define action of the selected unit(s)
 */
void Game::UIRightMouseDown()
{
   this->mouse[SDL_BUTTON_RIGHT] = 0;

   if (this->key[KEY_LSHIFT]
    || this->key[KEY_RSHIFT])
   {
      this->player->addPathNode(this->mouseX, this->mouseY);
   }
   else
   {
      this->player->setPathNode(this->mouseX, this->mouseY);
   }
}

/**
 * Left button clic
 * Initialise selection
 */
void Game::UILeftMouseDown()
{
   this->mousePosition.x = this->mouseX;
   this->mousePosition.y = this->mouseY;
   this->mousePosition.w = 0;
   this->mousePosition.h = 0;
   this->mouseActive = 1;
}

/**
 * Left button hold
 * Selection in progress
 */
void Game::UILeftMouseHold()
{
   this->mousePosition.w = this->mouseX - this->mousePosition.x;
   this->mousePosition.h = this->mouseY - this->mousePosition.y;
}

/**
 * Left button released
 * Set selection
 */
void Game::UILeftMouseUp()
{
   // "negative" selection
   if (this->mousePosition.w < 0)
   {
      this->mousePosition.x += this->mousePosition.w;
      this->mousePosition.w *= -1;
   }
   if (this->mousePosition.h < 0)
   {
      this->mousePosition.y += this->mousePosition.h;
      this->mousePosition.h *= -1;
   }

   // deselect current selection if no SHIFT key pressed
   if (!this->key[KEY_LSHIFT] && !this->key[KEY_RSHIFT])
   {
      this->player->deselect();
   }

   // Single unit selection case
   if (this->mousePosition.w == 0
    && this->mousePosition.h == 0)
   {
      this->mousePosition.w = 1;
      this->mousePosition.h = 1;
   }

   for (auto &u : this->UM->ships)
   {
      if (SDL_HasIntersection(&(this->mousePosition), u->getpRect()))
      {
         this->player->select(u, true);
      }
   }

   this->mouseActive = 0;
}

/**
 * Ctrl key pressed
 * Assign current selection to a group
 */
void Game::UISetGroup()
{
   if (this->key[KEY_0])
      this->player->groupSelection(0);
   else if (this->key[KEY_1])
      this->player->groupSelection(1);
   else if (this->key[KEY_2])
      this->player->groupSelection(2);
   else if (this->key[KEY_3])
      this->player->groupSelection(3);
   else if (this->key[KEY_4])
      this->player->groupSelection(4);
   else if (this->key[KEY_5])
      this->player->groupSelection(5);
   else if (this->key[KEY_6])
      this->player->groupSelection(6);
   else if (this->key[KEY_7])
      this->player->groupSelection(7);
   else if (this->key[KEY_8])
      this->player->groupSelection(8);
   else if (this->key[KEY_9])
      this->player->groupSelection(9);
}

/**
 * Ctrl key not pressed
 * Call a group to the selection
 */
void Game::UIGetGroup()
{
   bool flag = this->key[KEY_LSHIFT] || this->key[KEY_RSHIFT];
   if (this->key[KEY_0])
      this->player->selectGroup(0, flag);
   else if (this->key[KEY_1])
      this->player->selectGroup(1, flag);
   else if (this->key[KEY_2])
      this->player->selectGroup(2, flag);
   else if (this->key[KEY_3])
      this->player->selectGroup(3, flag);
   else if (this->key[KEY_4])
      this->player->selectGroup(4, flag);
   else if (this->key[KEY_5])
      this->player->selectGroup(5, flag);
   else if (this->key[KEY_6])
      this->player->selectGroup(6, flag);
   else if (this->key[KEY_7])
      this->player->selectGroup(7, flag);
   else if (this->key[KEY_8])
      this->player->selectGroup(8, flag);
   else if (this->key[KEY_9])
      this->player->selectGroup(9, flag);
}

/**
 * Handle the current player's action
 */
void Game::UIHandleAction()
{
   // quit game
   if (this->key[KEY_QUIT])
   {
      this->UIQuit();
   }

   // pause game
   else if (this->key[KEY_PAUSE] && this->pauseGame == 0)
   {
      this->UIPause(true);
   }

   // unpause game
   else if (this->key[KEY_PAUSE]  && this->pauseGame == 1)
   {
      this->UIPause(false);
   }

   // target action
   else if (this->mouse[SDL_BUTTON_RIGHT])
   {
      this->UIRightMouseDown();
   }

   // mouse selection
   else if (this->mouse[SDL_BUTTON_LEFT] && this->mouseActive == 0)
   {
      this->UILeftMouseDown();
   }
   else if (this->mouse[SDL_BUTTON_LEFT])
   {
      this->UILeftMouseHold();
   }
   else if (! this->mouse[SDL_BUTTON_LEFT] && this->mouseActive == 1)
   {
      this->UILeftMouseUp();
   }

   // deselection
   else if (this->key[KEY_ESCAPE])
   {
      this->UIEscape();
   }

   // Group definition
   else if (this->key[KEY_LCTRL])
   {
      this->UISetGroup();
   }

   // group selection
   else if (! this->key[KEY_LCTRL])
   {
      this->UIGetGroup();
   }
}

/**
 * Get the current player's action
 */
void Game::UIgetAction(SDL_Event event)
{
   while (SDL_PollEvent(&event))
   {
      // ignore repeted event
      if (event.key.repeat == 1) continue;

      switch(event.type)
      {
         case SDL_QUIT:
            this->key[KEY_QUIT] = 1;
            break;
         case SDL_KEYDOWN:
            this->key[SDL_KEY_CONVERT(event.key.keysym.sym)] = 1;
            break;
         case SDL_KEYUP:
            this->key[SDL_KEY_CONVERT(event.key.keysym.sym)] = 0;
            break;
         case SDL_MOUSEBUTTONDOWN:
            this->mouse[event.button.button] = 1;
            break;
         case SDL_MOUSEBUTTONUP:
            this->mouse[event.button.button] = 0;
            break;
         default:
            break;
     }

     this->mouseX = event.button.x;
     this->mouseY = event.button.y;
   }
}
