///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright (c) 2022 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
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
   GUIFont(GUISystem*, const Neat&);

   void Refresh() {}
};

