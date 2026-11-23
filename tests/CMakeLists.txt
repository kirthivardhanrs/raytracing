file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(raytracing_tests ${TEST_SOURCES})

target_link_libraries(raytracing_tests
  PRIVATE
    raytracing_lib
    GTest::gtest_main   
)

set_property(TARGET raytracing_tests PROPERTY CXX_STANDARD 20)

if(MSVC)
  target_compile_options(raytracing_tests PRIVATE /wd4365)
endif()

include(GoogleTest)
gtest_discover_tests(raytracing_tests)