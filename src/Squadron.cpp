#include "Squadron.h"

Squadron::Squadron()
{
   this->maxAcceleration = 0;
   this->maxVelocity     = 0;
}

Squadron::~Squadron()
{
   //dtor
}

void Squadron::addUnit(Unit * u)
{
   this->squadron.push_back(u);
   u->setSquadron(this);

   //this->updateMaxAcceleration(u->getMaxAcceleration());
   //this->updateMaxVelocity(u->getMaxVelocity());
}

void Squadron::clear()
{
   for (auto &u : this->squadron)
   {
      u->setSquadron(nullptr);
   }
   this->squadron.clear();
}

void Squadron::setAction(int action, Unit * target)
{
   for (auto &u : this->squadron)
   {
      u->setAction(action, target);
   }
}

void Squadron::setPath(Path path)
{
   for (auto &u : this->squadron)
   {
      u->setPath(path);
   }
}

vector<Unit*> Squadron::getUnits()
{
   return this->squadron;
}

int Squadron::getUnitCount()
{
   return this->squadron.size();
}

void Squadron::select(bool selected)
{
   for (auto &u : this->squadron)
   {
      u->select(selected);
   }
}

void Squadron::updateMaxVelocity(double maxVelocity) {
	if (this->maxVelocity == 0
    || this->maxVelocity > maxVelocity)
   {
      this->maxVelocity = maxVelocity;
      this->updateUnitsCurrentVelocity(this->maxVelocity);
   }
}
void Squadron::updateMaxAcceleration(double maxAcceleration) {
	if (this->maxAcceleration == 0
    || this->maxAcceleration > maxAcceleration)
   {
      this->maxAcceleration = maxAcceleration;
      this->updateUnitsCurrentAcceleration(this->maxAcceleration);
   }
}

void Squadron::updateUnitsCurrentVelocity(double velocity)
{
   for (auto &u : this->squadron)
   {
      u->setCurrentVelocity(velocity);
   }
}
void Squadron::updateUnitsCurrentAcceleration(double acceleration)
{
   for (auto &u : this->squadron)
   {
      u->setCurrentAcceleration(acceleration);
   }
}
