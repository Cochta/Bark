cmake_minimum_required(VERSION 3.22)
project(Bark)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

option(ENABLE_ADDRESS_SANITIZER "Enable Address Sanitizer" OFF)
option(ENABLE_TRACY_PROFILER "Enable Tracy Profiler" OFF)





IF(ENABLE_TRACY_PROFILER)

    add_compile_definitions(TRACY_ENABLE)

    add_subdirectory(libs/tracy) 

    set_target_properties(TracyClient PROPERTIES FOLDER libs/)
    
endif()

IF(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 /Oi /Oy-")

    # Check if AddressSanitizer is enabled
    if(ENABLE_ADDRESS_SANITIZER)
        # Enable AddressSanitizer flags and options
        add_compile_options(/fsanitize=address /DEBUG)
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps -march=haswell -ffast-math")

    # Check if AddressSanitizer is enabled
    if(ENABLE_ADDRESS_SANITIZER)
        # Enable AddressSanitizer flags and options
        add_compile_options(-fsanitize=address /DEBUG)
    endif()
ENDIF()

# Common library
file(GLOB_RECURSE COMMON_FILES common/include/*.h common/src/*.cpp)
add_library(Common ${COMMON_FILES})
set_target_properties(Common PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Common PUBLIC common/include/)

# Common tests
file(GLOB COMMON_TEST_FILES common/tests/*.cpp )
foreach(test_file ${COMMON_TEST_FILES} )
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})

    target_link_libraries(${test_name} PUBLIC Common)
    target_link_libraries(${test_name} PRIVATE GTest::gtest GTest::gtest_main)

    IF(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 /Oi /Oy-")
    else()
        set_target_properties(${test_name} PROPERTIES COMPILE_FLAGS "-save-temps -march=haswell -ffast-math -fno-omit-frame-pointer")
    ENDIF()
endforeach()

# Engine library
file(GLOB_RECURSE ENGINE_FILES engine/include/*.h engine/src/*.cpp)
add_library(Engine ${ENGINE_FILES})
set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Engine PUBLIC engine/include/)
target_include_directories(Engine PUBLIC libs/Math/include/)
target_include_directories(Engine PUBLIC common/include/)

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

# Sample library
file(GLOB_RECURSE SAMPLE_FILES samples/include/*.h samples/src/*.cpp)
add_library(Samples ${SAMPLE_FILES})
set_target_properties(Samples PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Samples PUBLIC samples/include)
target_include_directories(Samples PUBLIC engine/include/)
target_include_directories(Samples PUBLIC libs/Math/include/)
target_include_directories(Samples PUBLIC common/include/)

# Graphics library
file(GLOB_RECURSE GRAPHICS_FILES graphics/include/*.h graphics/src/*.cpp)
add_library(Graphics ${GRAPHICS_FILES})
set_target_properties(Graphics PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Graphics PUBLIC graphics/include/)
target_include_directories(Graphics PUBLIC engine/include/)
target_include_directories(Graphics PUBLIC libs/Math/include/)
target_include_directories(Graphics PUBLIC common/include/)
target_include_directories(Graphics PUBLIC samples/include/)

target_link_libraries(Graphics PUBLIC Samples Engine Common $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main> $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static> imgui::imgui)
IF(TRACY_ENABLE)
    target_compile_definitions(Graphics PUBLIC TRACY_ENABLE)
    target_link_libraries(Graphics PUBLIC TracyClient)
endif()

add_executable(Main Main.cpp)
target_link_libraries(Main Graphics)