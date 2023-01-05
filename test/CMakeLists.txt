project(Langulus.Test.Mod.ImGui)
remove_definitions(-DLANGULUS_EXPORT_IMGUI)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES)
add_executable(Test.Mod.ImGui ${SOURCE_FILES})
target_link_libraries(Test.Mod.ImGui
	PRIVATE		Mod.ImGui
				Catch2
)

add_custom_command(
    TARGET Test.Mod.ImGui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:Langulus.Framework>" "$<TARGET_FILE_DIR:Test.Mod.ImGui>"
    COMMENT "Copying `$<TARGET_FILE:Langulus.Framework>` to `$<TARGET_FILE_DIR:Test.Mod.ImGui>`"
)

add_custom_command(
    TARGET Test.Mod.ImGui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:Mod.GLFW>" "$<TARGET_FILE_DIR:Test.Mod.ImGui>"
    COMMENT "Copying `$<TARGET_FILE:Mod.GLFW>` to `$<TARGET_FILE_DIR:Test.Mod.ImGui>`"
)

add_custom_command(
    TARGET Test.Mod.ImGui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:Mod.ImGui>" "$<TARGET_FILE_DIR:Test.Mod.ImGui>"
    COMMENT "Copying `$<TARGET_FILE:Mod.ImGui>` to `$<TARGET_FILE_DIR:Test.Mod.ImGui>`"
)

add_test(NAME Test.Mod.ImGui COMMAND Test.Mod.ImGui)
