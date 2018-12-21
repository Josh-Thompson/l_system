#ifndef L_SYSTEM_RULE_H
#define L_SYSTEM_RULE_H

#include "l_system/l_symbol.h"
#include "l_system/l_param.h"

namespace l_system::rule {

  using namespace symbol;
  using namespace param;

  class LRuleParameterReference {

    LParameterCount sourceIndex_;
    LParameterCount destIndex_;
    LParameter type_;
    //ParameterTransformation transformation_;

  public:
    LRuleParameterReference(LParameterCount sourceIndex, LParameterCount destIndex, LParameter type) : sourceIndex_(sourceIndex), destIndex_(destIndex), type_(type) {}

    auto sourceIndex() const noexcept -> LParameterCount {

      return sourceIndex_;
    }

    auto destIndex() const noexcept -> LParameterCount {

      return destIndex_;
    }

    auto type() const noexcept -> LParameter {

      return type_;
    }
  };

  using LRuleParamRefSet = std::vector<std::vector<LRuleParameterReference>>;

  template <typename T>
  class LRule {

    LSymbolType<T> predecessor_;
    LTypeString<T> result_;
    LRuleParamRefSet paramTransforms_;

  public:

    LRule(LSymbolType<T> predecessor, LTypeString<T> result, LRuleParamRefSet paramTransforms = {}) :
      predecessor_(predecessor),
      result_(result),
      paramTransforms_(paramTransforms) {}



    auto result() const noexcept -> LTypeString<T> {

      return result_;
    }

    //This operator needs to be refactored
    auto operator()(LSymbol<T> inputSymbol, [[maybe_unused]] LString<T> before, [[maybe_unused]] LString<T> after) const noexcept -> LString<T> {

      LString<T> result;
      result.reserve(result_.size());

      //for if for switch????? why
      for(size_t symbolIndex = 0; symbolIndex < result_.size(); ++symbolIndex) {

        auto resSymbol = LSymbol<T>(result_.at(symbolIndex));

        if(symbolIndex < paramTransforms_.size()) {

          for(size_t paramIndex = 0; paramIndex < paramTransforms_.at(symbolIndex).size(); ++paramIndex) {

            auto reference = paramTransforms_.at(symbolIndex).at(paramIndex);

            switch(reference.type()) { //TODO get rid of this switch statement

              case LCHAR:
                resSymbol.setCharParam(inputSymbol.getCharParam(reference.sourceIndex()), reference.destIndex());
                break;
              case LINT:
                resSymbol.setIntParam(inputSymbol.getIntParam(reference.sourceIndex()), reference.destIndex());
                break;
              case LFLOAT:
                resSymbol.setFloatParam(inputSymbol.getFloatParam(reference.sourceIndex()), reference.destIndex());
                break;
              case LCUSTOM:
                resSymbol.setCustomParam(inputSymbol.getCustomParam(reference.sourceIndex()), reference.destIndex());
                break;
              default:
                break;
            }
          }
        }

        result.emplace_back(resSymbol);
      }

      return result;
    }
  };
}

#endif
