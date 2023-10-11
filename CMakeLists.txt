cmake_minimum_required(VERSION 3.22)
project(PhysicalMotor)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

file(GLOB_RECURSE HEADER_Engine common/engine/include/*.h)
file(GLOB_RECURSE SRC_Engine common/engine/src/*.cpp)
add_library(Engine ${SRC_Engine} ${HEADER_Engine})
set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(Engine PUBLIC fmt::fmt $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
target_include_directories(Engine PUBLIC common/engine/include/)

add_subdirectory(libs/Math)
target_include_directories(Engine PUBLIC libs/Math/include/)
# Tests
SET(TEST_DIR ${CMAKE_SOURCE_DIR}/engineTests)
file(GLOB TEST_FILES ${TEST_DIR}/*.cpp )

foreach(test_file ${TEST_FILES} )
    # I used a simple string replace, to cut off .cpp.
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    # Make sure YourLib is linked to each app
    target_link_libraries(${test_name} PUBLIC common)
    target_link_libraries(${test_name} PRIVATE GTest::gtest GTest::gtest_main)

    IF(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 /Oi /Oy-")
    else()
        set_target_properties(${test_name} PROPERTIES COMPILE_FLAGS "-save-temps -march=haswell -ffast-math -fno-omit-frame-pointer")
    ENDIF()

    set_target_properties (${test_name} PROPERTIES FOLDER Courses)
endforeach()

# Benchmarks
file(GLOB BENCH_FILES benchmarks/*.cpp)

foreach(BENCH_FILE ${BENCH_FILES})
    get_filename_component(BENCH_NAME ${BENCH_FILE} NAME_WE)
    target_link_libraries(${BENCH_NAME} PUBLIC common)
    target_link_libraries(${BENCH_NAME} PRIVATE benchmark::benchmark benchmark::benchmark_main)
    target_include_directories(${BENCH_NAME} PRIVATE common/engine/include/)
    if(MSVC)
        target_compile_definitions(${BENCH_NAME} PUBLIC "_USE_MATH_DEFINES")
        target_compile_options(${BENCH_NAME} PUBLIC /arch:AVX2 /Oi /GR- /EHs-c- /FA /Oy- /GL)
        target_link_options(${BENCH_NAME} PUBLIC /LTCG)
    else()
        target_compile_options(${BENCH_NAME} PUBLIC "-march=haswell" "-masm=intel" -fno-rtti -save-temps
                -fno-omit-frame-pointer -flto -ffast-math)
        target_link_options(${BENCH_NAME} PUBLIC -flto)
    endif()

    set_target_properties (${BENCH_NAME} PROPERTIES FOLDER Bench)
endforeach(BENCH_FILE ${BENCH_FILES})

add_executable(Main common/Main.cpp)
target_link_libraries(Main Engine)
