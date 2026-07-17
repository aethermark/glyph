cmake_minimum_required(VERSION 3.31)

project(
    Glyph
    VERSION 0.1.0
    DESCRIPTION "A software font rasterization library"
    LANGUAGES CXX
)

include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Options
# ============================================================================

option(GLYPH_BUILD_PLAYGROUND "Build playground" ON)
option(GLYPH_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(GLYPH_ENABLE_COVERAGE "Enable coverage instrumentation" OFF)

# ============================================================================
# Library
# ============================================================================

add_library(glyph)

add_library(glyph::glyph ALIAS glyph)

file(GLOB_RECURSE GLYPH_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

target_sources(glyph
    PRIVATE
      ${GLYPH_SOURCES}
)

target_include_directories(glyph
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
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
# Coverage
# ============================================================================
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(EnableCoverage)

enable_coverage(glyph)

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
