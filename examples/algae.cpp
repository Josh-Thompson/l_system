//Basic demonstration of Aristid Lindenmayer's original L-System for modeling the growth of algae.
//Also demonstrates basic runtime modification of a trivial L-System

#include <iostream>
#include <cassert>
#include <stdlib.h>

#include "l_system/l_system.h"

int main(int argc, char const *argv[]) {

  using namespace l_system;

  assert(argc >= 2 && "Usage: algae generation");
  int generation = static_cast<int>(strtol(argv[1], nullptr, 0));

  //define symbol types. Any type can be passed to the constructor, so long as they are hashable.
  LSymbolType A('A'); //true / false, 5 / 6, -0.33 / 99999.3, anything goes
  LSymbolType B('B'); //if a custom type is used, a custom hash type can be passed to the LSystem constructor.

  LSymbol A_(A); //define symbols needed for axiom and rules
  LSymbol B_(B);

  LRule A_AB(A, {A_, B_}); //define rules
  LRule B_A(B, {A_});

  LSystem algae({LSymbol(A)}); //define the system using an axiom

  algae.setRule(A, A_AB); //add the rules to the system
  algae.setRule(B, B_A);

  std::cout << "Algae axiom: " << represent(algae.axiom()) << '\n'; //the represent function can turn an l system into something readable

  std::cout << "Algae generation " << generation << ": " << represent(algae.generate(generation)) << '\n'; //generate a generation

  LSymbolType C('C'); //new symbol types can be designated on the fly

  LSymbol C_(C); //new symbols as well

  LRule B_CA(B, {C_, A_}); //and new rules

  algae.setRule(B, B_CA); //the rules can be changed

  algae.setAxiom({A_, C_, C_}); //the axiom can also be modified

  std::cout << "\nAlgae generation " << generation << " with C rule : " << represent(algae.generate(generation)) << '\n'; //and generations can be created again

  return 0;
}
