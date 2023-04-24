///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright(C) 2022 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include <Langulus.hpp>

using namespace Langulus;
using namespace Langulus::Flow;
using namespace Langulus::Anyness;
using namespace Langulus::Entity;
using namespace Langulus::Math;

struct GUI;
struct GUISystem;
struct GUIItem;

/// Include ImGui                                                             
#include <imgui.h>

#if LANGULUS(DEBUG)
   #define IMGUI_VULKAN_DEBUG_REPORT
#endif