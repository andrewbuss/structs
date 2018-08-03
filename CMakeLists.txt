add_library(mmstruct gen/judgment.cpp gen/token.cpp gen/application.cpp gen/rule.cpp)
include_directories(PRIVATE include/)
include_directories(INTERFACE gen/)