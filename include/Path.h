#ifndef PATH_H
#define PATH_H

#include <vector>
using std::vector;

#include <Eigen/Dense>
#include<Eigen/StdVector> // allow use of vector<Vector2d>
using namespace Eigen;

#define PATH_ONE_WAY  0
#define PATH_TWO_WAY  1
#define PATH_CYCLIC   2


class Path
{
   public:
      // http://eigen.tuxfamily.org/dox-devel/group__TopicWrongStackAlignment.html
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW

      Path();
      virtual ~Path();

      // http://eigen.tuxfamily.org/dox-devel/group__TopicStlContainers.html
      vector<Vector2d, Eigen::aligned_allocator<Eigen::Vector2d> > nodes;
      double  radius;
      int     type;
		unsigned int currentNode;
		int     currentDirection;

		int getSize();
		Vector2d getCurrentNode();



      void addNode(double x, double y);

      void resetLastNode();   // set one node path (last one)
      void updateNode();      // goto next node
      void setLastNode();     // goto last node
      void setFirstNode();    // goto first node
      void changeDirection(); // change direction

      bool isLastNode();
      bool isFirstNode();




   protected:


   private:


};

#endif // PATH_H
