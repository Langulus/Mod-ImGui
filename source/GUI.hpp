///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright(C) 2022 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "GUISystem.hpp"

///                                                                           
///   GUI manager                                                             
///                                                                           
/// Manages and produces GUI systems                                          
///                                                                           
class GUI : public Module {
   LANGULUS(ABSTRACT) false;
   LANGULUS_BASES(Module);
   LANGULUS_VERBS(Verbs::Create);

private:
   // List of created GUI systems                                       
   TFactory<GUISystem> mSystems;

public:
   GUI(Runtime*, const Any&);
   ~GUI();

   virtual void Update(Time) final;

   void Create(Verb&);
};

