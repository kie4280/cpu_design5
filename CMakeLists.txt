cmake_minimum_required(VERSION 3.0.0)
project(cpu_design5 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(cpu_design5 src/direct_mapped_cache_lru.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
