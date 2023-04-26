project(Langulus.Test.Mod.ImGui)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} LANGULUS_MOD_IMGUI_TEST_SOURCES)
add_executable(Test.Mod.ImGui ${LANGULUS_MOD_IMGUI_TEST_SOURCES})

#if (MSVC)
#    target_compile_options(Test.Mod.ImGui PRIVATE /bigobj)
#endif()

target_link_libraries(Test.Mod.ImGui
	PRIVATE		Langulus.Framework
				Catch2
)

add_dependencies(Test.Mod.ImGui Mod.GLFW Mod.ImGui)
langulus_copy_dlls(
	TO		Test.Mod.ImGui
	FROM	Langulus.Framework Mod.GLFW Mod.Vulkan Mod.ImGui
)

add_test(NAME Test.Mod.ImGui COMMAND Test.Mod.ImGui)
