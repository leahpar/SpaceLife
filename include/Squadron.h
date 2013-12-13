#ifndef SQUADRON_H
#define SQUADRON_H

#include <vector>
using std::vector;


#include "Unit.h"
#include "Path.h"


class Unit;
class Squadron
{
   public:
      Squadron();
      virtual ~Squadron();

      vector<Unit*> squadron;

      void addUnit(Unit * u);
      void clear();
      vector<Unit*> getUnits();
      int getUnitCount();

      void setAction(int action, Unit * target);
      void setPath(Path path);

      void updateMaxVelocity(double maxVelocity);
      void updateMaxAcceleration(double maxAcceleration);

      void select(bool selected = true);

   protected:

   private:

      double maxVelocity;
		double maxAcceleration;

      void updateUnitsCurrentVelocity(double velocity);
      void updateUnitsCurrentAcceleration(double acceleration);


};

#endif // SQUADRON_H
