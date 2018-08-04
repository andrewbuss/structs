

#include <application.h>
#include <rule.h>

#include <list>

std::vector<Application> all_applications{{}};
const Application *application::operator->() const {
  return all_applications.data() + i;
}
const Application &application::operator*() const {
  return all_applications[i];
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
}

application
Application::create(const rule &via,
                    const std::unordered_set<application> &condition_proofs,
                    const judgment &result, const std::vector<judgment> &args) {
  all_applications.push_back({via, condition_proofs, result, args});
  application a = {(int)all_applications.size() - 1};
  return ApplicationIndex::index(a);
}

application Application::get_or_create(const judgment &x) {
  auto a = ApplicationIndex::lookup_by_hypothesis(x);
  std::cout << a << std::endl;
  if (!a)
    return create(x);
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

std::ostream &operator<<(std::ostream &os, const Application &a) {
  os << "Application{";
  os << a.via;
  os << ", " << a.condition_proofs;
  os << ", " << a.result;
  os << ", " << a.args;
  return os << "}";
}

std::ostream &operator<<(std::ostream &os, const application &a) {
  return os << a.i;
}
