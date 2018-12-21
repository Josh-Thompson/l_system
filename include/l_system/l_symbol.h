#ifndef L_SYSTEM_SYMBOL_H
#define L_SYSTEM_SYMBOL_H

#include "l_system/l_param.h"

namespace l_system::symbol {

  using namespace param;

  template <typename T>
  class LSymbolType {

    T representation_;
    LParameterSet parameterSet_;
    LParameterCustomSize customSize_;

    public:

    LSymbolType(T representation = T(), LParameterSet parameterSet = LNONE, LParameterCustomSize customSize = 0) :
      representation_(representation),
      parameterSet_(parameterSet),
      customSize_(customSize) {}

    auto representation() const noexcept -> T {

      return representation_;
    }

    auto paramSet() const noexcept -> LParameterSet {

      return parameterSet_;
    }

    auto customParamSize() const noexcept -> LParameterCustomSize {

      return customSize_;
    }

    bool operator==(const LSymbolType &other) const {

      return representation() == other.representation();
    }

    bool operator<(const LSymbolType &other) const {

      return representation() < other.representation();
    }
  };

  template <typename T, typename H>
  struct LSymbolTypeHash {

    size_t operator()(const LSymbolType<T>& k) const {

      return H()(k.representation());
    }
  };

  template <typename T>
  class LSymbol {

    LSymbolType<T> type_;
    LParameterData parameters_;

  public:
    LSymbol(LSymbolType<T> type = LSymbolType<T>()) : type_(type), parameters_(type.paramSet(), type.customParamSize()) {}

    auto type() const noexcept -> LSymbolType<T> {

      return type_;
    }

    auto paramSet() const noexcept -> LParameterSet {

      return type_.paramSet();
    }

    auto customParamSize() const noexcept -> LParameterCustomSize {

      return type_.customParamSize();
    }

    auto getCharParam(LParameterCount n) const noexcept -> char {

      return parameters_.getChar(n);
    }

    auto getIntParam(LParameterCount n) const noexcept -> int {

      return parameters_.getInt(n);
    }

    auto getFloatParam(LParameterCount n) const noexcept -> float {

      return parameters_.getFloat(n);
    }

    auto getCustomParam(LParameterCount n) const noexcept -> std::vector<unsigned char> {

      return parameters_.getCustom(n);
    }

    void setCharParam(char c, LParameterCount n) noexcept {

      parameters_.setChar(c, n);
    }

    void setIntParam(int i, LParameterCount n) noexcept {

      parameters_.setInt(i, n);
    }

    void setFloatParam(float f, LParameterCount n) noexcept {

      parameters_.setFloat(f, n);
    }

    void setCustomParam(const std::vector<unsigned char>& c, LParameterCount n) {

      parameters_.setCustom(c, n);
    }
  };

  template <typename T>
  using LString = std::vector<LSymbol<T>>;

  template <typename T>
  auto represent(const LString<T>& lstring, bool showParams = false) noexcept -> std::string {

    std::ostringstream stream;

    for(LSymbol<T> symbol : lstring) {

      stream << symbol.type().representation();

      if(!empty(symbol.paramSet()) && showParams) {

        stream << '(';

        for(LParameterCount i = 0; i < parameterCount(symbol.paramSet(), LCHAR); ++i) {

          stream << symbol.getCharParam(i) << ' ';
        }
        for(LParameterCount i = 0; i < parameterCount(symbol.paramSet(), LINT); ++i) {

          stream << symbol.getIntParam(i) << ' ';
        }
        for(LParameterCount i = 0; i < parameterCount(symbol.paramSet(), LFLOAT); ++i) {

          stream << symbol.getFloatParam(i) << ' ';
        }
        for(LParameterCount i = 0; i < parameterCount(symbol.paramSet(), LCUSTOM); ++i) {

          stream << param::represent(symbol.getCustomParam(i), false) << ' ';
        }

        stream << ')';
      }
    }

    return stream.str();
  }
}

#endif
