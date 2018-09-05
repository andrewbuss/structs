

#include <axiom.h>

std::vector<Axiom> AxiomIndex::all_Axioms{{}};
const Axiom *axiom::operator->() const {
  return AxiomIndex::all_Axioms.data() + i;
}
const Axiom &axiom::operator*() const { return AxiomIndex::all_Axioms[i]; }
axiom axiom::null = 0;

AxiomIndex::lookup_by_sub_index_type AxiomIndex::lookup_by_sub_index;

std::unordered_map<substitution_substitutions, axiom>
    AxiomIndex::lookup_by_sub_hyps_index;

AxiomIndex::lookup_by_hyp_index_type AxiomIndex::lookup_by_hyp_index;

// {'body': '{ return std::make_pair(sub, hyps); }', 'type':
// 'substitution_substitutions'}
substitution_substitutions Axiom::sub_hyps() const {
  { return std::make_pair(sub, hyps); }
}

axiom Axiom::create(const substitution &sub, const substitutions &hyps) {
  AxiomIndex::all_Axioms.push_back({sub, hyps});
  axiom a{(int)AxiomIndex::all_Axioms.size() - 1};

  return AxiomIndex::index(a);
}

axiom Axiom::get_or_create(const substitution_substitutions &x) {
  auto a = AxiomIndex::lookup_by_sub_hyps_index.find(x);
  if (a == AxiomIndex::lookup_by_sub_hyps_index.end())
    return create(x);
  return a->second;
}

axiom Axiom::get_if_exists(const substitution_substitutions &x) {
  auto a = AxiomIndex::lookup_by_sub_hyps(x);
  if (!a)
    return axiom::null;
  return a;
}

// {'getter': 'sub', 'type': 'substitution'}
std::pair<AxiomIndex::lookup_by_sub_index_iterator,
          AxiomIndex::lookup_by_sub_index_iterator>
AxiomIndex::lookup_by_sub(const substitution &x) {
  return lookup_by_sub_index.equal_range(x);
}

// {'getter': 'sub_hyps()', 'unique': True, 'type':
// 'substitution_substitutions'}
axiom AxiomIndex::lookup_by_sub_hyps(const substitution_substitutions &x) {
  auto it = lookup_by_sub_hyps_index.find(x);
  if (it == lookup_by_sub_hyps_index.end())
    return axiom();
  return it->second;
}

// {'getter': 'hyps', 'type': 'substitution', 'iterable': True}
std::pair<AxiomIndex::lookup_by_hyp_index_iterator,
          AxiomIndex::lookup_by_hyp_index_iterator>
AxiomIndex::lookup_by_hyp(const substitution &x) {
  return lookup_by_hyp_index.equal_range(x);
}

axiom AxiomIndex::index(const axiom a) {
  const auto &obj = *a;
  const auto &obj_sub = obj.sub;
  lookup_by_sub_index.emplace(obj_sub, a);
  const auto &obj_sub_hyps = obj.sub_hyps();
  lookup_by_sub_hyps_index[obj_sub_hyps] = a;
  for (const auto &obj_hyps : obj.hyps)
    lookup_by_hyp_index.emplace(obj_hyps, a);
  return a;
}

std::ostream &operator<<(std::ostream &os, const Axiom &a) {
  os << "Axiom{";
  os << a.sub;
  os << ", " << a.hyps;
  return os << "}";
}

std::ostream &operator<<(std::ostream &os, const axiom &a) {
  return os << *(a) << "@" << a.i;
}
