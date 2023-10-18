cmake_minimum_required(VERSION 3.22)
project(PhysicalMotor)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

# Common library
#file(GLOB_RECURSE COMMON_FILES common/include/*.h common/src/*.cpp)
#add_library(CommonLib ${COMMON_FILES})
#set_target_properties(CommonLib PROPERTIES LINKER_LANGUAGE CXX)
#target_include_directories(CommonLib PUBLIC common/include/)

file(GLOB_RECURSE Engine_Files engine/include/*.h engine/src/*.cpp)
add_library(Engine ${Engine_Files})
set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Engine PUBLIC engine/include/)
target_include_directories(Engine PUBLIC libs/Math/include/)

file(GLOB_RECURSE Common_Files graphics/include/*.h graphics/src/*.cpp)
add_library(graphics ${Common_Files})
set_target_properties(graphics PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Engine PUBLIC graphics/include/)
target_link_libraries(graphics PUBLIC Engine)
target_link_libraries(Engine PUBLIC fmt::fmt $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)

add_executable(Main Main.cpp)
target_link_libraries(Main graphics)

# Tests
SET(TEST_DIR ${CMAKE_SOURCE_DIR}/engineTests)
file(GLOB TEST_FILES ${TEST_DIR}/*.cpp )

foreach(test_file ${TEST_FILES} )
    # I used a simple string replace, to cut off .cpp.
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    # Make sure YourLib is linked to each app
    target_link_libraries(${test_name} PUBLIC Engine)
    target_link_libraries(${test_name} PRIVATE GTest::gtest GTest::gtest_main)

    IF(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 /Oi /Oy-")
    else()
        set_target_properties(${test_name} PROPERTIES COMPILE_FLAGS "-save-temps -march=haswell -ffast-math -fno-omit-frame-pointer")
    ENDIF()

    set_target_properties (${test_name} PROPERTIES FOLDER Courses)
endforeach()