cmake_minimum_required(VERSION 3.31)

file(GLOB TESTS "*_test.cxx")
foreach(test_file IN LISTS TESTS)
    get_filename_component(name "${test_file}" NAME_WE)
endforeach()
