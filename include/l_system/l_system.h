#ifndef L_SYSTEM_H
#define L_SYSTEM_H

#include <unordered_map>
#include <numeric>
#include <iterator>
#include <functional>
#include <vector>
#include <set>
#include <sstream>

namespace l_system {

  template <typename T>
  class LSymbolType {

    T representation_;

    public:

    LSymbolType() {}
    LSymbolType(T representation) : representation_(representation) {}

    auto representation() const noexcept -> T {

      return representation_;
    }

    bool operator==(const LSymbolType &other) const {

      return representation() == other.representation();
    }

    bool operator<(const LSymbolType &other) const {

      return representation() < other.representation();
    }
  };

  template <typename T>
  LSymbolType<T> NullLSymbol() {

    static LSymbolType<T> res;

    return res;
  }

  template <typename T, typename H>
  struct LSymbolTypeHash {

    size_t operator()(const LSymbolType<T>& k) const {

      return H()(k.representation());
    }
  };

  template <typename T>
  class LSymbol {

    LSymbolType<T> type_;

  public:
    LSymbol(LSymbolType<T> type) : type_(type) {}

    LSymbol() : type_(NullLSymbol<T>()) {}

     auto type() const noexcept -> LSymbolType<T> {

      return type_;
    }
  };

  template <typename T>
  using LString = std::vector<LSymbol<T>>;

  template <typename T>
  auto represent(const LString<T>& lstring) noexcept -> std::string {

    std::ostringstream stream;

    for(LSymbol<T> symbol : lstring) {

      stream << symbol.type().representation();
    }

    return stream.str();
  }

  template <typename T>
  class LRule {

    LString<T> result_;

  public:

    LRule(LString<T> result) : result_(result) {}

    LRule(std::initializer_list<LSymbol<T>> result) : result_(result) {}

    auto result() const noexcept -> LString<T> {

      return result_;
    }

    auto operator()([[maybe_unused]] LString<T> before, [[maybe_unused]] LString<T> after) const noexcept -> LString<T> {

      return result_;
    }
  };

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
            symbolReplacement = rules_.at(symbol->type())(before, after);
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
