#ifndef PLAYER_H
#define PLAYER_H

#include "Unit.h"
#include "Squadron.h"

class Player
{
   public:
      Player();
      virtual ~Player();

      vector<Unit*> selection;
      Squadron groups[10];

      void deselect();
      void select(Unit * unit, bool addMode = false);
      void select(Squadron * squadron, bool addMode = false);

      void groupSelection(int group);
      void selectGroup(int group, bool addMode = false);


      void setPathNode(double x, double y);
      void addPathNode(double x, double y);



   protected:


   private:
};

#endif // PLAYER_H
