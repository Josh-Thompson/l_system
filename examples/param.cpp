//an example of the parameter system, decoupled from any l system and existing standalone

#include <iostream>
#include <iomanip>

#include "l_system/l_param.h"

int main() {

  using namespace l_system; //parameters are decoupled from the central system, they can be worked with outside of any l system context

  LParameterSet emptySet = parameterSet(); //creates an empty parameter set
  LParameterSet oneOfEach = parameterSet({LCHAR, LINT, LFLOAT, LCUSTOM}); //insert each type the number of times you want to include it
  LParameterSet one234 = parameterSet({LCHAR, LINT, LFLOAT, LCUSTOM, LINT, LFLOAT, LCUSTOM, LFLOAT, LCUSTOM, LCUSTOM});
  LParameterSet five678 = parameterSet(5, 6, 7, 8); //you can also specify number of chars, ints, floats, and custom datas in that order
  LParameterSet max = parameterSet(MAX_PARAMS, MAX_PARAMS, MAX_PARAMS, MAX_PARAMS);

  std::cout << empty(emptySet) << '\n'; //you can check emptiness

  std::cout << represent(emptySet) << '\n'; //parameter sets can be printed
  std::cout << represent(oneOfEach) << '\n'; //pretty print a parameter set
  std::cout << represent(one234) << '\n';

  std::cout << +parameterCount(one234, LCHAR) << '\n'; //you can query the parameter count for a given set and parameter type
  std::cout << +parameterCount(one234, LINT) << '\n';
  std::cout << +parameterCount(one234, LFLOAT) << '\n';
  std::cout << +parameterCount(one234, LCUSTOM) << '\n';

  std::cout << +parameterCount(five678, LCHAR) << '\n';
  std::cout << +parameterCount(five678, LINT) << '\n';
  std::cout << +parameterCount(five678, LFLOAT) << '\n';
  std::cout << +parameterCount(five678, LCUSTOM) << '\n';

  std::cout << +parameterCount(max, LCHAR) << '\n';
  std::cout << +parameterCount(max, LINT) << '\n';
  std::cout << +parameterCount(max, LFLOAT) << '\n';
  std::cout << +parameterCount(max, LCUSTOM) << '\n';

  LParameterData a(oneOfEach); //a ParameterData actually holds an instance of data determined by a parameter set
  LParameterData b(five678, 6); //you can specify a custom size for the custom data type parameter

  std::cout << a.getInt(0) << '\n'; //all parameters are initialized to zero
  std::cout << +b.getCustom(7)[0] << '\n'; //custom data is output as a vector of unsigned chars of your given size

  b.setInt(-1359, 5);
  std::cout << b.getInt(5) << '\n'; //set and get at positions

  b.setFloat(53.122f, 3);
  std::cout << b.getFloat(3) << '\n'; //floats

  a.setChar('c', 0);
  std::cout << a.getChar(0) << '\n'; //chars

  b.setCustom({0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54}, 7); //custom data - must be exact number of bytes specified at construction
  std::cout << represent(b.getCustom(7)) << '\n'; //represent can display raw values for custom data, but you could also pass to a constructor for your data type
  std::cout << represent(b.getCustom(7), false) << '\n'; //can also print without spacing
  return 0;
}
