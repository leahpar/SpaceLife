#include <iostream>
#include <time.h>

#include "Device.h"
#include "Game.h"
#include "Config.h"
#include "Exceptions.h"

#include <SDL2/SDL.h>

using namespace std;

#ifdef __MINGW32__
#undef main
#endif

int main(int argc, char **argv)
{
   Game * _game;

   // init rand
   srand(time(NULL));

   try
   {
      _game = new Game();
      _game->init();
      _game->play();
   }
   catch (int e)
   {
      // MessageBox(NULL, "KO", NULL, 0);
      return e;
   }

   // MessageBox(NULL, "OK", NULL, 0);

   _game->quit();
   delete _game;
   return 0;
}
