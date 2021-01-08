# Enable the tests

find_package(Threads REQUIRED)
include(CheckCXXCompilerFlag)

# NOTE: Some tests use `<cassert>` to perform the test. Therefore we must
# strip -DNDEBUG from the default CMake flags in DEBUG mode.
string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
if( NOT uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" )
  add_definitions( -UNDEBUG )
  add_definitions(-DTEST_BENCHMARK_LIBRARY_HAS_NO_ASSERTIONS)
  # Also remove /D NDEBUG to avoid MSVC warnings about conflicting defines.
  foreach (flags_var_to_scrub
      CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_RELWITHDEBINFO
      CMAKE_CXX_FLAGS_MINSIZEREL
      CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_RELWITHDEBINFO
      CMAKE_C_FLAGS_MINSIZEREL)
    string (REGEX REPLACE "(^| )[/-]D *NDEBUG($| )" " "
      "${flags_var_to_scrub}" "${${flags_var_to_scrub}}")
  endforeach()
endif()
file( GLOB TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cc" )
check_cxx_compiler_flag(-O3 BENCHMARK_HAS_O3_FLAG)
set(BENCHMARK_O3_FLAG "")
if (BENCHMARK_HAS_O3_FLAG)
  set(BENCHMARK_O3_FLAG "-O3")
endif()

# NOTE: These flags must be added after find_package(Threads REQUIRED) otherwise
# they will break the configuration check.
if (DEFINED BENCHMARK_CXX_LINKER_FLAGS)
  list(APPEND CMAKE_EXE_LINKER_FLAGS ${BENCHMARK_CXX_LINKER_FLAGS})
endif()

macro(compile_easydev_test name)
  add_executable(${name} "${name}.cc")
  target_link_libraries(${name} easydev benchmark fmt ${CMAKE_THREAD_LIBS_INIT})
endmacro(compile_easydev_test)

# Demonstration executable
foreach (FILE_NAME ${TEST_SRC})
	STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FILE_NAME})
	compile_easydev_test(${FILE_NAME})
	add_test(NAME test_${FILE_NAME} COMMAND ${FILE_NAME} --benchmark_min_time=0.01)
endforeach()
# add_filter_test(filter_simple "Foo" 3)
# add_filter_test(filter_simple_negative "-Foo" 2)
# add_filter_test(filter_suffix "BM_.*" 4)
# add_filter_test(filter_suffix_negative "-BM_.*" 1)
# add_filter_test(filter_regex_all ".*" 5)
# add_filter_test(filter_regex_all_negative "-.*" 0)
# add_filter_test(filter_regex_blank "" 5)
# add_filter_test(filter_regex_blank_negative "-" 0)
# add_filter_test(filter_regex_none "monkey" 0)
# add_filter_test(filter_regex_none_negative "-monkey" 5)
# add_filter_test(filter_regex_wildcard ".*Foo.*" 3)
# add_filter_test(filter_regex_wildcard_negative "-.*Foo.*" 2)
# add_filter_test(filter_regex_begin "^BM_.*" 4)
# add_filter_test(filter_regex_begin_negative "-^BM_.*" 1)
# add_filter_test(filter_regex_begin2 "^N" 1)
# add_filter_test(filter_regex_begin2_negative "-^N" 4)
# add_filter_test(filter_regex_end ".*Ba$" 1)
# add_filter_test(filter_regex_end_negative "-.*Ba$" 4)


check_cxx_compiler_flag(-std=c++03 BENCHMARK_HAS_CXX03_FLAG)


# Attempt to work around flaky test failures when running on Appveyor servers.
if (DEFINED ENV{APPVEYOR})
  set(COMPLEXITY_MIN_TIME "0.5")
else()
  set(COMPLEXITY_MIN_TIME "0.01")
endif()



###############################################################################
# Code Coverage Configuration
###############################################################################

# Add the coverage command(s)
if(CMAKE_BUILD_TYPE)
  string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWER)
endif()
if (${CMAKE_BUILD_TYPE_LOWER} MATCHES "coverage")
  find_program(GCOV gcov)
  find_program(LCOV lcov)
  find_program(GENHTML genhtml)
  find_program(CTEST ctest)
  if (GCOV AND LCOV AND GENHTML AND CTEST AND HAVE_CXX_FLAG_COVERAGE)
    add_custom_command(
      OUTPUT ${CMAKE_BINARY_DIR}/lcov/index.html
      COMMAND ${LCOV} -q -z -d .
      COMMAND ${LCOV} -q --no-external -c -b "${CMAKE_SOURCE_DIR}" -d . -o before.lcov -i
      COMMAND ${CTEST} --force-new-ctest-process
      COMMAND ${LCOV} -q --no-external -c -b "${CMAKE_SOURCE_DIR}" -d . -o after.lcov
      COMMAND ${LCOV} -q -a before.lcov -a after.lcov --output-file final.lcov
      COMMAND ${LCOV} -q -r final.lcov "'${CMAKE_SOURCE_DIR}/test/*'" -o final.lcov
      COMMAND ${GENHTML} final.lcov -o lcov --demangle-cpp --sort -p "${CMAKE_BINARY_DIR}" -t benchmark
      DEPENDS filter_test benchmark_test options_test basic_test fixture_test cxx03_test complexity_test
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMENT "Running LCOV"
    )
    add_custom_target(coverage
      DEPENDS ${CMAKE_BINARY_DIR}/lcov/index.html
      COMMENT "LCOV report at lcov/index.html"
    )
    message(STATUS "Coverage command added")
  else()
    if (HAVE_CXX_FLAG_COVERAGE)
      set(CXX_FLAG_COVERAGE_MESSAGE supported)
    else()
      set(CXX_FLAG_COVERAGE_MESSAGE unavailable)
    endif()
    message(WARNING
      "Coverage not available:\n"
      "  gcov: ${GCOV}\n"
      "  lcov: ${LCOV}\n"
      "  genhtml: ${GENHTML}\n"
      "  ctest: ${CTEST}\n"
      "  --coverage flag: ${CXX_FLAG_COVERAGE_MESSAGE}")
  endif()
endif()
