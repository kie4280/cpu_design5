cmake_minimum_required(VERSION 3.0.0)
project(cpu_design5 VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB cpps "src/*.cpp")
file(GLOB headers "src/*.h")

add_executable(cpu_design5 ${cpps} ${headers})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
