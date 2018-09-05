

#pragma once

#include "hashutils.hpp"
#include "naming_ptr.h"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <stdint.h>

#include <vector>

using mapping = std::vector<int>;

struct Naming {
  const mapping map;

  Naming() : map(mapping()) {}

  Naming(const mapping &map) : map(map) {}

  static naming create(const mapping &map);

  static naming get_or_create(const mapping &x);
  static naming get_if_exists(const mapping &x);
  naming save() const { return get_or_create(map); }
};

struct NamingIndex {

  // {'getter': 'map', 'unique': True, 'type': 'mapping'}
  static std::unordered_map<mapping, naming> lookup_by_map_index;
  static naming lookup_by_map(const mapping &x);

  static naming index(naming);
  static std::vector<Naming> all_Namings;
};

std::ostream &operator<<(std::ostream &os, const Naming &);
std::ostream &operator<<(std::ostream &os, const naming &);
