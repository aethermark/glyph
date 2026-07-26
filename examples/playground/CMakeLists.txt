add_executable(glyph-playground
	main.cpp
)

target_link_libraries(glyph-playground
	PRIVATE
	  glyph::glyph
)

target_include_directories(glyph-playground
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

