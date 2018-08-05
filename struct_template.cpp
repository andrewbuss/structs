{%- set lname = name.lower() %}
{%- set plural = lname + 's' %}
{%- set abbr = lname[0] -%}
{%- set all_members = ', '.join(members.keys()) %}

#include <{{ lname }}.h>
{%- for include in includes %}
#include <{{ include }}>
{% endfor %}

std::vector<{{ name }}> all_{{ plural }}{ {} };
const {{ name }}* {{ lname }}::operator->() const { return all_{{ plural }}.data() + i; }
const {{ name }}& {{ lname }}::operator*() const { return all_{{ plural }}[i]; }

{% for lookup, lookup_spec in (lookups or {}).items() %}
{%- if lookup_spec.unique %}
std::unordered_map<{{ lookup_spec.type }}, {{ lname }}> {{ name }}Index::{{ lookup }}_index;
{% else %}
{{ name }}Index::{{ lookup }}_index_type {{ name }}Index::{{ lookup }}_index;
{% endif %}
{% endfor %}

{% for property, prop_spec in (properties or {}).items() -%}
// {{ prop_spec }}
{{ prop_spec.type }} {{ name }}::{{ property }}() const {
  {{ prop_spec.body }}
}
{%- endfor %}

{% set comma = joiner(", ") -%}
{{ lname }} {{ name }}::create(
  {%- for member, type in members.items() -%}
  {{ comma() }}const {{ type }}& {{ member }}
  {%- endfor -%}) {
  all_{{ plural }}.push_back({ {{ all_members }} });
  {{ lname }} {{ abbr }} = {(int)all_{{ plural }}.size() - 1};
  return {{ name }}Index::index({{ abbr }});
}

{% if unique_index %}
{{ lname }} {{ name }}::get_or_create(const {{ lookups[unique_index].type }}& x) {
  auto {{ abbr }} = {{ name }}Index::{{ unique_index }}(x);
  if (!{{ abbr }}) return create(x);
  return {{ abbr }};
}
{%- endif %}

{% for lookup, lookup_spec in (lookups or {}).items() %}
// {{ lookup_spec }}
{%- if lookup_spec.unique %}
{{ lname }} {{ name }}Index::{{ lookup }}(const {{ lookup_spec.type }}& x) {
  auto it = {{ lookup }}_index.find(x);
  if (it == {{ lookup }}_index.end())
    return {{ lname }}();
  return it->second;
}
{% else %}
std::pair<{{ name }}Index::{{ lookup }}_index_iterator,
          {{ name }}Index::{{ lookup }}_index_iterator>
{{ name }}Index::{{ lookup }}(const {{ lookup_spec.type }}& x) {
  return {{ lookup }}_index.equal_range(x);
}
{% endif %}
{% endfor %}

{{ lname }} {{ name }}Index::index(const {{ lname }} {{ abbr }}) {
  const auto& obj = *{{ abbr }};
  {%- for lookup, lookup_spec in (lookups or {}).items() -%}
  {%- set prop_var = 'obj_' + lookup_spec.getter.strip('()') -%}
  {% if lookup_spec.iterable %}
  for(const auto& {{ prop_var }} : obj.{{ lookup_spec.getter }})
    {% else %}
  const auto& {{ prop_var }} = obj.{{ lookup_spec.getter }};
  {% endif %}
  {%- if lookup_spec.unique -%}
  {{ lookup }}_index[{{ prop_var }}] = {{ abbr }};
  {%- else -%}
  {{ lookup }}_index.emplace({{prop_var }}, {{ abbr }});
  {%- endif %}
  {%- endfor %}
  return {{ abbr }};
}
/*
std::ostream& operator<<(std::ostream& os, const {{ name }}& {{ abbr }}) {
  os << "{{ name }}{";
  {% set comma = joiner(' ", " << ') -%}
  {%- for member, type in members.items() -%}
  os << {{ comma() }} {{ abbr }}.{{ member }};
  {%- endfor -%}
  return os << "}";
}
*/

/*
std::ostream& operator<<(std::ostream& os, const {{ lname }}& {{ abbr }}) {
  return os << {{ abbr }}.i;
}
*/
