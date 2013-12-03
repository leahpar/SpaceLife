#ifndef UNITMANAGER_H
#define UNITMANAGER_H

#include "Unit.h"

class UnitManager
{
   public:
      UnitManager();
      virtual ~UnitManager();

      vector<Unit*> ships;

      Unit * addUnit(Unit * u);

      void   setPause();
      void   manage();
      static void run(UnitManager * um);

      unsigned int workTime;


   protected:

   private:

      bool pause;
};

#endif // UNITMANAGER_H
