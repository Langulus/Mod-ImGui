///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright (c) 2022 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "GUI.hpp"

LANGULUS_DEFINE_MODULE(
   GUI, 9, "ImGui",
   "GUI generator and simulator, using ImGui as backend", "",
   GUI, GUISystem, GUIItem
)

/// Module construction                                                       
///   @param runtime - the runtime that owns the module                       
///   @param descriptor - instructions for configuring the module             
GUI::GUI(Runtime* runtime, Describe)
   : A::UIModule {MetaOf<GUI>(), runtime}
   , mSystems    {this} {
   VERBOSE_GUI("Initializing...");
   IMGUI_CHECKVERSION();
   VERBOSE_GUI("Initialized");
}

/// Module update routine                                                     
///   @param dt - time from last update                                       
bool GUI::Update(Time) {
   return true;
}

/// Create/Destroy GUI systems                                                
///   @param verb - the creation/destruction verb                             
void GUI::Create(Verb& verb) {
   mSystems.Create(verb);
}