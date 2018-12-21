#ifndef L_SYSTEM_H
#define L_SYSTEM_H

#include <unordered_map>
#include <set>

#include "l_system/l_param.h"
#include "l_system/l_rule.h"

namespace l_system {

  using namespace param;
  using namespace symbol;
  using namespace rule;

  template <typename T, typename H = std::hash<T>>
  class LSystem {

    LString<T> axiom_;
    std::unordered_map<LSymbolType<T>, LRule<T>, LSymbolTypeHash<T, H>> rules_;

  public:

    LSystem(LString<T> axiom) : axiom_(axiom) {}
    LSystem(std::initializer_list<LSymbol<T>> axiom) : axiom_(axiom) {}

    void setRule(LSymbolType<T> type, LRule<T> rule) noexcept {

      rules_.insert_or_assign(type, rule);
    }

    void setAxiom(const LString<T>& axiom) noexcept {

      axiom_ = axiom;
    }

    auto axiom() const noexcept -> LString<T> {

      return axiom_;
    }

    auto rules() const noexcept -> std::unordered_map<LSymbolType<T>, LRule<T>, LSymbolTypeHash<T, H>> {

      return rules_;
    }

    auto generate(int generations) const noexcept -> LString<T> {

      auto current = axiom_;

      for(int i = 0; i < generations; ++i) {

        std::vector<LString<T>> result;
        result.reserve(current.size());

        for(auto symbol = current.begin(); symbol != current.end(); symbol++) {

          bool symbolIsTerminal = rules_.count(symbol->type()) == 0;

          LString<T> symbolReplacement;

          if(symbolIsTerminal) {

            symbolReplacement.emplace_back(*symbol);
          }
          else {

            auto before = (symbol == current.begin()) ? LString<T>() : LString<T>(current.begin(), symbol - 1);
            auto after = (symbol == current.end() - 1) ? LString<T>() : LString<T>(symbol + 1, current.end() - 1);
            symbolReplacement = rules_.at(symbol->type())(*symbol, before, after);
          }

          result.emplace_back(symbolReplacement);
        }

        current = LString<T>();

        for(auto& res : result) {

          current.insert(current.end(), std::make_move_iterator(res.begin()), std::make_move_iterator(res.end()));
        }
      }

      return current;
    }

    auto getAllSymbolTypes() const noexcept -> std::set<LSymbolType<T>> {

      std::set<LSymbolType<T>> result;

      for(const auto& symbol : axiom_) {

        result.emplace(symbol.type());
      }

      for(const auto& [pred, succ] : rules_) {

        result.emplace(pred);

        for(const auto& symbol : succ.result()) {

          result.emplace(symbol.type());
        }
      }

      return result;
    }
  };
}

#endif
