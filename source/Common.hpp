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

#if 0
   #define VERBOSE_GUI(...)      Logger::Verbose(Self(), __VA_ARGS__)
   #define VERBOSE_GUI_TAB(...)  const auto tab = Logger::VerboseTab(Self(), __VA_ARGS__)
#else
   #define VERBOSE_GUI(...)      LANGULUS(NOOP)
   #define VERBOSE_GUI_TAB(...)  LANGULUS(NOOP)
#endif

/// Include ImGui                                                             
#include <imgui.h>

#if LANGULUS(DEBUG)
   #define IMGUI_VULKAN_DEBUG_REPORT
#endif