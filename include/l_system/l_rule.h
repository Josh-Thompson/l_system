#ifndef L_SYSTEM_RULE_H
#define L_SYSTEM_RULE_H

#include "l_system/l_symbol.h"
#include "l_system/l_param.h"

namespace l_system {

  template <typename T>
  class LRule {

    LSymbolType<T> predecessor_;
    LTypeString<T> result_;

  public:

    LRule(LSymbolType<T> predecessor, LTypeString<T> result) :
      predecessor_(predecessor),
      result_(result) {}

    auto result() const noexcept -> LTypeString<T> {

      return result_;
    }

    auto predecessor() const noexcept -> LSymbolType<T> {

      return predecessor_;
    }

    auto applies(LSymbol<T> symbol) const noexcept -> bool {

      return symbol.type() == predecessor_;
    }

    auto produce(LSymbol<T> symbol) const noexcept -> LString<T> {

      LString<T> result;
      result.reserve(result_.size());

      for(size_t symbolIndex = 0; symbolIndex < result_.size(); ++symbolIndex) {

        result.emplace_back(LSymbol<T>(result_.at(symbolIndex)));
      }

      return result;
    }

    auto representation() const noexcept -> std::string {

      std::ostringstream stream;

      stream << predecessor_.representation();
      stream << "->";
      stream << represent(result_);

      return stream.str();
    }
  };
}

#endif
