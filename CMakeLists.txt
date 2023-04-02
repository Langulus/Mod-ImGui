project(Langulus.Mod.ImGui)

# Configure ImGui library, it will be statically built inside this module   
download_project(
    PROJ                ImGui
    GIT_REPOSITORY      https://github.com/ocornut/imgui.git
    GIT_TAG             master
    UPDATE_DISCONNECTED 1
)
include_directories(${ImGui_SOURCE_DIR})

# Build the module                                                          
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/source LANGULUS_MOD_IMGUI_SOURCES)
add_library(Mod.ImGui SHARED
    ${ImGui_SOURCE_DIR}/imgui.cpp
    ${ImGui_SOURCE_DIR}/imgui_draw.cpp
    ${ImGui_SOURCE_DIR}/imgui_widgets.cpp
    ${ImGui_SOURCE_DIR}/imgui_tables.cpp
    ${LANGULUS_MOD_IMGUI_SOURCES}
)

if (MSVC)
    target_compile_options(Mod.ImGui PRIVATE /bigobj)
endif()

target_link_libraries(Mod.ImGui PRIVATE Langulus.Framework)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()