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
    LString<T> result_;

  public:

    LRule(LSymbolType<T> predecessor, LString<T> result) : predecessor_(predecessor), result_(result) {}

    LRule(LSymbolType<T> predecessor, std::initializer_list<LSymbol<T>> result) : predecessor_(predecessor), result_(result) {}

    auto result() const noexcept -> LString<T> {

      return result_;
    }

    auto operator()([[maybe_unused]] LSymbol<T> inputSymbol, [[maybe_unused]] LString<T> before, [[maybe_unused]] LString<T> after) const noexcept -> LString<T> {

      auto result = result_;


      return result;
    }
  };
}

#endif
