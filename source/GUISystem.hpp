///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright(C) 2022 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "GUIItem.hpp"

///                                                                           
///   GUI System                                                              
///                                                                           
/// Manages and produces GUI items that interact within an isolated system    
///                                                                           
class GUISystem : public Unit {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) GUI;
   LANGULUS_BASES(Unit);
   LANGULUS_VERBS(Verbs::Create);

private:
   // List of created GUI items                                         
   TFactory<GUIItem> mItems;

public:
   GUISystem(const Any&);
   ~GUISystem();

   void Create(Verb&);

   virtual void Refresh();
};

