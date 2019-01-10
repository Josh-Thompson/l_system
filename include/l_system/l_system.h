#ifndef L_SYSTEM_H
#define L_SYSTEM_H

#include <unordered_map>
#include <set>

#include "l_system/l_param.h"
#include "l_system/l_rule.h"

namespace l_system {

  template <typename T>
  class LSystem {

    LString<T> axiom_;
    std::vector<LRule<T>> rules_;

  public:

    LSystem(LString<T> axiom) : axiom_(axiom) {}
    LSystem(std::initializer_list<LSymbol<T>> axiom) : axiom_(axiom) {}

    void addRule(LRule<T> rule) noexcept {

      rules_.emplace_back(rule);
    }

    void setAxiom(const LString<T>& axiom) noexcept {

      axiom_ = axiom;
    }

    auto axiom() const noexcept -> LString<T> {

      return axiom_;
    }

    auto rules() const noexcept -> std::vector<LRule<T>> {

      return rules_;
    }

    auto generate(int generations) const noexcept -> LString<T> {

      auto current = axiom_;

      for(int i = 0; i < generations; ++i) {

        std::vector<LString<T>> result;
        result.reserve(current.size());

        for(size_t j = 0; j < current.size(); ++j) {

          LString<T> symbolReplacement = {current[j]};

          for(const auto& rule : rules_) {

              if(rule.applies(current[j])) {

                symbolReplacement = rule.produce(current[j]);
              }
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

      for(const auto& rule : rules_) {

        result.emplace(rule.predecessor());

        for(const auto& symbolType : rule.result()) {

          result.emplace(symbolType);
        }
      }

      return result;
    }
  };
}

#endif
