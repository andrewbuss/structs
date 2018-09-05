

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

#include <shape.h>

#include <naming.h>

#include <metavar.h>

#include <string>

#include <vector>

using ForwardRenaming = std::unordered_map<token, metavar>;

using shape_naming = std::pair<shape, naming>;

using judgments = std::vector<judgment>;

struct Judgment {
  const shape shp;
  const naming nam;
  // {'body': '{\n  std::vector<token> rv(shp->shp);\n  int x = 0;\n  bool first
  // = true;\n  for(auto& s : rv) {\n    if(first) {\n      first = false;\n
  // continue;\n    }\n    if(s->typ) {\n      uint8_t i = nam->map[x++];\n s =
  // s->typ->mvs[i]->tok;\n    }\n  }\n  return Shape::get_or_create(rv);\n}',
  // 'type': 'shape'}
  shape flattened() const; // {'body': '{ return std::make_pair(shp, nam); }',
                           // 'type': 'shape_naming'}
  shape_naming shp_nam() const;

  Judgment() : shp(shape()), nam(naming()) {}

  Judgment(const shape &shp, const naming &nam) : shp(shp), nam(nam) {}

  static judgment create(const shape &shp, const naming &nam);

  static judgment get_or_create(const shape_naming &x);
  static judgment get_if_exists(const shape_naming &x);
  judgment save() const { return get_or_create(shp_nam()); }

  inline static judgment create(const shape_naming &shp_nam) {
    return create(shp_nam.first, shp_nam.second);
  }
  static judgment get_or_create(const std::vector<std::string> &ss,
                                const ForwardRenaming &r) {
    std::vector<token> tokens;
    for (const auto &s : ss) {
      tokens.push_back(Token::get_or_create(s));
    }
    return get_or_create(tokens, r);
  }
  inline static judgment get_or_create(std::vector<token> cs) {
    return get_or_create(cs, {});
  }
  inline static judgment get_or_create(const std::vector<std::string> &ss) {
    return get_or_create(ss, {});
  }
  static judgment get_or_create(std::vector<token> cs,
                                const ForwardRenaming &r) {
    mapping map;
    for (token &t : cs) {
      if (t->var_typ) {
        map.push_back(r.at(t));
        t = r.at(t)->tok;
      } else if (t->mv) {
        map.push_back(t->mv->i);
        t = t->mv->typ->tok;
      } else if (r.find(t) != r.end()) {
        map.push_back(r.at(t)->i);
        t = r.at(t)->typ->tok;
      }
    }
    shape shp = Shape::get_or_create(cs);
    naming nam = Naming::get_or_create(map);
    return get_or_create(std::make_pair(shp, nam));
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

  static judgment index(judgment);
  static std::vector<Judgment> all_Judgments;
};

std::ostream &operator<<(std::ostream &os, const Judgment &);
std::ostream &operator<<(std::ostream &os, const judgment &);
