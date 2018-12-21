#ifndef L_SYSTEM_RULE_H
#define L_SYSTEM_RULE_H

#include "l_system/l_symbol.h"
#include "l_system/l_param.h"

namespace l_system::rule {

  using namespace symbol;
  using namespace param;

  template <typename T>
  class LRule {

    LSymbolType<T> predecessor_;
    LTypeString<T> result_;

  public:

    LRule(LSymbolType<T> predecessor, LTypeString<T> result) : predecessor_(predecessor), result_(result) {}

    LRule(LSymbolType<T> predecessor, std::initializer_list<LSymbolType<T>> result) : predecessor_(predecessor), result_(result) {}

    auto result() const noexcept -> LTypeString<T> {

      return result_;
    }

    auto operator()([[maybe_unused]] LSymbol<T> inputSymbol, [[maybe_unused]] LString<T> before, [[maybe_unused]] LString<T> after) const noexcept -> LString<T> {

      LString<T> result;
      result.reserve(result_.size());

      for(auto l : result_) {

        result.emplace_back(LSymbol<T>(l));
      }

      return result;
    }
  };
}

#endif
