#include "Path.h"

Path::Path()
{
   //ctor
   this->radius = 50.0;
   this->type = PATH_ONE_WAY;

   this->currentNode = 0;
   this->currentDirection = 1;
}

Path::~Path()
{
   //dtor
}


void Path::addNode(double x, double y)
{
   Vector2d node(x, y);
   this->nodes.push_back(node);
}

int Path::getSize()
{
   return this->nodes.size();
}

Vector2d Path::getCurrentNode()
{
   return this->nodes[this->currentNode];
}

void Path::updateNode()
{
   // goto next node
   this->currentNode += this->currentDirection;

   // case last node
   if (this->isLastNode())
   {
      switch (this->type)
      {
         case PATH_ONE_WAY:
            this->setLastNode();
            break;
         case PATH_TWO_WAY:
            this->setLastNode();
            this->changeDirection();
            break;
         case PATH_CYCLIC:
            this->setFirstNode();
            break;
      }
   }
   // case first node
   else if (this->isFirstNode())
   {
      switch (this->type)
      {
         case PATH_TWO_WAY:
            this->setFirstNode();
            this->changeDirection();
            break;
      }
   }
}

void Path::setLastNode()
{
   this->currentNode = this->nodes.size() - 1;
}

void Path::setFirstNode()
{
   this->currentNode = 0;
}

void Path::resetLastNode()
{
   this->nodes.erase(this->nodes.begin(),
                     this->nodes.begin() + this->nodes.size() - 1);
   this->setFirstNode();
}

void Path::changeDirection()
{
   this->currentDirection *= -1;
}

bool Path::isLastNode()
{
   return (this->currentNode >= this->nodes.size());
}
bool Path::isFirstNode()
{
   return (this->currentNode <= 0);
}





