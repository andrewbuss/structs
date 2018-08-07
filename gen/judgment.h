

#pragma once

#include "hashutils.hpp"
#include "judgment_ptr.h"
#include "prettyprint.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <token.h>

#include <type.h>

#include <constant.h>

#include <shape.h>

#include <metavar.h>

using naming = std::vector<uint8_t>;

using shape_naming = std::pair<shape, naming>;

struct Judgment {
  const shape shp;
  const naming nam;
  // {'body': '{\n  std::vector<token> cs(shp->cs);\n  int x = 0;\n  for(auto& s
  // : cs) {\n    if(s->typ) {\n      uint8_t i = nam[x++];\n      s =
  // s->typ->mvs[i]->tok;\n    }\n  }\n  return cs;\n}', 'type':
  // 'std::vector<token>'}
  std::vector<token> cs() const; // {'body': '{ return std::make_pair(shp, nam);
                                 // }', 'type': 'shape_naming'}
  shape_naming shp_nam() const;

  Judgment() : shp(shape()), nam(naming()) {}

  Judgment(const shape &shp, const naming &nam) : shp(shp), nam(nam) {}

  static judgment create(const shape &shp, const naming &nam);

  static judgment get_or_create(const shape_naming &x);
  static judgment get_if_exists(const shape_naming &x);
  judgment save() const { return get_or_create(shp_nam()); }

  static judgment get_or_create(const std::vector<std::string> &ss) {
    std::vector<token> toks;
    for (const auto &s : ss) {
      toks.push_back(Token::get_or_create(s));
    }
    return get_or_create(toks);
  }
  static judgment get_or_create(const std::vector<token> &ts) {
    std::vector<token> shp(ts);
    naming nam;
    for (auto &t : shp) {
      if (t->mv) {
        t = t->mv->typ->tok;
        nam.push_back(t->mv->i);
      }
    }
    return get_or_create(std::make_pair(Shape::get_or_create(shp), nam));
  }
  inline static judgment create(const shape_naming &shp_nam) {
    return create(shp_nam.first, shp_nam.second);
  }
};

struct JudgmentIndex {

  // {'getter': 'nam', 'type': 'naming'}
  using lookup_by_naming_index_type = std::unordered_multimap<naming, judgment>;
  static lookup_by_naming_index_type lookup_by_naming_index;
  using lookup_by_naming_index_iterator =
      lookup_by_naming_index_type::const_iterator;
  static std::pair<lookup_by_naming_index_iterator,
                   lookup_by_naming_index_iterator>
  lookup_by_naming(const naming &x);

  // {'getter': 'cs()', 'unique': True, 'type': 'std::vector<token>'}
  static std::unordered_map<std::vector<token>, judgment> lookup_by_cs_index;
  static judgment lookup_by_cs(const std::vector<token> &x);

  // {'getter': 'shp', 'type': 'shape'}
  using lookup_by_shape_index_type = std::unordered_multimap<shape, judgment>;
  static lookup_by_shape_index_type lookup_by_shape_index;
  using lookup_by_shape_index_iterator =
      lookup_by_shape_index_type::const_iterator;
  static std::pair<lookup_by_shape_index_iterator,
                   lookup_by_shape_index_iterator>
  lookup_by_shape(const shape &x);

  // {'getter': 'shp_nam()', 'unique': True, 'type': 'shape_naming'}
  static std::unordered_map<shape_naming, judgment> lookup_by_shp_nam_index;
  static judgment lookup_by_shp_nam(const shape_naming &x);

  // {'getter': 'shp->typ()', 'type': 'type'}
  using lookup_by_type_index_type = std::unordered_multimap<type, judgment>;
  static lookup_by_type_index_type lookup_by_type_index;
  using lookup_by_type_index_iterator =
      lookup_by_type_index_type::const_iterator;
  static std::pair<lookup_by_type_index_iterator, lookup_by_type_index_iterator>
  lookup_by_type(const type &x);

  static judgment index(judgment);
  static std::vector<Judgment> all_Judgments;
};

std::ostream &operator<<(std::ostream &os, const Judgment &);
std::ostream &operator<<(std::ostream &os, const judgment &);
