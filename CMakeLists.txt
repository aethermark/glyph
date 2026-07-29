cmake_minimum_required(VERSION 3.31)

project(
    Glyph
    VERSION 0.1.0
    DESCRIPTION "A software font rasterization library"
    LANGUAGES CXX
)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(GLYPH_IS_TOP_LEVEL ON)
else()
  set(GLYPH_IS_TOP_LEVEL OFF)
endif()

if(GLYPH_IS_TOP_LEVEL)
    include(CTest)
else()
    set(BUILD_TESTING OFF)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Options
# ============================================================================

option(GLYPH_BUILD_PLAYGROUND "Build playground" ${GLYPH_IS_TOP_LEVEL})
option(GLYPH_BUILD_BENCHMARKS "Build benchmarks" ${GLYPH_IS_TOP_LEVEL})
option(GLYPH_ENABLE_COVERAGE "Enable coverage instrumentation" ${GLYPH_IS_TOP_LEVEL})
option(GLYPH_BUILD_STATIC_ANALYSIS "Build the static analysis translation unit" ${GLYPH_IS_TOP_LEVEL})

# ============================================================================
# Library
# ============================================================================

add_library(glyph STATIC)

add_library(glyph::glyph ALIAS glyph)

file(GLOB_RECURSE GLYPH_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

target_sources(glyph
    PRIVATE
      ${GLYPH_SOURCES}
)

target_include_directories(glyph
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(glyph
    PUBLIC
        cxx_std_26
)

set_target_properties(glyph PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# ============================================================================
# Static analysis translation unit
# ============================================================================

if(GLYPH_BUILD_STATIC_ANALYSIS)

    file(
      GLOB_RECURSE GLYPH_PUBLIC_HEADERS
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    )

    set(HEADER_INCLUDES "")

    foreach(header ${GLYPH_PUBLIC_HEADERS})
        file(
            RELATIVE_PATH rel
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
            "${header}"
        )

        string(APPEND HEADER_INCLUDES "#include <${rel}>\n")
    endforeach()

    configure_file(
        cmake/static_analysis.cpp.in
        "${CMAKE_CURRENT_BINARY_DIR}/static_analysis.cpp"
        @ONLY
    )

    add_library(glyph-static-analysis OBJECT
        "${CMAKE_CURRENT_BINARY_DIR}/static_analysis.cpp"
    )

    target_include_directories(glyph-static-analysis
          PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    target_link_libraries(glyph-static-analysis
        PRIVATE glyph
    )

endif()

# ============================================================================
# Dependencies
# ============================================================================

set(PRINTER_BUILD_PLAYGROUND OFF CACHE BOOL "" FORCE)
set(PRINTER_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(PRINTER_ENABLE_COVERAGE OFF CACHE BOOL "" FORCE)

if (NOT TARGET printer::printer)
  add_subdirectory(third_party/printer)
endif()

target_link_libraries(glyph
  PUBLIC
      printer::printer
)

# ============================================================================
# Coverage
# ============================================================================

if(GLYPH_IS_TOP_LEVEL AND GLYPH_ENABLE_COVERAGE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  include(EnableCoverage)
  enable_coverage(glyph)
endif()

# ============================================================================
# Subdirectories
# ============================================================================

if(GLYPH_BUILD_PLAYGROUND)
    add_subdirectory(examples/playground)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(GLYPH_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

# ============================================================================
# Installation
# ============================================================================

install(
    TARGETS glyph
    EXPORT GlyphTargets

    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT GlyphTargets
    FILE GlyphTargets.cmake
    NAMESPACE glyph::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Glyph
)

# ============================================================================
# Package Configuration
# ============================================================================

configure_package_config_file(
    cmake/GlyphConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/GlyphConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Glyph
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/GlyphConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/GlyphConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/GlyphConfigVersion.cmake
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/Glyph
)
