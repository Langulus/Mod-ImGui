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
///   GUI font                                                                
///                                                                           
struct GUIFont final : A::UI::Unit, ProducedFrom<GUISystem> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) GUISystem;
   LANGULUS_BASES(A::UI::Unit);

private:
   Own<ImFont*> mFont;
   Ref<A::Image> mAtlas;

public:
   GUIFont(GUISystem*, const Descriptor&);

   void Refresh() {}
};

