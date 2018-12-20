#ifndef L_SYSTEM_PARAM_H
#define L_SYSTEM_PARAM_H

#include <vector>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <array>

namespace l_system::param {

  namespace {

    template<typename T>
    auto fromBytes(const std::vector<unsigned char>& source, size_t offset) noexcept -> T {

      T t;

      unsigned char bytes[sizeof(T)];

      for(size_t i = 0; i < sizeof(bytes); i++) {

        bytes[i] = source.at(i + offset);
      }

      memcpy(&t, &bytes, sizeof(t));

      return t;
    }

    template<typename T>
    auto toBytes(T t) noexcept -> std::array<unsigned char, sizeof(T)> {

      std::array<unsigned char, sizeof(T)> result;

      memcpy(&result[0], &t, sizeof(t));

      return result;
    }

  }

  using LParameterSet = unsigned int;
  using LParameterCount = unsigned char;
  using LParameterCustomSize = unsigned char;
  using LParameterDataSize = size_t;

  constexpr const static LParameterCount MAX_PARAMS = 0xFF;

  enum LParameter : LParameterSet {

    LCHAR =   0x01000000,
    LINT =    0x00010000,
    LFLOAT =  0x00000100,
    LCUSTOM = 0x00000001,
    LNONE =   0x00000000,
  };

  auto represent(LParameterSet set) noexcept -> std::string {

    std::ostringstream res;

    res << std::setfill('0') << std::setw(8) << std::hex << set;

    return res.str();
  }

  auto represent(const std::vector<unsigned char>& data, bool spaced = true) noexcept -> std::string {

    std::ostringstream res;
    const auto spacing = (spaced) ? " " : "";

    for(auto d : data) {

      res << std::hex << static_cast<unsigned int>(d) << spacing;
    }

    return res.str();
  }

  auto parameterSet(std::initializer_list<LParameter> params_) noexcept -> LParameterSet {

    std::vector<LParameter> params(params_);

    auto charCount = std::count(params.begin(), params.end(), LCHAR);
    auto intCount = std::count(params.begin(), params.end(), LINT);
    auto floatCount = std::count(params.begin(), params.end(), LFLOAT);
    auto customCount = std::count(params.begin(), params.end(), LCUSTOM);

    assert(charCount <= MAX_PARAMS && "Too many chars.");
    assert(intCount <= MAX_PARAMS && "Too many ints.");
    assert(floatCount <= MAX_PARAMS && "Too many floats.");
    assert(customCount <= MAX_PARAMS && "Too many custom data objects.");

    return std::accumulate(params.begin(), params.end(), static_cast<LParameterSet>(LNONE));
  }

  auto parameterSet(LParameterCount chars = 0, LParameterCount ints = 0, LParameterCount floats = 0, LParameterCount customs = 0) noexcept -> LParameterSet {

    return (chars * LCHAR) + (ints * LINT) + (floats * LFLOAT) + (customs * LCUSTOM);
  }

  auto empty(LParameterSet set) noexcept -> bool {

    return set == LNONE;
  }

  auto parameterCount(LParameterSet set, LParameter param) noexcept -> LParameterCount {

    LParameterCount count = 0;

    switch (param) {
      case LCHAR:
        count = static_cast<LParameterCount>((set << 0) >> 24);
        break;
      case LINT:
        count = static_cast<LParameterCount>((set << 8) >> 24);
        break;
      case LFLOAT:
        count = static_cast<LParameterCount>((set << 16) >> 24);
        break;
      case LCUSTOM:
        count = static_cast<LParameterCount>((set << 24) >> 24);
        break;
      case LNONE:
        break;
      default:
        break;
    }

    return count;
  }

  auto requiredDataSize(LParameterSet set, LParameterCustomSize customSize = 1) noexcept -> LParameterDataSize {

    return (parameterCount(set, LCHAR) * sizeof(char))
    + (parameterCount(set, LINT) * sizeof(int))
    + (parameterCount(set, LFLOAT) * sizeof(float))
    + (static_cast<LParameterDataSize>(parameterCount(set, LCUSTOM)) * customSize); //cast because unsigned char * unsigned char == signed int :/
  }

  class LParameterData {

    const LParameterSet set_; //the information about the set of parameters, 4 bytes
    const LParameterCustomSize customSize_; //the information about the size of the custom parameter type, 1 byte
    std::vector<unsigned char> bytes_; //the data storage, sizeof(vector) bytes, probably 20 bytes maximum

  public:
    LParameterData(LParameterSet set, LParameterCustomSize customSize = 1) : set_(set), customSize_(customSize), bytes_(std::vector<unsigned char>(requiredDataSize(set, customSize))) {}

    auto set() const noexcept -> LParameterSet {

      return set_;
    }

    auto customSize() const noexcept -> LParameterCustomSize {

      return customSize_;
    }

    auto getChar(LParameterCount n) const noexcept -> char {

      assert(n < parameterCount(set_, LCHAR) && "out of bounds parameter access.");

      size_t offset = 0;

      return(static_cast<char>(bytes_.at(offset + n)));
    }

    auto getInt(LParameterCount n) const noexcept -> int {

      assert(n < parameterCount(set_, LINT) && "out of bounds parameter access.");

      size_t offset = sizeof(char) * parameterCount(set_, LCHAR);

      return fromBytes<int>(bytes_, offset + n * sizeof(int));
    }

    auto getFloat(LParameterCount n) const noexcept -> float {

      assert(n < parameterCount(set_, LFLOAT) && "out of bounds parameter access.");

      size_t offset = (sizeof(char) * parameterCount(set_, LCHAR)) + (sizeof(int) * parameterCount(set_, LINT));

      return fromBytes<float>(bytes_, offset + n * sizeof(float));
    }

    auto getCustom(LParameterCount n) const noexcept -> std::vector<unsigned char> {

      assert(n < parameterCount(set_, LCUSTOM) && "out of bounds parameter access.");

      size_t offset = (sizeof(char) * parameterCount(set_, LCHAR)) + (sizeof(int) * parameterCount(set_, LINT)) + (sizeof(float) * parameterCount(set_, LFLOAT));

      std::vector<unsigned char> bytes(customSize_);

      for(size_t i = 0; i < customSize_; i++) {

        bytes[i] = bytes_.at(i + offset + static_cast<size_t>(n * customSize_));
      }

      return bytes;
    }

    void setChar(char c, LParameterCount n) noexcept {

      assert(n < parameterCount(set_, LCHAR) && "out of bounds parameter assignment.");

      size_t offset = 0;

      bytes_[offset + n] = static_cast<unsigned char>(c);
    }

    void setInt(int i, LParameterCount n) noexcept {

      assert(n < parameterCount(set_, LINT) && "out of bounds parameter assignment.");

      size_t offset = sizeof(char) * parameterCount(set_, LCHAR);

      auto insert = toBytes<int>(i);

      for(size_t j = 0; j < sizeof(int); j++) {

        bytes_[j + offset + n * sizeof(int)] = insert[j];
      }
    }

    void setFloat(float f, LParameterCount n) noexcept {

      assert(n < parameterCount(set_, LFLOAT) && "out of bounds parameter assignment.");

      size_t offset = (sizeof(char) * parameterCount(set_, LCHAR)) + (sizeof(int) * parameterCount(set_, LINT));

      auto insert = toBytes<float>(f);

      for(size_t j = 0; j < sizeof(float); j++) {

        bytes_[j + offset + n * sizeof(float)] = insert[j];
      }
    }

    void setCustom(const std::vector<unsigned char>& c, LParameterCount n) {

      assert(n < parameterCount(set_, LCUSTOM) && "out of bounds parameter assignment.");
      assert(c.size() == customSize_ && "invalid custom data size");

      size_t offset = (sizeof(char) * parameterCount(set_, LCHAR)) + (sizeof(int) * parameterCount(set_, LINT)) + (sizeof(float) * parameterCount(set_, LFLOAT));

      for(size_t j = 0; j < customSize_; j++) {

        bytes_[j + offset + static_cast<size_t>(n * customSize_)] = c.at(j);
      }
    }
  };
}

#endif
