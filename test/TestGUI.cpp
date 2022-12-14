///                                                                           
/// Langulus::Module::GLFW                                                    
/// Copyright(C) 2015 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Main.hpp"
#include <catch2/catch.hpp>

/// See https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md        
CATCH_TRANSLATE_EXCEPTION(::Langulus::Exception const& ex) {
   const Text serialized {ex};
   return ::std::string {Token {serialized}};
}

SCENARIO("Window creation", "[window]") {
   GIVEN("A root entity") {
      // Create root entity                                             
      Thing root;
      root.AddTrait(Traits::Name {"ROOT"_text});

      // Create runtime at the root                                     
      auto runtime = root.CreateRuntime();

      // Load GLFW module                                               
      auto module = root.LoadMod("GLFW");

      WHEN("The window is created") {
         root.CreateUnitToken("Window");

         // Update once                                                 
         root.Update(Time::zero());

         THEN("Various traits change") {
            root.DumpHierarchy();

            REQUIRE(true);
         }
      }
   }
}

