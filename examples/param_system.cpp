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
  A_.setFloatParam(500.005f, 0);
  A_.setFloatParam(200.002f, 1);
  A_.setFloatParam(-100.001f, 2);
  LSymbol B_(B);
  B_.setIntParam(50285, 0);

  LRule A_AB(A, {A, B}, {
    {
      LRuleParameterReference(0, 1, LFLOAT),
      LRuleParameterReference(1, 2, LFLOAT),
      LRuleParameterReference(2, 0, LFLOAT),
    }
  }); //define rules

  LRule B_A(B, {A});

  LSystem system({A_}); //define the system using an axiom

  system.setRule(A, A_AB); //add the rules to the system
  system.setRule(B, B_A);

  std::cout << "system axiom: " << represent(system.axiom()) << '\n'; //the represent function can turn an l system into something readable

  std::cout << "system generation " << generation << ": " << represent(system.generate(generation), true) << '\n'; //generate a generation


  return 0;
}
