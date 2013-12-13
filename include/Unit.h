#ifndef UNIT_H
#define UNIT_H

#include <SDL2/SDL.h>

#include "Squadron.h"
#include "Path.h"

#include <Eigen/Dense>
// http://eigen.tuxfamily.org/dox/AsciiQuickReference.txt
using namespace Eigen;

#define BASE_VELOCITY         3.0
#define BASE_FORCE            0.6
#define BASE_MASS             20.0
#define SLOWDOWN_RADIUS       200.0
#define LEADER_SIGHT_RADIUS   20.0
#define LEADER_BEHIND_DIST    10.0
#define SEPARATION_RADIUS     15.0
// SEPARATION_RADIUS2 = SEPARATION_RADIUS ^ 2
#define SEPARATION_RADIUS2    225.0

#define UNIT_ACTION_NONE      0
#define UNIT_ACTION_SEEK      1
#define UNIT_ACTION_FLEE      2
#define UNIT_ACTION_PURSUIT   3
#define UNIT_ACTION_ORBIT     4
#define UNIT_ACTION_FOLLOW    5
#define UNIT_ACTION_PATH      6

class Squadron;
class Unit
{
   public:
      // http://eigen.tuxfamily.org/dox-devel/group__TopicWrongStackAlignment.html
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW

      Unit(int    id,
           double x     = 0,
           double y     = 0,
           int    color = 0xFF0000,
           double orbit = 0);
      virtual ~Unit();

      int         id;

      /* physics */
      Vector2d    position;
      Vector2d    velocity;
		Vector2d    target;
		Vector2d    desired;
		Vector2d    steering;

		/* characteristics */
		double      mass;
		double      maxVelocity;
		double      maxAcceleration;
      double      currentVelocity;
		double      currentAcceleration;
		double      slowdownRadius;

      double getMaxVelocity();
      void setMaxVelocity(double maxVelocity);
      double getMaxAcceleration();
      void setMaxAcceleration(double maxAcceleration);
      double getCurrentVelocity();
      void setCurrentVelocity(double currentVelocity);
      double getCurrentAcceleration();
      void setCurrentAcceleration(double currentAcceleration);





		bool        flag;
		int         tic;
      double      distance;


      /* orders */
		Unit *      unitTarget;
		int         action;
		Vector2d    curPosition;

		Path        path;

      /*  */
		SDL_Rect    rect;
		double      orbitDistance;
		int         color;
		Squadron *  squadron;
		double      time;
		bool        selected;


		void setPosition(double x, double y);

		Vector2d truncate(const Vector2d& v, double max);


      void setAction(int action, Unit * target);
      void setSquadron(Squadron * squadron);

      void setPath(Path path);
      Path * getPath();


      SDL_Rect&   getRect();
      SDL_Rect*   getpRect();
      double      getOrbit();
      void        setOrbit(double orbit);
      int         getColor();
      void        setColor(int color);

      void        select(bool selected = true);

		Vector2d seek(const Vector2d& target, double slowingRadius = 0);
		Vector2d flee(const Vector2d& target);
		Vector2d separation();
		Vector2d pursuit(const Vector2d& targetPosition,
                       const Vector2d& targetVelocity);
      Vector2d evade(const Vector2d& targetPosition,
                     const Vector2d& targetVelocity);
		Vector2d orbit(const Vector2d& target,
                     double distance);
      Vector2d followLeader(const Vector2d& leaderPosition,
                            const Vector2d& leaderVelocity);
      Vector2d pathFollowing();

		void updatePosition();

		bool alive;
		void run();

   protected:
   private:
};

std::ostream& operator <<(std::ostream&, const Unit&);


#endif // UNIT_H
