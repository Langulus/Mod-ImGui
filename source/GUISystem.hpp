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
struct GUISystem final : Unit, ProducedFrom<GUI> {
   LANGULUS(ABSTRACT) false;
   LANGULUS_BASES(Unit);
   LANGULUS_VERBS(Verbs::Create);

private:
   ImGuiContext* mContext {};
   A::Window* mWindow {};
   A::Renderer* mRenderer {};
   double mTime {};
   Unit* mMouseWindow {};
   A::Cursor* mMouseCursors[ImGuiMouseCursor_COUNT] {};
   ImVec2 mLastValidMousePos {};
   Text mClipboard;

   // Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
   /*GLFWwindowfocusfun      PrevUserCallbackWindowFocus;
   GLFWcursorposfun        PrevUserCallbackCursorPos;
   GLFWcursorenterfun      PrevUserCallbackCursorEnter;
   GLFWmousebuttonfun      PrevUserCallbackMousebutton;
   GLFWscrollfun           PrevUserCallbackScroll;
   GLFWkeyfun              PrevUserCallbackKey;
   GLFWcharfun             PrevUserCallbackChar;
   GLFWmonitorfun          PrevUserCallbackMonitor;*/

   // List of created GUI items                                         
   TFactory<GUIItem> mItems;

public:
   GUISystem(GUI*, const Any&);
   ~GUISystem();

   void Create(Verb&);
   void Draw(Verb&);

   void Refresh();

   NOD() auto GetWindow() const noexcept { return mWindow; }
   NOD() auto& GetClipboard() noexcept { return mClipboard; }
};

