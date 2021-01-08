# Allow the source files to find headers in src/
include(GNUInstallDirs)
include_directories(${PROJECT_SOURCE_DIR}/src)

if (DEFINED BENCHMARK_CXX_LINKER_FLAGS)
  list(APPEND CMAKE_SHARED_LINKER_FLAGS ${BENCHMARK_CXX_LINKER_FLAGS})
  list(APPEND CMAKE_MODULE_LINKER_FLAGS ${BENCHMARK_CXX_LINKER_FLAGS})
endif()

file(GLOB
  SOURCE_FILES
    *.cc
    ${PROJECT_SOURCE_DIR}/include/easydev/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB BENCHMARK_MAIN "easydev_main.cc")
foreach(item ${BENCHMARK_MAIN})
  list(REMOVE_ITEM SOURCE_FILES "${item}")
endforeach()

add_library(easydev ${SOURCE_FILES})
add_library(easydev::easydev ALIAS easydev)
set_target_properties(easydev PROPERTIES
  OUTPUT_NAME "easydev"
  VERSION ${GENERIC_LIB_VERSION}
  SOVERSION ${GENERIC_LIB_SOVERSION}
)
target_include_directories(easydev PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    )

# Link threads.
target_link_libraries(easydev  ${BENCHMARK_CXX_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
find_library(LIBRT rt)
if(LIBRT)
  target_link_libraries(easydev ${LIBRT})
endif()

if(CMAKE_BUILD_TYPE)
  string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UPPER)
endif()
if(NOT CMAKE_THREAD_LIBS_INIT AND "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE_UPPER}}" MATCHES ".*-fsanitize=[^ ]*address.*")
  message(WARNING "CMake's FindThreads.cmake did not fail, but CMAKE_THREAD_LIBS_INIT ended up being empty. This was fixed in https://github.com/Kitware/CMake/commit/d53317130e84898c5328c237186dbd995aaf1c12 Let's guess that -pthread is sufficient.")
  target_link_libraries(easydev -pthread)
endif()

# We need extra libraries on Windows
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(easydev shlwapi)
endif()

# We need extra libraries on Solaris
if(${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
  target_link_libraries(easydev kstat)
endif()

# Benchmark main library
add_library(easydev_main "easydev_main.cc")
add_library(easydev::easydev_main ALIAS easydev_main)
set_target_properties(easydev_main PROPERTIES
  OUTPUT_NAME "easydev_main"
  VERSION ${GENERIC_LIB_VERSION}
  SOVERSION ${GENERIC_LIB_SOVERSION}
)
target_include_directories(easydev PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    )
target_link_libraries(easydev_main easydev::easydev)


set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(pkg_config "${generated_dir}/${PROJECT_NAME}.pc")
set(targets_export_name "${PROJECT_NAME}Targets")

set(namespace "${PROJECT_NAME}::")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${version_config}" VERSION ${GENERIC_LIB_VERSION} COMPATIBILITY SameMajorVersion
)

configure_file("${PROJECT_SOURCE_DIR}/cmake/Config.cmake.in" "${project_config}" @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/cmake/easydev.pc.in" "${pkg_config}" @ONLY)

if (BENCHMARK_ENABLE_INSTALL)
  # Install target (will install the library to specified CMAKE_INSTALL_PREFIX variable)
  install(
    TARGETS easydev easydev_main
    EXPORT ${targets_export_name}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/include/easydev"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.*h")

  install(
      FILES "${project_config}" "${version_config}"
      DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

  install(
      FILES "${pkg_config}"
      DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

  install(
      EXPORT "${targets_export_name}"
      NAMESPACE "${namespace}"
      DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
endif()
