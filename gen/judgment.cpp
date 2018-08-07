

#include <judgment.h>
#include <token.h>

std::vector<Judgment> JudgmentIndex::all_Judgments{{}};
const Judgment *judgment::operator->() const {
  return JudgmentIndex::all_Judgments.data() + i;
}
const Judgment &judgment::operator*() const {
  return JudgmentIndex::all_Judgments[i];
}

JudgmentIndex::lookup_by_naming_index_type
    JudgmentIndex::lookup_by_naming_index;

JudgmentIndex::lookup_by_shape_index_type JudgmentIndex::lookup_by_shape_index;

std::unordered_map<std::vector<token>, judgment>
    JudgmentIndex::lookup_by_cs_index;

JudgmentIndex::lookup_by_type_index_type JudgmentIndex::lookup_by_type_index;

JudgmentIndex::lookup_by_token_index_type JudgmentIndex::lookup_by_token_index;

// {'body': '{ if(cs.size() > 0) return cs[0]; else return token(); }', 'type':
// 'token'}
token Judgment::type() const {
  {
    if (cs.size() > 0)
      return cs[0];
    else
      return token();
  }
}

judgment Judgment::create(const std::vector<token> &cs, const shape &s,
                          const naming &n) {
  JudgmentIndex::all_Judgments.push_back({cs, s, n});
  judgment j = {(int)JudgmentIndex::all_Judgments.size() - 1};
  return JudgmentIndex::index(j);
}

judgment Judgment::get_or_create(const std::vector<token> &x) {
  auto j = JudgmentIndex::lookup_by_cs(x);
  if (!j)
    return create(x);
  return j;
}

// {'getter': 'n', 'type': 'naming'}
std::pair<JudgmentIndex::lookup_by_naming_index_iterator,
          JudgmentIndex::lookup_by_naming_index_iterator>
JudgmentIndex::lookup_by_naming(const naming &x) {
  return lookup_by_naming_index.equal_range(x);
}

// {'getter': 's', 'type': 'shape'}
std::pair<JudgmentIndex::lookup_by_shape_index_iterator,
          JudgmentIndex::lookup_by_shape_index_iterator>
JudgmentIndex::lookup_by_shape(const shape &x) {
  return lookup_by_shape_index.equal_range(x);
}

// {'getter': 'cs', 'unique': True, 'type': 'std::vector<token>'}
judgment JudgmentIndex::lookup_by_cs(const std::vector<token> &x) {
  auto it = lookup_by_cs_index.find(x);
  if (it == lookup_by_cs_index.end())
    return judgment();
  return it->second;
}

// {'getter': 'type()', 'type': 'token'}
std::pair<JudgmentIndex::lookup_by_type_index_iterator,
          JudgmentIndex::lookup_by_type_index_iterator>
JudgmentIndex::lookup_by_type(const token &x) {
  return lookup_by_type_index.equal_range(x);
}

// {'getter': 'cs', 'type': 'token', 'iterable': True}
std::pair<JudgmentIndex::lookup_by_token_index_iterator,
          JudgmentIndex::lookup_by_token_index_iterator>
JudgmentIndex::lookup_by_token(const token &x) {
  return lookup_by_token_index.equal_range(x);
}

judgment JudgmentIndex::index(const judgment j) {
  const auto &obj = *j;
  const auto &obj_n = obj.n;
  lookup_by_naming_index.emplace(obj_n, j);
  const auto &obj_s = obj.s;
  lookup_by_shape_index.emplace(obj_s, j);
  const auto &obj_cs = obj.cs;
  lookup_by_cs_index[obj_cs] = j;
  const auto &obj_type = obj.type();
  lookup_by_type_index.emplace(obj_type, j);
  for (const auto &obj_cs : obj.cs)
    lookup_by_token_index.emplace(obj_cs, j);
  return j;
}

/*
std::ostream& operator<<(std::ostream& os, const Judgment& j) {
  os << "Judgment{";
  os <<  j.cs;os <<  ", " <<  j.s;os <<  ", " <<  j.n;return os << "}";
}
*/

/*
std::ostream& operator<<(std::ostream& os, const judgment& j) {
  return os << j.i;
}
*/
