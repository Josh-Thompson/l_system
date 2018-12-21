//an l system using parameters
#include <iostream>

#include "l_system/l_system.h"

using namespace l_system;

int main(int argc, char** argv) {

  assert(argc >= 2 && "Usage: algae generation");
  int generation = static_cast<int>(strtol(argv[1], nullptr, 0));

  LSymbolType A('A', parameterSet({LFLOAT, LFLOAT, LFLOAT}));
  LSymbolType B('B', parameterSet({LINT}));

  LSymbol A_(A); //define symbols needed for axiom and rules, as well as their parameters
  A_.setFloatParam(0.5f, 0);
  A_.setFloatParam(-0.5f, 1);
  A_.setFloatParam(0.25f, 2);
  LSymbol B_(B);
  B_.setIntParam(50285, 0);

  LRule A_AB(A, {A_, B_}); //define rules
  LRule B_A(B, {A_});

  LSystem system({LSymbol(A)}); //define the system using an axiom

  system.setRule(A, A_AB); //add the rules to the system
  system.setRule(B, B_A);

  std::cout << "system axiom: " << represent(system.axiom()) << '\n'; //the represent function can turn an l system into something readable

  std::cout << "system generation " << generation << ": " << represent(system.generate(generation), true) << '\n'; //generate a generation


  return 0;
}
