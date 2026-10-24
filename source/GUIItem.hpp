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
struct GUIPart final : Things::UIPart, ProducedFrom<GUISystem> {
   using CTTI_Abstract = No;
   using CTTI_Producer = GUISystem;
   LANGULUS_BASES(Things::UIPart);

public:
   GUIPart(GUISystem*, Describe);

   void Refresh();
};

