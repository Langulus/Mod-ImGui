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

/// Include ImGui                                                             
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx10.h>
#include <d3d10_1.h>
#include <d3d10.h>
#include <tchar.h>

class GUI;
class GUISystem;
class GUIItem;