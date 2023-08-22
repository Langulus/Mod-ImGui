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
   Allocator::State memoryState;

   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         Thing root;
         root.SetName("ROOT");

         // Create runtime at the root                                  
         root.CreateRuntime();

         // Load ImGui module                                           
         root.LoadMod("GLFW");
         root.LoadMod("Vulkan");
         root.LoadMod("ImGui");

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The GUI system is created via tokens") {
            auto window = root.CreateUnitToken("Window", Traits::Size(640, 480));
            auto renderer = root.CreateUnitToken("Renderer");
            auto gui = root.CreateUnitToken("GUISystem");

            // Update once                                              
            root.Update(Time::zero());

            THEN("Various traits change") {
               root.DumpHierarchy();

               REQUIRE(window.GetCount() == 1);
               REQUIRE(window.CastsTo<A::Window>(1));
               REQUIRE(window.IsSparse());

               REQUIRE(renderer.GetCount() == 1);
               REQUIRE(renderer.CastsTo<A::Renderer>(1));
               REQUIRE(renderer.IsSparse());

               REQUIRE(gui.GetCount() == 1);
               REQUIRE(gui.CastsTo<A::UI::System>(1));
               REQUIRE(gui.IsSparse());
            }
         }
      #endif

         WHEN("The GUI system is created via abstractions") {
            auto window = root.CreateUnit<A::Window>(Traits::Size(640, 480));
            auto renderer = root.CreateUnit<A::Renderer>();
            auto gui = root.CreateUnit<A::UI::System>();

            // Update once                                              
            root.Update(Time::zero());

            THEN("Various traits change") {
               root.DumpHierarchy();

               REQUIRE(window.GetCount() == 1);
               REQUIRE(window.CastsTo<A::Window>(1));
               REQUIRE(window.IsSparse());

               REQUIRE(renderer.GetCount() == 1);
               REQUIRE(renderer.CastsTo<A::Renderer>(1));
               REQUIRE(renderer.IsSparse());

               REQUIRE(gui.GetCount() == 1);
               REQUIRE(gui.CastsTo<A::UI::System>(1));
               REQUIRE(gui.IsSparse());
            }
         }

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());
      }
   }
}

