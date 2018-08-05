CXXFLAGS=-Igen/ -Iinclude/
.PRECIOUS: gen/%.cpp gen/%.h
main: gen/token.o gen/judgment.o gen/rule.o gen/application.o gen/metavar.o

gen/:
	mkdir gen

gen/%.h: structs/%.yaml gen/
	python2 jinjafy.py struct_template.h $< > $@
	clang-format -i $@

gen/%.cpp: structs/%.yaml gen/%.h
	python2 jinjafy.py struct_template.cpp $< > $@
	clang-format -i $@
