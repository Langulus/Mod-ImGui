///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright(C) 2022 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include <Entity/External.hpp>

using namespace Langulus;

struct GUI;
struct GUISystem;
struct GUIItem;
struct GUIFont;

/// Include ImGui                                                             
#include <imgui.h>

#if LANGULUS(DEBUG)
   #define IMGUI_VULKAN_DEBUG_REPORT
#endif