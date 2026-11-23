cmake_minimum_required(VERSION 3.20)

project(raytracing)

if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


file(GLOB_RECURSE RAYTRACING_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)

list(FILTER RAYTRACING_SOURCES EXCLUDE REGEX ".*raytracing\\.cpp$")

add_library(raytracing_lib STATIC ${RAYTRACING_SOURCES})

target_include_directories(raytracing_lib PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

target_compile_features(raytracing_lib PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(raytracing_lib PRIVATE /W4 /WX /w44365)
else()
    target_compile_options(raytracing_lib PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(raytracing "src/raytracing.cpp")

target_link_libraries(raytracing PRIVATE raytracing_lib)

set_property(TARGET raytracing PROPERTY CXX_STANDARD 20)

# tests

option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_subdirectory(tests)
endif()