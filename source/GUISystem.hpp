///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright(C) 2022 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "GUIItem.hpp"
#include "GUIFont.hpp"


///                                                                           
///   GUI system                                                              
///                                                                           
/// Manages and produces GUI items that interact with each other within an    
/// isolated system                                                           
///                                                                           
struct GUISystem final : A::UI::System, ProducedFrom<GUI> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) GUI;
   LANGULUS_BASES(A::UI::System);
   LANGULUS_VERBS(Verbs::Create);

private:
   Ptr<A::Window> mWindow;
   Ptr<A::Renderer> mRenderer;

   Own<ImGuiContext*> mContext;
   Own<ImGuiIO*> mIO;

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
   TFactoryUnique<GUIFont> mFonts;

public:
   GUISystem(GUI*, const Neat&);
   ~GUISystem();

   void Create(Verb&);
   void Draw(Verb&);

   void Refresh();

   NOD() auto GetWindow() const noexcept { return mWindow; }
   NOD() auto& GetClipboard() noexcept { return mClipboard; }
   NOD() ImGuiIO* GetIO() const noexcept { return *mIO; }
};



// Initialization data, for ImGui_ImplVulkan_Init()
// [Please zero-clear before use!]
/*struct ImGui_ImplVulkan_InitInfo
{
   VkInstance                      Instance;
   VkPhysicalDevice                PhysicalDevice;
   VkDevice                        Device;
   uint32_t                        QueueFamily;
   VkQueue                         Queue;
   VkPipelineCache                 PipelineCache;
   VkDescriptorPool                DescriptorPool;
   uint32_t                        Subpass;
   uint32_t                        MinImageCount;          // >= 2
   uint32_t                        ImageCount;             // >= MinImageCount
   VkSampleCountFlagBits           MSAASamples;            // >= VK_SAMPLE_COUNT_1_BIT (0 -> default to VK_SAMPLE_COUNT_1_BIT)
   const VkAllocationCallbacks* Allocator;
   void                            (*CheckVkResultFn)(VkResult err);
};*/

/*
// Vulkan data
struct ImGui_ImplVulkan_Data
{
   ImGui_ImplVulkan_InitInfo   VulkanInitInfo;
   VkRenderPass                RenderPass;
   VkDeviceSize                BufferMemoryAlignment;
   VkPipelineCreateFlags       PipelineCreateFlags;
   VkDescriptorSetLayout       DescriptorSetLayout;
   VkPipelineLayout            PipelineLayout;
   VkPipeline                  Pipeline;
   uint32_t                    Subpass;
   VkShaderModule              ShaderModuleVert;
   VkShaderModule              ShaderModuleFrag;

   // Font data
   VkSampler                   FontSampler;
   VkDeviceMemory              FontMemory;
   VkImage                     FontImage;
   VkImageView                 FontView;
   VkDescriptorSet             FontDescriptorSet;
   VkDeviceMemory              UploadBufferMemory;
   VkBuffer                    UploadBuffer;

   // Render buffers for main window
   ImGui_ImplVulkanH_WindowRenderBuffers MainWindowRenderBuffers;

   ImGui_ImplVulkan_Data()
   {
      memset((void*)this, 0, sizeof(*this));
      BufferMemoryAlignment = 256;
   }
};*/