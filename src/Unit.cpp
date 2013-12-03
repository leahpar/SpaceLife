#include <cstdlib>
#include <iomanip>

#include "Unit.h"

#include <SDL2/SDL.h>

#include <Eigen/Dense>
using namespace Eigen;

#include <vector>
using std::vector;

#define X 0
#define Y 1



Unit::Unit(int id, double x, double y, int color, double orbit)
{
   //ctor
   this->id = id;
   this->position << x, y;
   this->curPosition << x, y;
   this->velocity << 0.01, 0.01;
	this->target   << 0, 0;
	this->desired  << 0, 0;
	this->steering << 0.01, 0.01;
	this->alive = false;
	this->tic   = 0;

	this->action = UNIT_ACTION_NONE;
	this->rect.x = x;
	this->rect.y = y;
	this->rect.h = 5;
	this->rect.w = 5;

	this->orbitDistance = orbit;
   this->color = color;

   this->maxVelocity     = BASE_VELOCITY * (1.5 - (double)rand()/RAND_MAX);
   this->maxAcceleration = BASE_FORCE    * (1.5 - (double)rand()/RAND_MAX);
   this->mass            = BASE_MASS     * (1.5 - (double)rand()/RAND_MAX);

   this->currentVelocity     = this->maxVelocity;
   this->currentAcceleration = this->maxAcceleration;

   this->slowdownRadius = this->currentVelocity
                        * this->currentVelocity
                        / this->currentAcceleration
                        * 20.0;

   this->squadron = nullptr;

   this->selected = false;
}

Unit::~Unit()
{
   //dtor
}


void Unit::setPosition(double x, double y)
{
   this->position[X] = x;
   this->position[Y] = y;
   this->rect.x = x;
	this->rect.y = y;
}

Vector2d Unit::truncate(const Vector2d& v, double max)
{
   double i;
   i = max / v.norm();
   i = i < 1.0 ? i : 1.0;
   return i * v;
}

Vector2d Unit::seek(const Vector2d& target, double slowingRadius)
{
   double distance;
   this->desired = target - this->position;
	distance = this->desired.norm();
	this->desired.normalize();

   if (distance <= slowingRadius)
   {
      this->desired *= (this->currentVelocity * distance / slowingRadius);
   }
   else
   {
      this->desired *= (this->currentVelocity);
   }

	return this->desired - this->velocity;
}

Vector2d Unit::orbit(const Vector2d& target, double orbit)
{
   double distance;

   this->desired = target - this->position;
	distance = this->desired.norm();
	this->desired.normalize();

   if (distance > orbit * 2.0)
   {
      return seek(target, this->slowdownRadius);
   }
   else if (distance > orbit)
   {
      this->desired *= this->currentVelocity * (distance - orbit)/orbit;
      this->desired += this->desired.unitOrthogonal() * this->currentVelocity * 0.5;

      return this->desired - this->velocity;
   }
   else if (distance > orbit * 0.5)
   {
      this->desired *= this->currentVelocity * (distance - orbit)/orbit;
      this->desired -= this->desired.unitOrthogonal() * this->currentVelocity * 0.5;

      return this->desired - this->velocity;
   }
   else
   {
      this->desired *= -this->currentVelocity;
      this->desired -= this->desired.unitOrthogonal() * this->currentVelocity * 0.5;

      return this->desired - this->velocity;
   }
}

Vector2d Unit::flee(const Vector2d& target)
{
   this->desired = target - this->position;
	this->desired.normalize();

      this->desired *= (-this->currentVelocity);

	return this->desired - this->velocity;
}

Vector2d Unit::evade(const Vector2d& targetPosition,
                     const Vector2d& targetVelocity)
{
   double updatesNeeded;

   this->desired = targetPosition - this->position;
   this->distance = this->desired.norm();

   updatesNeeded = this->distance / this->currentVelocity;

   return flee(targetPosition + (targetVelocity * updatesNeeded));
}

Vector2d Unit::pursuit(const Vector2d& targetPosition,
                       const Vector2d& targetVelocity)
{
   double updatesNeeded;

   this->desired = targetPosition - this->position;
   this->distance = this->desired.norm();

   updatesNeeded = this->distance / this->currentVelocity;


   return seek(targetPosition + (targetVelocity * updatesNeeded),
               this->slowdownRadius);
}

Vector2d Unit::followLeader(const Vector2d& leaderPosition,
                            const Vector2d& leaderVelocity)
{
   Vector2d tv = leaderVelocity;
   Vector2d forcefollow(0, 0);

   tv.normalize();
   tv *= LEADER_BEHIND_DIST;

   //if (isOnLeaderSight())
   if ((leaderPosition + tv - this->position).norm() <= LEADER_SIGHT_RADIUS
    || (leaderPosition      - this->position).norm() <= LEADER_SIGHT_RADIUS)
   {
      forcefollow = evade(leaderPosition, leaderVelocity);
      forcefollow.normalize();
   }


   forcefollow += pursuit(leaderPosition - tv, leaderVelocity);

   return forcefollow;
}

Vector2d Unit::pathFollowing()
{
   Vector2d target(0, 0);
   if (this->path.getSize() > 0)
   {
      target = this->path.getCurrentNode();

      if ((target - this->position).norm() <= this->path.radius)
      {
         this->path.updateNode();
      }
      return seek(target, this->slowdownRadius);
   }
   else
   {
      return target; // null steering
   }
}

Vector2d Unit::separation()
{
   // http://gamedev.tutsplus.com/tutorials/implementation/the-three-simple-rules-of-flocking-behaviors-alignment-cohesion-and-separation/
   Vector2d forceSeparation(0, 0);
   Vector2d t;
   int shipCount = 0;

   for (auto &u : this->squadron->getUnits())
   {
      t = this->position - u->position;
      if  ((this->id != u->id)
       && (t.squaredNorm() <= SEPARATION_RADIUS2))
      {
         t.normalize();
         forceSeparation += t;
         shipCount++;
      }
   }

   if (shipCount > 0)
   {
      forceSeparation /= shipCount;
   }

   return forceSeparation;
}

void Unit::updatePosition()
{
   switch (this->action)
   {
      case UNIT_ACTION_NONE:
         this->steering = seek(this->curPosition,
                               this->slowdownRadius);
         break;
      case UNIT_ACTION_SEEK:
         this->steering = seek(this->unitTarget->position,
                               this->slowdownRadius);
         break;
      case UNIT_ACTION_FLEE:
         this->steering = flee(this->unitTarget->position);
         break;
      case UNIT_ACTION_ORBIT:
         this->steering = orbit(this->unitTarget->position,
                                this->unitTarget->getOrbit());
         break;
      case UNIT_ACTION_PURSUIT:
         this->steering = pursuit(this->unitTarget->position,
                                  this->unitTarget->velocity);
         break;
      case UNIT_ACTION_FOLLOW:
         this->steering = followLeader(this->unitTarget->position,
                                       this->unitTarget->velocity);
      case UNIT_ACTION_PATH:
         this->steering = pathFollowing();
         break;
   }

   // squadron separation
   if (this->squadron != nullptr)
      this->steering += separation();

   this->steering  = truncate(this->steering, this->currentAcceleration);
   this->steering /= this->mass;

   this->velocity  = truncate(this->velocity + this->steering, this->currentVelocity);

   this->position += this->velocity;
}

void Unit::setAction(int action, Unit * target)
{
   this->action     = action;
   this->unitTarget = target;
   if (action == UNIT_ACTION_NONE)
   {
      this->curPosition = this->position + this->velocity;
   }
}

void Unit::setPath(Path path)
{
   this->path = path;
}
Path * Unit::getPath()
{
   return &(this->path);
}



void Unit::setSquadron(Squadron * squadron)
{
   this->squadron = squadron;
}

void Unit::select(bool selected)
{
   this->selected = selected;
}

/************* Carac accessors ******/
SDL_Rect& Unit::getRect()
{
   this->rect.x = this->position(X);
   this->rect.y = this->position(Y);
   return this->rect;
}
SDL_Rect * Unit::getpRect()
{
   this->rect.x = this->position(X);
   this->rect.y = this->position(Y);
   return &(this->rect);
}
double Unit::getOrbit()
{
   return this->orbitDistance;
}
void Unit::setOrbit(double orbit)
{
   this->orbitDistance = orbit;
}
int Unit::getColor()
{
   return this->color;
}
void Unit::setColor(int color)
{
   this->color = color;
}
double Unit::getMaxVelocity()
{
	return this->maxVelocity;
}
void Unit::setMaxVelocity(double maxVelocity)
{
	this->maxVelocity = maxVelocity;
}
double Unit::getMaxAcceleration()
{
	return this->maxAcceleration;
}
void Unit::setMaxAcceleration(double maxAcceleration)
{
	this->maxAcceleration = maxAcceleration;
}
double Unit::getCurrentVelocity()
{
	return this->currentVelocity;
}
void Unit::setCurrentVelocity(double currentVelocity)
{
	this->currentVelocity = currentVelocity;
}
double Unit::getCurrentAcceleration()
{
	return this->currentAcceleration;
}
void Unit::setCurrentAcceleration(double currentAcceleration)
{
	this->currentAcceleration = currentAcceleration;
}


/************* TO STRING *************/

std::ostream& operator <<(std::ostream& Stream, const Unit& Obj)
{
    Stream << std::setprecision(3)
           << "ID " << Obj.id << " - "
           << "tic " << Obj.tic << " ; time " << Obj.time << " => " << Obj.time / Obj.tic;
    return Stream;
}

/********** thread things **************/

void Unit::run()
{
   unsigned int ticStart;

   ticStart = SDL_GetTicks();

   this->updatePosition();

   this->tic++;
   this->time += SDL_GetTicks() - ticStart;
}

