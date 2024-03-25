///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright (c) 2022 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include <Langulus.hpp>
#include <Langulus/UI.hpp>

using namespace Langulus;

struct GUI;
struct GUISystem;
struct GUIItem;
struct GUIFont;

#define VERBOSE_GUI(...)      Logger::Verbose(Self(), __VA_ARGS__)
#define VERBOSE_GUI_TAB(...)  const auto tab = Logger::Verbose(Self(), __VA_ARGS__, Logger::Tabs {})

/// Include ImGui                                                             
#include <imgui.h>

#if LANGULUS(DEBUG)
   #define IMGUI_VULKAN_DEBUG_REPORT
#endif