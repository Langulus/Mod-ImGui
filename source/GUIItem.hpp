///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright (c) 2022 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Common.hpp"
#include <Flow/Factory.hpp>


///                                                                           
///   GUI item                                                                
///                                                                           
/// A single widget inside of a GUI system                                    
///                                                                           
struct GUIItem final : Things::UIUnit, ProducedFrom<GUISystem> {
   using CTTI_Abstract = No;
   using CTTI_Producer = GUISystem;
   LANGULUS_BASES(Things::UIUnit);

public:
   GUIItem(GUISystem*, Describe);

   void Refresh();
};

