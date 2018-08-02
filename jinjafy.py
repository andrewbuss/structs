from jinja2 import Template
import yaml
import sys

template = Template(open(sys.argv[1]).read())
for definition in sys.argv[2:]:
    print template.render(**yaml.load(open(definition)))
