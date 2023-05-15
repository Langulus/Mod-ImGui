cmake_minimum_required(VERSION 3.24)
project(Langulus::Mod::ImGui)

# Configure ImGui library, it will be statically built inside this module   
fetch_external_module(
    ImGui
    GIT_REPOSITORY  https://github.com/ocornut/imgui.git
    GIT_TAG         bcfc1ad # master
)

# Build the module                                                          
add_library(LangulusModImGui SHARED
	${ImGui_SOURCE_DIR}/imgui.cpp
    ${ImGui_SOURCE_DIR}/imgui_draw.cpp
    ${ImGui_SOURCE_DIR}/imgui_widgets.cpp
    ${ImGui_SOURCE_DIR}/imgui_tables.cpp
    source/GUI.cpp
    source/GUISystem.cpp
    source/GUIItem.cpp
)

target_link_libraries(LangulusModImGui 
    PRIVATE     Langulus
)

target_include_directories(LangulusModImGui
    PRIVATE     ${ImGui_SOURCE_DIR}
)

if(LANGULUS_ENABLE_TESTING)
	add_subdirectory(test)
endif()