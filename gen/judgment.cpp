

#include <judgment.h>

std::vector<Judgment> JudgmentIndex::all_Judgments{{}};
const Judgment *judgment::operator->() const {
  return JudgmentIndex::all_Judgments.data() + i;
}
const Judgment &judgment::operator*() const {
  return JudgmentIndex::all_Judgments[i];
}
judgment judgment::null = 0;

JudgmentIndex::lookup_by_naming_index_type
    JudgmentIndex::lookup_by_naming_index;

JudgmentIndex::lookup_by_shape_index_type JudgmentIndex::lookup_by_shape_index;

std::unordered_map<shape_naming, judgment>
    JudgmentIndex::lookup_by_shp_nam_index;

// {'body': '{\n  std::vector<token> rv(shp->shp);\n  int x = 0;\n  bool first =
// true;\n  for(auto& s : rv) {\n    if(first) {\n      first = false;\n
// continue;\n    }\n    if(s->typ) {\n      uint8_t i = nam->map[x++];\n      s
// = s->typ->mvs[i]->tok;\n    }\n  }\n  return Shape::get_or_create(rv);\n}',
// 'type': 'shape'}
shape Judgment::flattened() const {
  {
    std::vector<token> rv(shp->shp);
    int x = 0;
    bool first = true;
    for (auto &s : rv) {
      if (first) {
        first = false;
        continue;
      }
      if (s->typ) {
        uint8_t i = nam->map[x++];
        s = s->typ->mvs[i]->tok;
      }
    }
    return Shape::get_or_create(rv);
  }
} // {'body': '{ return std::make_pair(shp, nam); }', 'type': 'shape_naming'}
shape_naming Judgment::shp_nam() const {
  { return std::make_pair(shp, nam); }
}

judgment Judgment::create(const shape &shp, const naming &nam) {
  JudgmentIndex::all_Judgments.push_back({shp, nam});
  judgment j{(int)JudgmentIndex::all_Judgments.size() - 1};

  return JudgmentIndex::index(j);
}

judgment Judgment::get_or_create(const shape_naming &x) {
  auto j = JudgmentIndex::lookup_by_shp_nam_index.find(x);
  if (j == JudgmentIndex::lookup_by_shp_nam_index.end())
    return create(x);
  return j->second;
}

judgment Judgment::get_if_exists(const shape_naming &x) {
  auto j = JudgmentIndex::lookup_by_shp_nam(x);
  if (!j)
    return judgment::null;
  return j;
}

// {'getter': 'nam', 'type': 'naming'}
std::pair<JudgmentIndex::lookup_by_naming_index_iterator,
          JudgmentIndex::lookup_by_naming_index_iterator>
JudgmentIndex::lookup_by_naming(const naming &x) {
  return lookup_by_naming_index.equal_range(x);
}

// {'getter': 'shp', 'type': 'shape'}
std::pair<JudgmentIndex::lookup_by_shape_index_iterator,
          JudgmentIndex::lookup_by_shape_index_iterator>
JudgmentIndex::lookup_by_shape(const shape &x) {
  return lookup_by_shape_index.equal_range(x);
}

// {'getter': 'shp_nam()', 'unique': True, 'type': 'shape_naming'}
judgment JudgmentIndex::lookup_by_shp_nam(const shape_naming &x) {
  auto it = lookup_by_shp_nam_index.find(x);
  if (it == lookup_by_shp_nam_index.end())
    return judgment();
  return it->second;
}

judgment JudgmentIndex::index(const judgment j) {
  const auto &obj = *j;
  const auto &obj_nam = obj.nam;
  lookup_by_naming_index.emplace(obj_nam, j);
  const auto &obj_shp = obj.shp;
  lookup_by_shape_index.emplace(obj_shp, j);
  const auto &obj_shp_nam = obj.shp_nam();
  lookup_by_shp_nam_index[obj_shp_nam] = j;
  return j;
}

std::ostream &operator<<(std::ostream &os, const Judgment &j) {
  os << "Judgment{";
  os << j.shp;
  os << ", " << j.nam;
  return os << "}";
}

std::ostream &operator<<(std::ostream &os, const judgment &j) {
  return os << *(j) << "@" << j.i;
}
