project(Langulus.Mod.ImGui)

# We're importing symbols from Langulus.Framework                           
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
remove_definitions(-DLANGULUS_EXPORT_ALL)
if(NOT WIN32)
    add_compile_options(-fvisibility=hidden)
endif()

# Configure ImGui library, it will be statically built inside this module   
message(STATUS "Configuring external project: ImGui...")
download_project(
    PROJ                ImGui
    GIT_REPOSITORY      https://github.com/ocornut/imgui.git
    GIT_TAG             master
    UPDATE_DISCONNECTED 1
)
include_directories(${ImGui_SOURCE_DIR})

# Build the module                                                          
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/source SOURCE_FILES)
add_library(Mod.ImGui SHARED
    ${ImGui_SOURCE_DIR}/imgui.cpp
    ${ImGui_SOURCE_DIR}/imgui_draw.cpp
    ${ImGui_SOURCE_DIR}/imgui_widgets.cpp
    ${ImGui_SOURCE_DIR}/imgui_tables.cpp
    ${SOURCE_FILES}
)

target_link_libraries(Mod.ImGui
    PUBLIC      Langulus.Framework
)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()