///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright (c) 2022 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"
#include <Flow/Factory.hpp>


///                                                                           
///   GUI item                                                                
///                                                                           
/// A single widget inside of a GUI system                                    
///                                                                           
struct GUIItem final : A::UI::Unit, ProducedFrom<GUISystem> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) GUISystem;
   LANGULUS_BASES(A::UI::Unit);

public:
   GUIItem(GUISystem*, const Neat&);

   void Refresh();
};

