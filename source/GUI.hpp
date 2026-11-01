///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright (c) 2022 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "GUISystem.hpp"


///                                                                           
///   User interface module using ImGUI                                       
///                                                                           
/// Manages and produces GUI systems                                          
struct GUI final : Things::UIModule {
   using CTTI_Abstract  = No;
   using CTTI_Bases     = Things::UIModule;
   using CTTI_Ability   = Verbs::Create;

private:
   // List of created GUI systems                                       
   TFactory<GUISystem> mSystems;

public:
   GUI(Runtime*, Describe);

   bool Update(Time);
   void Create(Verb&);
};

