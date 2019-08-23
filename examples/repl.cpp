//An example of a basic REPL-like interface (catchy isn't it?) for an l system on characters

#include <iostream>
#include <unordered_map>
#include <string>
#include "l_system/l_system.h"

using namespace l_system;

//helper functions
constexpr bool isValidSymbol(char c) {

  constexpr const auto invalid = {' ', '-', '>', '!', '(', ')'};

  for(auto in : invalid) {

    if (c == in) {

      return false;
    }
  }
  return true;
}

bool doesSymbolExist(std::unordered_map<char, LSymbolType<char>> symbols, char c) {

  return symbols.count(c) != 0;
}

bool doesAllSymbolSExist(std::unordered_map<char, LSymbolType<char>> symbols, std::string cs) {

  for(auto c : cs) {
    if (symbols.count(c) == 0) {

      return false;
    }
  }

  return true;
}

int main() {

  std::unordered_map<char, LSymbolType<char>> symbolTypes;
  const std::string arrow = "->";
  bool startup = true;

  LSystem<char> system({});

  for(std::string input; input != "exit"; std::cin >> input) {

    if(input == "help") { //print help

      std::cout << "This REPL runs with a character representation" << '\n';
      std::cout << "Enter a single character to define it as a symbol." << '\n';
      std::cout << "Enter an expression of the form a->abc.. to add a rule." << '\n';
      std::cout << "Enter an expression of the form (abc..) to set the axiom." << '\n';
      std::cout << "Enter an expression of the form N! to evaluate the system at N generations." << '\n';
      std::cout << "Enter 'info' to display info about the current system." << '\n';
      std::cout << "Enter 'help' to display this message." << '\n';
      std::cout << "Enter 'exit' to exit the program." << '\n';
    }
    else if(input == "info") {

      std::cout << "Current axiom: " << represent(system.axiom()) << '\n';
      std::cout << "All defined symbols: ";

      for(const auto& pair : symbolTypes) {

        std::cout << pair.first << ' ';
      }

      std::cout << "\nAll symbols used in system: ";

      for(const auto& symbolType : system.getAllSymbolTypes()) {

        std::cout << symbolType.representation() << ' ';
      }

      std::cout << "\nRules used in system: ";

      for(const auto& rule : system.rules()) {

        std::cout << rule.representation() << ' ';
      }

      std::cout << '\n';
    }
    else if(input.find(arrow) != std::string::npos) { //add rule

      auto from = input.substr(0, input.find(arrow));
      auto to = input.substr(input.find(arrow) + arrow.size());

      if(from.size() != 1) {

        std::cout << "Invalid predecessor " << from << " in rule. Predecessors may only be a single symbol." << '\n';
      }
      else if(!doesSymbolExist(symbolTypes, from[0])) {

        std::cout << "Invalid predecessor " << from << " in rule. Predecessor does not exist." << '\n';
      }
      else if(!doesAllSymbolSExist(symbolTypes, to)) {

        std::cout << "Invalid successor " << to << " in rule. Successor contains nonexistent symbols." << '\n';
      }
      else {

        LSymbolType<char> predecessor = symbolTypes.at(from[0]);
        LTypeString<char> successor;

        for(auto c : to) {

          successor.emplace_back(symbolTypes.at(c));
        }

        system.addRule(LRule(predecessor, successor));
        std::cout << "Added rule " << input << '\n';
      }
    }
    else if(input.size() == 1) { //define symbol type

      char c = input[0];
      if(!isValidSymbol(c)) {
        std::cout << "Invalid symbol, character " << c << " is reserved." << '\n';
      }
      else if(doesSymbolExist(symbolTypes, c)) {

        std::cout << "Symbol " << c << " already exists." << '\n';
      }
      else {
        symbolTypes.insert_or_assign(c, LSymbolType(c));
        std::cout << "Added symbol type " << c << '\n';
      }
    }
    else if(input.find("(") != std::string::npos && input.find(")") != std::string::npos) { //set axiom

      auto axiomString = input.substr(1, input.size() - 2);

      if(!doesAllSymbolSExist(symbolTypes, axiomString)) {

        std::cout << "Invalid axiom (" << axiomString << "); contains undefined symbols." << '\n';
      }
      else {

        LString<char> axiom;

        for(auto c : axiomString) {

          axiom.emplace_back(LSymbol(symbolTypes.at(c)));
        }

        std::cout << "Set axiom to " << axiomString << '\n';
        system.setAxiom(axiom);
      }
    }
    else if(input[input.size() - 1] == '!') { //generate

      try {

        std::cout << represent(system.generate(stoi(input))) << '\n';
      }
      catch(const std::invalid_argument&) {

          std::cout << input.substr(0, input.size() - 1) << " is not a valid number." << '\n';
      }
    }
    else {

      if(startup) {

        std::cout << "\nBasic l system REPL\n\n";
        startup = false;
      }
      else {

        std::cout << "Unrecognized input. Enter 'help' for info." << '\n';
      }
    }
  }

  return 0;
}
