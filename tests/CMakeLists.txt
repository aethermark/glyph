include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(Catch2)

add_executable(glyph-tests)

file(GLOB_RECURSE GLYPH_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

target_sources(glyph-tests 
    PRIVATE
        ${GLYPH_TEST_SOURCES}
)

target_include_directories(glyph-tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(EnableCoverage)

enable_coverage(glyph-tests)

target_link_libraries(glyph-tests
    PRIVATE
        glyph::glyph
        Catch2::Catch2WithMain
)

include(Catch)

catch_discover_tests(glyph-tests)
