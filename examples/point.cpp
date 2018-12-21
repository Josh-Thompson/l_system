//Demonstration of using a custom type for the L-System's representation
//An actual L-System with a point representation would likely be parametarized to be useful
//This solely serves to show what is required for enabling custom representation types

#include <iostream>
#include <cassert>
#include <stdlib.h>
#include <ostream>
#include "l_system/l_system.h"

class Point { //a simple 2D point class

  int x_;
  int y_;

public:

  Point(int x, int y) : x_(x), y_(y) {}
  auto x() const { return x_; }
  auto y() const { return y_; }

  bool operator== (const Point& other) { //Point must have an equality operator

    return x() == other.x() && y() == other.y();
  }

  friend std::ostream& operator<< (std::ostream& stream, const Point& point) { //Point must have an ostream << operator

    stream << "(" << point.x() << ", " << point.y() << ")";

    return stream;
  }
};

//Custom hash struct, Point must be hashable
struct PointHash {
    std::size_t operator()(Point const& point) const noexcept {

        size_t h1 = std::hash<int>()(point.x());
        size_t h2 = std::hash<int>()(point.y());
        return h1 ^ (h2 << 1); //basic hashing of coordinates
    }
};

int main(int argc, char const *argv[]) {

  using namespace l_system;

  assert(argc >= 2 && "Usage: customtype generation");
  int generation = static_cast<int>(strtol(argv[1], nullptr, 0));

  LSymbolType A(Point(5, 3)); //define symbol types
  LSymbolType B(Point(2, 6));

  LSymbol A_(A); //define symbol instances needed for axiom and rules
  LSymbol B_(B);

  LRule A_AB(A, {A, B}); //define rules
  LRule B_A(B, {A});

  LSystem<Point, PointHash> points({LSymbol(A)}); //custom hash types require explicit template declaration for the LSystem type

  points.setRule(A, A_AB); //add the rules to the system
  points.setRule(B, B_A);

  std::cout << "Points generation " << generation << ": " << represent(points.generate(generation)) << '\n';

  return 0;
}
