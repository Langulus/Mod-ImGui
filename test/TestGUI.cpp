///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright(C) 2022 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Main.hpp"
#include <Entity/Thing.hpp>
#include <catch2/catch.hpp>

/// See https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md        
CATCH_TRANSLATE_EXCEPTION(::Langulus::Exception const& ex) {
   const Text serialized {ex};
   return ::std::string {Token {serialized}};
}

SCENARIO("GUI creation", "[gui]") {
   GIVEN("A root entity") {
      // Create root entity                                             
      Thing root;
      root.SetName("ROOT"_text);

      // Create runtime at the root                                     
      root.CreateRuntime();

      // Load ImGui module                                              
      root.LoadMod("GLFW");
      root.LoadMod("Vulkan");
      root.LoadMod("ImGui");

      WHEN("The GUI system is created via tokens") {
         root.CreateUnitToken("Window");
         root.CreateUnitToken("Renderer");
         root.CreateUnitToken("GUISystem");

         // Update once                                                 
         root.Update(Time::zero());

         THEN("Various traits change") {
            root.DumpHierarchy();

            REQUIRE(true);
         }
      }

      WHEN("The GUI system is created via abstractions") {
         root.CreateUnit<A::Window>();
         root.CreateUnit<A::Renderer>();
         root.CreateUnit<A::UI::System>();

         // Update once                                                 
         root.Update(Time::zero());

         THEN("Various traits change") {
            root.DumpHierarchy();

            REQUIRE(true);
         }
      }
   }
}

