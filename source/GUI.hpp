///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright (c) 2022 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "GUISystem.hpp"


///                                                                           
///   User interface module using ImGUI                                       
///                                                                           
/// Manages and produces GUI systems                                          
///                                                                           
struct GUI final : A::UI::Module {
   LANGULUS(ABSTRACT) false;
   LANGULUS_BASES(A::UI::Module);
   LANGULUS_VERBS(Verbs::Create);

private:
   // List of created GUI systems                                       
   TFactory<GUISystem> mSystems;

public:
   GUI(Runtime*, const Neat&);

   bool Update(Time);
   void Create(Verb&);
};

