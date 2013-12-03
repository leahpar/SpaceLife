#include "Player.h"

#include "Unit.h"
#include "Squadron.h"
#include "Path.h"

Player::Player()
{
   //ctor
}

Player::~Player()
{
   //dtor
}

void Player::deselect()
{
   for (auto &u : this->selection)
   {
      u->select(false);
   }
   this->selection.clear();
}

void Player::select(Unit * unit, bool addMode)
{
   if (addMode == false)
   {
      this->deselect();
   }

   unit->select(true);
   this->selection.push_back(unit);
}

void Player::select(Squadron * squadron, bool addMode)
{
   if (addMode == false)
   {
      this->deselect();
   }

   squadron->select(true);
   for (auto &u : squadron->getUnits())
   {
      this->selection.push_back(u);
   }
}

void Player::groupSelection(int group)
{
   this->groups[group].clear();

   for (auto &u : this->selection)
   {
      this->groups[group].addUnit(u);
      u->setSquadron(&(this->groups[group]));
   }
}

void Player::selectGroup(int group, bool addMode)
{
   this->select(&(this->groups[group]), addMode);
}

/**
 * add node if path exists
 * set path if not exists
**/
void Player::addPathNode(double x, double y)
{
   for (auto &u : this->selection)
   {
      u->getPath()->addNode(x, y);
      u->setAction(UNIT_ACTION_PATH, nullptr);
   }
}

void Player::setPathNode(double x, double y)
{
   for (auto &u : this->selection)
   {
      u->getPath()->addNode(x, y);
      u->getPath()->resetLastNode();
      u->setAction(UNIT_ACTION_PATH, nullptr);
   }
}
