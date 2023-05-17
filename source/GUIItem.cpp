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
GUIItem::GUIItem(GUISystem* producer, const Descriptor& descriptor)
   : A::UI::Unit {MetaOf<GUIItem>(), descriptor}
   , ProducedFrom {producer, descriptor} {
}

/// React on environmental change                                             
void GUIItem::Refresh() {

}

