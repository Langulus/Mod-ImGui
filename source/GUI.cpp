///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright(C) 2022 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
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
GUI::GUI(Runtime* runtime, const Neat&)
   : A::UI::Module {MetaOf<GUI>(), runtime}
   , mSystems {this} {
   Logger::Verbose(Self(), "Initializing...");
   IMGUI_CHECKVERSION();
   Logger::Verbose(Self(), "Initialized");
}

/// Module update routine                                                     
///   @param dt - time from last update                                       
void GUI::Update(Time) {

}

/// Create/Destroy GUI systems                                                
///   @param verb - the creation/destruction verb                             
void GUI::Create(Verb& verb) {
   mSystems.Create(verb);
}