file(GLOB_RECURSE
	LANGULUS_MOD_IMGUI_TEST_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp *.hpp
)

add_executable(LangulusModImGuiTest ${LANGULUS_MOD_IMGUI_TEST_SOURCES})

target_link_libraries(LangulusModImGuiTest
	PRIVATE		Langulus
				Catch2
)

add_dependencies(LangulusModImGuiTest
	LangulusModImGui
)

add_test(
	NAME		LangulusModImGuiTest
	COMMAND		LangulusModImGuiTest
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)