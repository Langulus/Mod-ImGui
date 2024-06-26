cmake_minimum_required(VERSION 3.28)
project(LangulusModImGui
    VERSION         1.0.0
    DESCRIPTION     "Langulus plug-in module for handling user interface using ImGUI library"
    HOMEPAGE_URL    https://langulus.com
)

# Configure ImGui library, it will be statically built inside this module       
fetch_external_module(
    ImGui
    GIT_REPOSITORY  https://github.com/ocornut/imgui.git
    GIT_TAG         bcfc1ad # master
)

file(GLOB_RECURSE
	LANGULUS_MOD_IMGUI_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	source/*.cpp
)

# Build the module                                                              
add_library(LangulusModImGui SHARED
	${ImGui_SOURCE_DIR}/imgui.cpp
    ${ImGui_SOURCE_DIR}/imgui_draw.cpp
    ${ImGui_SOURCE_DIR}/imgui_widgets.cpp
    ${ImGui_SOURCE_DIR}/imgui_tables.cpp
    ${LANGULUS_MOD_IMGUI_SOURCES}
)

target_link_libraries(LangulusModImGui 
    PRIVATE     Langulus
)

target_include_directories(LangulusModImGui
    PRIVATE     ${ImGui_SOURCE_DIR}
)

if(LANGULUS_TESTING)
    enable_testing()
	add_subdirectory(test)
endif()