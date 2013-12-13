#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
using std::vector;

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
/*#include <SDL2/SDL_mixer.h>*/

#include "Unit.h"
#include "Player.h"
#include "UnitManager.h"

#define SDL_MAX_KEY 1000
#define SDL_MAX_MOUSE 5

using namespace std;

class Game
{
   public:
      Game();
      ~Game();

      bool init();
      bool quit();

      void play();


      /* tests */
      vector<Unit*> ships;


      Player * player;
      UnitManager * UM;

      /** UI **/
      char key[SDL_MAX_KEY];
      int  mouse[SDL_MAX_MOUSE];
      int  mouseActive;
      SDL_Rect mousePosition;
      int  mouseX;
      int  mouseY;



      /****** debug ******/
      unsigned int displayTime;
      unsigned int textTime;
      unsigned int workTime;
      vector<int>  v_displayTime;
      vector<int>  v_textTime;
      vector<int>  v_workTime;

   protected:
   private:

      /* Game */
      int endGame;
      int pauseGame;

      /* UI functions */
      void UIgetAction(SDL_Event event);
      void UIHandleAction();

      void UIEscape();
      void UIPause(bool pause);
      void UIQuit();

      void UIGetGroup();
      void UISetGroup();

      void UILeftMouseDown();
      void UILeftMouseHold();
      void UILeftMouseUp();
      void UIRightMouseDown();

      /* textures */
      SDL_Texture*   texShip;
      SDL_Texture*   texConsole;

      void display();
      void displayText();


      /* RESSOURCES */
      SDL_Texture* texture_ship_1;
      SDL_Texture* texture_font_12;
      TTF_Font*    font_12;

      /* COLOR */
};

#endif // GAME_H
