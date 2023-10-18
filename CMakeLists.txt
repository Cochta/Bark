cmake_minimum_required(VERSION 3.22)
project(PhysicalMotor)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest CONFIG REQUIRED)

find_package(SDL2 CONFIG REQUIRED)

 #Common library
file(GLOB_RECURSE COMMON_FILES common/include/*.h common/src/*.cpp)
add_library(Common ${COMMON_FILES})
set_target_properties(Common PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Common PUBLIC common/include/)

# Common tests
file(GLOB COMMON_TEST_FILES common/tests/*.cpp )

foreach(test_file ${COMMON_TEST_FILES} )
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})

    target_link_libraries(${test_name} PRIVATE GTest::gtest GTest::gtest_main)

    IF(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 /Oi /Oy-")
    else()
        set_target_properties(${test_name} PROPERTIES COMPILE_FLAGS "-save-temps -march=haswell -ffast-math -fno-omit-frame-pointer")
    ENDIF()
endforeach()

file(GLOB_RECURSE ENGINE_FILES engine/include/*.h engine/src/*.cpp)
add_library(Engine ${ENGINE_FILES})
set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Engine PUBLIC engine/include/)
target_include_directories(Engine PUBLIC libs/Math/include/)

# Engine tests

file(GLOB ENGINE_TEST_FILES engine/tests/*.cpp )

foreach(test_file ${ENGINE_TEST_FILES} )
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})

    target_link_libraries(${test_name} PUBLIC Engine)
    target_link_libraries(${test_name} PRIVATE GTest::gtest GTest::gtest_main)

    IF(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 /Oi /Oy-")
    else()
        set_target_properties(${test_name} PROPERTIES COMPILE_FLAGS "-save-temps -march=haswell -ffast-math -fno-omit-frame-pointer")
    ENDIF()
endforeach()

file(GLOB_RECURSE GRAPHICS_FILES graphics/src/*.cpp graphics/include/*.h)
add_library(Graphics ${GRAPHICS_FILES})
target_include_directories(Graphics PUBLIC graphics/include/)
target_include_directories(Graphics PUBLIC engine/include/)
target_include_directories(Graphics PUBLIC libs/Math/include/)
target_include_directories(Graphics PUBLIC common/include/)
target_link_libraries(Graphics PUBLIC Engine Common $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)

add_executable(Main Main.cpp)
target_link_libraries(Main Graphics)


