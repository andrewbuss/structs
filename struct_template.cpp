{%- set lname = name.lower() %}
{%- set plural = lname + 's' %}
{%- set abbr = lname[0] %}

#include <{{ lname }}.h>
{% for include in includes %}
#include <{{ include }}>
{% endfor %}

std::vector<{{ name }}> {{ name }}::all_{{ plural }}{ {} };
{% for lookup, lookup_spec in (lookups or {}).items() %}
{%- if lookup_spec.unique %}
std::unordered_map<{{ lookup_spec.type }}, {{ lname }}> {{ name }}::{{ lookup }}_index;
{% else %}
{{ name }}::{{ lookup }}_index_type {{ name }}::{{ lookup }}_index;
{% endif %}
{% endfor %}

std::ostream& operator<<(std::ostream& os, const {{ name }}& {{ abbr }})
{
  os << "{{ name }}{";
  {% set comma = joiner(' ", " << ') -%}
  {%- for member, type in members.items() -%}
  os << {{ comma() }} {{ abbr }}.{{ member }};
  {%- endfor -%}
  return os << "}";
}
