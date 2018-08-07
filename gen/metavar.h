

#pragma once

#include "hashutils.hpp"
#include "metavar_ptr.h"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <token.h>

#include <type_ptr.h>

#include <cstdint>

struct Metavar {
  const uint8_t i;
  const type typ;
  mutable token tok;
  // {'body': '{ return std::make_pair(typ, i); }', 'type': 'std::pair<type,
  // uint8_t>'}
  std::pair<type, uint8_t> typ_i() const;

  Metavar() : i(uint8_t()), typ(type()) {}

  Metavar(const uint8_t &i, const type &typ) : i(i), typ(typ) {}

  static metavar create(const uint8_t &i, const type &typ);

  static metavar get_or_create(const std::pair<type, uint8_t> &x);
  static metavar get_if_exists(const std::pair<type, uint8_t> &x);
  metavar save() const { return get_or_create(typ_i()); }

  inline static metavar create(const std::pair<type, uint8_t> typ_i) {
    return Metavar::create(typ_i.first, typ_i.second);
  }
};

struct MetavarIndex {

  // {'getter': 'typ', 'type': 'type'}
  using lookup_by_typ_index_type = std::unordered_multimap<type, metavar>;
  static lookup_by_typ_index_type lookup_by_typ_index;
  using lookup_by_typ_index_iterator = lookup_by_typ_index_type::const_iterator;
  static std::pair<lookup_by_typ_index_iterator, lookup_by_typ_index_iterator>
  lookup_by_typ(const type &x);

  // {'getter': 'typ_i()', 'unique': True, 'type': 'std::pair<type, uint8_t>'}
  static std::unordered_map<std::pair<type, uint8_t>, metavar>
      lookup_by_typ_i_index;
  static metavar lookup_by_typ_i(const std::pair<type, uint8_t> &x);

  static metavar index(metavar);
  static std::vector<Metavar> all_Metavars;
};

std::ostream &operator<<(std::ostream &os, const Metavar &);
std::ostream &operator<<(std::ostream &os, const metavar &);
