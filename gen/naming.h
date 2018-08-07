

#pragma once

#include "hashutils.hpp"
#include "naming_ptr.h"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <token.h>

#include <cstdint>

struct Naming {
  // {'is': 'std::vector<uint8_t>'}
  const std::vector<uint8_t> is;

  Naming() : is(std::vector<uint8_t>()) {}

  Naming(const std::vector<uint8_t> &is) : is(is) {}

  static naming create(const std::vector<uint8_t> &is);

  static naming get_or_create(const std::vector<uint8_t> &x);
  static naming get_if_exists(const std::vector<uint8_t> &x);
  naming save() const { return get_or_create(is); }
};

struct NamingIndex {

  // {'getter': 'is', 'unique': True, 'type': 'std::vector<uint8_t>'}
  static std::unordered_map<std::vector<uint8_t>, naming> lookup_by_is_index;
  static naming lookup_by_is(const std::vector<uint8_t> &x);

  static naming index(naming);
  static std::vector<Naming> all_Namings;
};

std::ostream &operator<<(std::ostream &os, const Naming &);
std::ostream &operator<<(std::ostream &os, const naming &);
