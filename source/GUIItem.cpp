///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright(C) 2022 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "GUIItem.hpp"
#include "GUI.hpp"

/// GUI item construction                                                     
///   @param producer - the system producer                                   
///   @param descriptor - instructions for configuring the item               
GUIItem::GUIItem(GUISystem* producer, const Neat& descriptor)
   : A::UI::Unit {MetaOf<GUIItem>()}
   , ProducedFrom {producer, descriptor} {
   VERBOSE_GUI("Initializing...");
   VERBOSE_GUI("Initialized");
}

/// React on environmental change                                             
void GUIItem::Refresh() {

}

