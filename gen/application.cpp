

#include <application.h>

std::vector<Application> ApplicationIndex::all_Applications{{}};
const Application *application::operator->() const {
  return ApplicationIndex::all_Applications.data() + i;
}
const Application &application::operator*() const {
  return ApplicationIndex::all_Applications[i];
}

ApplicationIndex::lookup_by_rule_index_type
    ApplicationIndex::lookup_by_rule_index;

std::unordered_map<judgment, application>
    ApplicationIndex::lookup_by_hypothesis_index;

ApplicationIndex::lookup_by_result_index_type
    ApplicationIndex::lookup_by_result_index;

// {'body': '{ if(via) return 0; else return result; }', 'type': 'judgment'}
judgment Application::hypothesis_or_empty() const {
  {
    if (via)
      return 0;
    else
      return result;
  }
} // {'body': '{\n  std::unordered_set<judgment> rv;\n  for(const auto& [typ,
  // conditions_of_type]: conditions) {\n    for(const auto& c:
  // conditions_of_type) {\n      if(!c->via && c->conditions.size() == 0)
  // continue;\n      const auto& c_assumptions = c->assumptions();\n
  // rv.insert(c_assumptions.begin(), c_assumptions.end());\n    }\n  }\n return
  // rv;\n}', 'type': 'std::unordered_set<judgment>'}
std::unordered_set<judgment> Application::assumptions() const {
  {
    std::unordered_set<judgment> rv;
    for (const auto &[typ, conditions_of_type] : conditions) {
      for (const auto &c : conditions_of_type) {
        if (!c->via && c->conditions.size() == 0)
          continue;
        const auto &c_assumptions = c->assumptions();
        rv.insert(c_assumptions.begin(), c_assumptions.end());
      }
    }
    return rv;
  }
}

application Application::create(const rule &via, const Stack &conditions,
                                const judgment &result, const Stack &args) {
  ApplicationIndex::all_Applications.push_back({via, conditions, result, args});
  application a{(int)ApplicationIndex::all_Applications.size() - 1};

  return ApplicationIndex::index(a);
}

application Application::get_or_create(const judgment &x) {
  auto a = ApplicationIndex::lookup_by_hypothesis(x);
  if (!a)
    return create(x);
  return a;
}

application Application::get_if_exists(const judgment &x) {
  auto a = ApplicationIndex::lookup_by_hypothesis(x);
  if (!a)
    return application{0};
  return a;
}

// {'getter': 'via', 'type': 'rule'}
std::pair<ApplicationIndex::lookup_by_rule_index_iterator,
          ApplicationIndex::lookup_by_rule_index_iterator>
ApplicationIndex::lookup_by_rule(const rule &x) {
  return lookup_by_rule_index.equal_range(x);
}

// {'getter': 'hypothesis_or_empty()', 'unique': True, 'type': 'judgment'}
application ApplicationIndex::lookup_by_hypothesis(const judgment &x) {
  auto it = lookup_by_hypothesis_index.find(x);
  if (it == lookup_by_hypothesis_index.end())
    return application();
  return it->second;
}

// {'getter': 'result', 'type': 'judgment'}
std::pair<ApplicationIndex::lookup_by_result_index_iterator,
          ApplicationIndex::lookup_by_result_index_iterator>
ApplicationIndex::lookup_by_result(const judgment &x) {
  return lookup_by_result_index.equal_range(x);
}

application ApplicationIndex::index(const application a) {
  const auto &obj = *a;
  const auto &obj_via = obj.via;
  lookup_by_rule_index.emplace(obj_via, a);
  const auto &obj_hypothesis_or_empty = obj.hypothesis_or_empty();
  lookup_by_hypothesis_index[obj_hypothesis_or_empty] = a;
  const auto &obj_result = obj.result;
  lookup_by_result_index.emplace(obj_result, a);
  return a;
}

/*
std::ostream& operator<<(std::ostream& os, const Application& a) {
  os << "Application{";
  os <<  a.via;os <<  ", " <<  a.conditions;os <<  ", " <<  a.result;os <<  ", "
<<  a.args;return os << "}";
}
*/

/*
std::ostream& operator<<(std::ostream& os, const application& a) {
  return os << a.i;
}
*/
