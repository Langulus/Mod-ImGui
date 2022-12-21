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
///   @param descriptor - instructions for configuring the item               
GUIItem::GUIItem(const Any& descriptor)
   : Unit {MetaOf<GUIItem>(), descriptor}
   , mContents {this} { }

/// React on environmental change                                             
void GUIItem::Refresh() {

}

