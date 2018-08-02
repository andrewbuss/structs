#include <application.h>
#include <rule.h>

std::vector<Application> Application::all_applications{{}};

Application::lookup_by_rule_index_type Application::lookup_by_rule_index;

Application::lookup_by_result_index_type Application::lookup_by_result_index;

std::ostream &operator<<(std::ostream &os, const Application &a) {
  os << "Application{";
  os << a.via;
  os << ", " << a.condition_proofs;
  os << ", " << a.result;
  os << ", " << a.args;
  return os << "}";
}
