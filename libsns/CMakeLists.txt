cmake_minimum_required(VERSION 2.8)
project(gamebaby)


set(CMAKE_VERBOSE_MAKEFILE ON)


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set(CCACHE_CMD "CCACHE_DIR=/tmp/ccachedir ccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_CMD}")
endif(CCACHE_FOUND)


set(SOURCES
  sns_sha256.cpp
  sns_tester.cpp
  sns_record.cpp
  sns_account.cpp
)


set(CMAKE_CXX_FLAGS "-std=c++17 -Og -O0 -Werror")


include_directories(./ ../)
add_library("sns" ${SOURCES})
add_executable("test_sns" "test_sns.cpp")
add_executable("test_sha256" "test_sha256.cpp")
target_link_libraries("test_sns" "sns")
target_link_libraries("test_sha256" "sns")




