///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright (c) 2022 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Export.hpp"


///                                                                           
///   GUI font                                                                
struct GUIFont final : Things::UIPart, ProducedFrom<GUISystem> {
   using CTTI_Abstract  = No;
   using CTTI_Producer  = GUISystem;
   using CTTI_Bases     = Things::UIPart;

private:
   Own<ImFont*>       mFont;
   Ref<Things::Image> mAtlas;

public:
   GUIFont(GUISystem const*, Neat const&);

   void Refresh() {}
};

