///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright(C) 2022 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"

///                                                                           
///   GUI item                                                                
///                                                                           
/// A single widget inside of a GUI system                                    
///                                                                           
struct GUIItem final : A::UI::Unit {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) GUISystem;
   LANGULUS_BASES(A::UI::Unit);

private:
   Any mContents;

public:
   GUIItem(const Any&);

   void Refresh();
};

