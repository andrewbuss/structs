

#include <substitution.h>

std::vector<Substitution> SubstitutionIndex::all_Substitutions{{}};
const Substitution *substitution::operator->() const {
  return SubstitutionIndex::all_Substitutions.data() + i;
}
const Substitution &substitution::operator*() const {
  return SubstitutionIndex::all_Substitutions[i];
}
substitution substitution::null = 0;

SubstitutionIndex::lookup_by_args_index_type
    SubstitutionIndex::lookup_by_args_index;

SubstitutionIndex::lookup_by_jud_index_type
    SubstitutionIndex::lookup_by_jud_index;

std::unordered_map<judgment_substitutions, substitution>
    SubstitutionIndex::lookup_by_jud_args_index;

// {'body': '{\n  std::vector<token> cs;\n
// cs.push_back(jud->shp->typ()->tok);\n\n  mapping map;\n  /*for(auto& s :
// judgment->shp) {\n    if(s->typ) {\n      uint8_t i = nam[x++];\n      s =
// s->typ->mvs[i]->tok;\n    }\n  }*/\n  shape shp = Shape::get_or_create(cs);\n
// naming nam = Naming::get_or_create(map);\n  return
// Judgment::get_or_create(std::make_pair(shp, nam));\n}', 'type': 'judgment'}
judgment Substitution::as_judgment() const {
  {
    std::vector<token> cs;
    cs.push_back(jud->shp->typ()->tok);

    mapping map;
    /*for(auto& s : judgment->shp) {
      if(s->typ) {
        uint8_t i = nam[x++];
        s = s->typ->mvs[i]->tok;
      }
    }*/
    shape shp = Shape::get_or_create(cs);
    naming nam = Naming::get_or_create(map);
    return Judgment::get_or_create(std::make_pair(shp, nam));
  }
} // {'body': '{ return std::make_pair(jud, args); }', 'type':
  // 'judgment_substitutions'}
judgment_substitutions Substitution::jud_args() const {
  { return std::make_pair(jud, args); }
}

substitution Substitution::create(const substitutions &args,
                                  const judgment &jud) {
  SubstitutionIndex::all_Substitutions.push_back({args, jud});
  substitution s{(int)SubstitutionIndex::all_Substitutions.size() - 1};

  return SubstitutionIndex::index(s);
}

substitution Substitution::get_or_create(const judgment_substitutions &x) {
  auto s = SubstitutionIndex::lookup_by_jud_args_index.find(x);
  if (s == SubstitutionIndex::lookup_by_jud_args_index.end())
    return create(x);
  return s->second;
}

substitution Substitution::get_if_exists(const judgment_substitutions &x) {
  auto s = SubstitutionIndex::lookup_by_jud_args(x);
  if (!s)
    return substitution::null;
  return s;
}

// {'getter': 'args', 'type': 'substitutions', 'iterable': True}
std::pair<SubstitutionIndex::lookup_by_args_index_iterator,
          SubstitutionIndex::lookup_by_args_index_iterator>
SubstitutionIndex::lookup_by_args(const substitutions &x) {
  return lookup_by_args_index.equal_range(x);
}

// {'getter': 'jud', 'type': 'judgment'}
std::pair<SubstitutionIndex::lookup_by_jud_index_iterator,
          SubstitutionIndex::lookup_by_jud_index_iterator>
SubstitutionIndex::lookup_by_jud(const judgment &x) {
  return lookup_by_jud_index.equal_range(x);
}

// {'getter': 'jud_args()', 'unique': True, 'type': 'judgment_substitutions'}
substitution
SubstitutionIndex::lookup_by_jud_args(const judgment_substitutions &x) {
  auto it = lookup_by_jud_args_index.find(x);
  if (it == lookup_by_jud_args_index.end())
    return substitution();
  return it->second;
}

substitution SubstitutionIndex::index(const substitution s) {
  const auto &obj = *s;
  for (const auto &obj_args : obj.args)
    lookup_by_args_index.emplace(obj_args, s);
  const auto &obj_jud = obj.jud;
  lookup_by_jud_index.emplace(obj_jud, s);
  const auto &obj_jud_args = obj.jud_args();
  lookup_by_jud_args_index[obj_jud_args] = s;
  return s;
}

std::ostream &operator<<(std::ostream &os, const Substitution &s) {
  os << "Substitution{";
  os << s.args;
  os << ", " << s.jud;
  return os << "}";
}

std::ostream &operator<<(std::ostream &os, const substitution &s) {
  return os << *(s) << "@" << s.i;
}
