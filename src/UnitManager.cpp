#include <thread>

#include <SDL2/SDL.h>

#include "UnitManager.h"
#include "Unit.h"



UnitManager::UnitManager()
{
   //ctor
}

UnitManager::~UnitManager()
{
   //dtor
}


Unit * UnitManager::addUnit(Unit * u)
{
   this->ships.push_back(u);
   return u;
}


void UnitManager::run(UnitManager * um)
{
   unsigned int i, j;
   unsigned int ticStart;
   unsigned int ticEnd;

   while (um->pause == false)
   {
      ticStart = SDL_GetTicks();
      j = um->ships.size();

      for (i=0; i<j; i++)
      {
         um->ships[i]->run();
      }

      ticEnd = SDL_GetTicks();
      um->workTime = ticEnd - ticStart;

      if (um->workTime >= 20)
         SDL_Delay(1);
      else
         SDL_Delay(20 - um->workTime);
   }
}

void UnitManager::manage()
{
   this->pause = false;

   std::thread t(UnitManager::run, this);
   t.detach();
}

void UnitManager::setPause()
{
   this->pause = true;
}
