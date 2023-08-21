add_executable(LangulusModImGuiTest
	Main.cpp
	TestGUI.cpp
)

target_link_libraries(LangulusModImGuiTest
	PRIVATE		Langulus
				Catch2
)

add_test(
	NAME		LangulusModImGuiTest
	COMMAND		LangulusModImGuiTest
)

langulus_copy_dlls(LangulusModImGuiTest ON LangulusModImGuiTest)
langulus_copy_dlls_advanced(LangulusModImGuiTest
	TO			LangulusModImGuiTest
	FROM		LangulusModGLFW
				LangulusModVulkan
				LangulusModImGui
)