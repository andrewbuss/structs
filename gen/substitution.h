

#pragma once

#include "hashutils.hpp"
#include "prettyprint.hpp"
#include "substitution_ptr.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <token.h>

#include <shape.h>

#include <vector>

#include <judgment.h>

using judgment_substitutions = std::pair<judgment, std::vector<substitution>>;

using substitutions = std::vector<substitution>;

struct Substitution {
  const substitutions args;
  const judgment jud;
  // {'body': '{\n  std::vector<token> cs;\n
  // cs.push_back(jud->shp->typ()->tok);\n\n  mapping map;\n  /*for(auto& s :
  // judgment->shp) {\n    if(s->typ) {\n      uint8_t i = nam[x++];\n      s =
  // s->typ->mvs[i]->tok;\n    }\n  }*/\n  shape shp =
  // Shape::get_or_create(cs);\n  naming nam = Naming::get_or_create(map);\n
  // return Judgment::get_or_create(std::make_pair(shp, nam));\n}', 'type':
  // 'judgment'}
  judgment as_judgment() const; // {'body': '{ return std::make_pair(jud, args);
                                // }', 'type': 'judgment_substitutions'}
  judgment_substitutions jud_args() const;

  Substitution() : args(substitutions()), jud(judgment()) {}

  Substitution(const substitutions &args, const judgment &jud)
      : args(args), jud(jud) {}

  static substitution create(const substitutions &args, const judgment &jud);

  static substitution get_or_create(const judgment_substitutions &x);
  static substitution get_if_exists(const judgment_substitutions &x);
  substitution save() const { return get_or_create(jud_args()); }

  inline static substitution create(const judgment_substitutions &jud_args) {
    return create(jud_args.second, jud_args.first);
  }
  static substitution get_or_create(const judgment jud,
                                    const substitutions &args) {
    return get_or_create(make_pair(jud, args));
  }
};

struct SubstitutionIndex {

  // {'getter': 'args', 'type': 'substitutions', 'iterable': True}
  using lookup_by_args_index_type =
      std::unordered_multimap<substitutions, substitution>;
  static lookup_by_args_index_type lookup_by_args_index;
  using lookup_by_args_index_iterator =
      lookup_by_args_index_type::const_iterator;
  static std::pair<lookup_by_args_index_iterator, lookup_by_args_index_iterator>
  lookup_by_args(const substitutions &x);

  // {'getter': 'jud', 'type': 'judgment'}
  using lookup_by_jud_index_type =
      std::unordered_multimap<judgment, substitution>;
  static lookup_by_jud_index_type lookup_by_jud_index;
  using lookup_by_jud_index_iterator = lookup_by_jud_index_type::const_iterator;
  static std::pair<lookup_by_jud_index_iterator, lookup_by_jud_index_iterator>
  lookup_by_jud(const judgment &x);

  // {'getter': 'jud_args()', 'unique': True, 'type': 'judgment_substitutions'}
  static std::unordered_map<judgment_substitutions, substitution>
      lookup_by_jud_args_index;
  static substitution lookup_by_jud_args(const judgment_substitutions &x);

  static substitution index(substitution);
  static std::vector<Substitution> all_Substitutions;
};

std::ostream &operator<<(std::ostream &os, const Substitution &);
std::ostream &operator<<(std::ostream &os, const substitution &);
