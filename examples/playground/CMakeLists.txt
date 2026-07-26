add_executable(playground
	main.cpp
)

target_link_libraries(playground
	PRIVATE
	  glyph::glyph
)

target_include_directories(playground
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

