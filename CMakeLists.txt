cmake_minimum_required(VERSION 3.15)
project(injector CXX)

find_package(PkgConfig)
pkg_check_modules(DLT REQUIRED automotive-dlt)

add_executable(dlt-injector src/main.cpp src/DltInjection.cpp)
target_compile_features(dlt-injector INTERFACE cxx_std_17)
target_link_libraries(dlt-injector ${DLT_LIBRARIES})
