///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright (c) 2022 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "GUI.hpp"


/// Function used by ImGui to retrieve current system clipboard               
///   @param user_data - pointer to the GUI system                            
///   @return a pointer to the clipboard text data (null-terminated)          
const char* GetClipboardText(void* user_data) {
   auto system = static_cast<GUISystem*>(user_data);
   system->GetWindow()->GetTrait<Traits::Clipboard>(system->GetClipboard());
   system->GetClipboard() = system->GetClipboard().Terminate();
   return system->GetClipboard().GetRaw();
}

/// Function used by ImGui to set current system clipboard                    
///   @param user_data - pointer to the GUI system                            
///   @param text - text to set                                               
void SetClipboardText(void* user_data, const char* text) {
   auto system = static_cast<GUISystem*>(user_data);
   system->GetClipboard() = Text {text}.Terminate();
   system->GetWindow()->SetTrait<Traits::Clipboard>(system->GetClipboard());
}

/// GUI system construction                                                   
///   @param producer - the system producer                                   
///   @param descriptor - instructions for configuring the GUI                
GUISystem::GUISystem(GUI* producer, Describe descriptor)
   : A::UISystem  {MetaOf<GUISystem>()}
   , ProducedFrom {producer, descriptor}
   , mItems {this}
   , mFonts {this} {
   VERBOSE_GUI("Initializing...");

   // Retrieve relevant traits from the environment                     
   mWindow = SeekUnitAux<A::Window>(descriptor);
   LANGULUS_ASSERT(mWindow, Construct,
      "No window available for renderer");

   mRenderer = SeekUnitAux<A::Renderer>(descriptor);
   LANGULUS_ASSERT(mRenderer, Construct,
      "No renderer available for UI");

   // Create the context for the GUI system                             
   mContext = ImGui::CreateContext();
   ImGui::SetCurrentContext(mContext);
   ImGui::StyleColorsDark();

   // TODO Pick a color style from descriptor
   //ImGui::StyleColorsLight();

   // Employ the available platform and renderer backends               
   mIO = &ImGui::GetIO();
   mIO->BackendPlatformUserData = this;
   mIO->BackendPlatformName = "Langulus";
   mIO->BackendRendererUserData = this;
   mIO->BackendRendererName = "Langulus";
   // We can honor GetMouseCursor() values (optional)                   
   mIO->BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
   // We can honor io.WantSetMousePos requests (optional, rarely used)  
   mIO->BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
   // We can honor the ImDrawCmd::VtxOffset field, allowing meshes      
   mIO->BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
   mIO->SetClipboardTextFn = SetClipboardText;
   mIO->GetClipboardTextFn = GetClipboardText;
   mIO->ClipboardUserData = mWindow;

   // Set platform dependent data in viewport                           
   #if LANGULUS_OS(WINDOWS)
      ImGui::GetMainViewport()->PlatformHandleRaw = mWindow->GetNativeHandle();
   #endif

   // Find available mouse cursors                                      
   mMouseCursors[ImGuiMouseCursor_Arrow] =
      SeekUnitAuxExt<A::Cursor>(descriptor, "arrow");
      // glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
   mMouseCursors[ImGuiMouseCursor_TextInput] = 
      SeekUnitAuxExt<A::Cursor>(descriptor, "ibeam");
      // glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
   mMouseCursors[ImGuiMouseCursor_ResizeNS] = 
      SeekUnitAuxExt<A::Cursor>(descriptor, "resize_vertical");
      // glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
   mMouseCursors[ImGuiMouseCursor_ResizeEW] =
      SeekUnitAuxExt<A::Cursor>(descriptor, "resize_horizontal");
      // glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
   mMouseCursors[ImGuiMouseCursor_Hand] = 
      SeekUnitAuxExt<A::Cursor>(descriptor, "hand");
      // glfwCreateStandardCursor(GLFW_HAND_CURSOR);
   mMouseCursors[ImGuiMouseCursor_ResizeAll] = 
      SeekUnitAuxExt<A::Cursor>(descriptor, "resize_all");
      // glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
   mMouseCursors[ImGuiMouseCursor_ResizeNESW] = 
      SeekUnitAuxExt<A::Cursor>(descriptor, "resize_nesw");
      // glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
   mMouseCursors[ImGuiMouseCursor_ResizeNWSE] = 
      SeekUnitAuxExt<A::Cursor>(descriptor, "resize_nwse");
      // glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
   mMouseCursors[ImGuiMouseCursor_NotAllowed] = 
      SeekUnitAuxExt<A::Cursor>(descriptor, "nope");
      // glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);

   // Load Fonts                                                        
   // If no fonts are loaded, dear imgui will use the default font,     
   // which isn't desirable, because it won't be interfaced by Langulus 
   // You can also load multiple fonts and use ImGui::PushFont() or     
   // PopFont() to select them.                                         
   Verbs::Create createFont {
      Construct::From<GUIFont>(
         Traits::Name {"default"},
         Traits::Size {16.0f}
      )
   };
   mFonts.Create(createFont);

   /*{
      // Create font sampler                                            
      {
         VkSamplerCreateInfo info = {};
         info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
         info.magFilter = VK_FILTER_LINEAR;
         info.minFilter = VK_FILTER_LINEAR;
         info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
         info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
         info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
         info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
         info.minLod = -1000;
         info.maxLod = 1000;
         info.maxAnisotropy = 1.0f;
         vkCreateSampler(v->Device, &info, v->Allocator, &bd->FontSampler);
      }

      // Descriptor set layout for the font                             
      {
         VkSampler sampler[1] = {bd->FontSampler};
         VkDescriptorSetLayoutBinding binding[1] = {};
         binding[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
         binding[0].descriptorCount = 1;
         binding[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
         binding[0].pImmutableSamplers = sampler;
         VkDescriptorSetLayoutCreateInfo info = {};
         info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
         info.bindingCount = 1;
         info.pBindings = binding;
         vkCreateDescriptorSetLayout(v->Device, &info, v->Allocator, &bd->DescriptorSetLayout);
      }

      // Pipeline layout                                                
      {
         // Constants: we are using 'vec2 offset' and 'vec2 scale' instead of a full 3d projection matrix
         VkPushConstantRange push_constants[1] = {};
         push_constants[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
         push_constants[0].offset = sizeof(float) * 0;
         push_constants[0].size = sizeof(float) * 4;
         VkDescriptorSetLayout set_layout[1] = {bd->DescriptorSetLayout};
         VkPipelineLayoutCreateInfo layout_info = {};
         layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
         layout_info.setLayoutCount = 1;
         layout_info.pSetLayouts = set_layout;
         layout_info.pushConstantRangeCount = 1;
         layout_info.pPushConstantRanges = push_constants;
         vkCreatePipelineLayout(v->Device, &layout_info, v->Allocator, &bd->PipelineLayout);
      }

      // Create pipeline for rendering draw lists                       
      {
         // Create the shader modules                                   
         {
            // Vertex shader                                            
            {
               VkShaderModuleCreateInfo vert_info = {};
               vert_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
               vert_info.codeSize = sizeof(__glsl_shader_vert_spv);
               vert_info.pCode = (uint32_t*)__glsl_shader_vert_spv;
               vkCreateShaderModule(device, &vert_info, allocator, &bd->ShaderModuleVert);
            }

            // Fragment shader                                          
            {
               VkShaderModuleCreateInfo frag_info = {};
               frag_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
               frag_info.codeSize = sizeof(__glsl_shader_frag_spv);
               frag_info.pCode = (uint32_t*)__glsl_shader_frag_spv;
               vkCreateShaderModule(device, &frag_info, allocator, &bd->ShaderModuleFrag);
            }
         }

         VkPipelineShaderStageCreateInfo stage[2] = {};
         stage[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
         stage[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
         stage[0].module = bd->ShaderModuleVert;                  //imgui specific (vertex shader)
         stage[0].pName = "main";
         stage[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
         stage[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
         stage[1].module = bd->ShaderModuleFrag;                  //imgui specific (fragment shader)
         stage[1].pName = "main";

         VkVertexInputBindingDescription binding_desc[1] = {};
         binding_desc[0].stride = sizeof(ImDrawVert);             //imgui specific (vertex size)
         binding_desc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

         VkVertexInputAttributeDescription attribute_desc[3] = {};
         attribute_desc[0].location = 0;
         attribute_desc[0].binding = binding_desc[0].binding;
         attribute_desc[0].format = VK_FORMAT_R32G32_SFLOAT;
         attribute_desc[0].offset = IM_OFFSETOF(ImDrawVert, pos); //imgui specific (vertex position offset)
         attribute_desc[1].location = 1;
         attribute_desc[1].binding = binding_desc[0].binding;
         attribute_desc[1].format = VK_FORMAT_R32G32_SFLOAT;
         attribute_desc[1].offset = IM_OFFSETOF(ImDrawVert, uv);  //imgui specific (vertex uv offset)
         attribute_desc[2].location = 2;
         attribute_desc[2].binding = binding_desc[0].binding;
         attribute_desc[2].format = VK_FORMAT_R8G8B8A8_UNORM;
         attribute_desc[2].offset = IM_OFFSETOF(ImDrawVert, col); //imgui specific (vertex color offset)

         VkPipelineVertexInputStateCreateInfo vertex_info = {};
         vertex_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
         vertex_info.vertexBindingDescriptionCount = 1;
         vertex_info.pVertexBindingDescriptions = binding_desc;
         vertex_info.vertexAttributeDescriptionCount = 3;
         vertex_info.pVertexAttributeDescriptions = attribute_desc;

         VkPipelineInputAssemblyStateCreateInfo ia_info = {};
         ia_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
         ia_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;  //imgui specific (topology)

         VkPipelineViewportStateCreateInfo viewport_info = {};
         viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
         viewport_info.viewportCount = 1;
         viewport_info.scissorCount = 1;

         VkPipelineRasterizationStateCreateInfo raster_info = {};
         raster_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
         raster_info.polygonMode = VK_POLYGON_MODE_FILL;
         raster_info.cullMode = VK_CULL_MODE_NONE;
         raster_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; //imgui specific (culling)
         raster_info.lineWidth = 1.0f;

         VkPipelineMultisampleStateCreateInfo ms_info = {};
         ms_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
         ms_info.rasterizationSamples = (MSAASamples != 0) ? MSAASamples : VK_SAMPLE_COUNT_1_BIT;

         VkPipelineColorBlendAttachmentState color_attachment[1] = {};
         color_attachment[0].blendEnable = VK_TRUE;
         color_attachment[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
         color_attachment[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
         color_attachment[0].colorBlendOp = VK_BLEND_OP_ADD;
         color_attachment[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
         color_attachment[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
         color_attachment[0].alphaBlendOp = VK_BLEND_OP_ADD;
         color_attachment[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

         VkPipelineDepthStencilStateCreateInfo depth_info = {}; //imgui specific (no depth attachment?)
         depth_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

         VkPipelineColorBlendStateCreateInfo blend_info = {};
         blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
         blend_info.attachmentCount = 1;
         blend_info.pAttachments = color_attachment;

         VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
         VkPipelineDynamicStateCreateInfo dynamic_state = {};
         dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
         dynamic_state.dynamicStateCount = (uint32_t)IM_ARRAYSIZE(dynamic_states);
         dynamic_state.pDynamicStates = dynamic_states;

         VkGraphicsPipelineCreateInfo info = {};
         info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
         info.flags = bd->PipelineCreateFlags;
         info.stageCount = 2;
         info.pStages = stage;
         info.pVertexInputState = &vertex_info;
         info.pInputAssemblyState = &ia_info;
         info.pViewportState = &viewport_info;
         info.pRasterizationState = &raster_info;
         info.pMultisampleState = &ms_info;
         info.pDepthStencilState = &depth_info;
         info.pColorBlendState = &blend_info;
         info.pDynamicState = &dynamic_state;
         info.layout = bd->PipelineLayout;
         info.renderPass = renderPass;
         info.subpass = subpass;
         vkCreateGraphicsPipelines(device, pipelineCache, 1, &info, allocator, pipeline);
      }
   }*/
   VERBOSE_GUI("Initialized");
}

/// GUI system destruction                                                    
GUISystem::~GUISystem() {
   if (mContext)
      ImGui::DestroyContext(mContext);
}

/// Produce GUI elements and fonts                                            
///   @param verb - creation verb to satisfy                                  
void GUISystem::Create(Verb& verb) {
   mItems.Create(verb);
   mFonts.Create(verb);
}

void GUISystem::Draw(Verb&) {
   //ImGui_ImplGlfw_NewFrame();
   /*{
      ImGuiIO& io = ImGui::GetIO();

      // Setup display size (every frame to accommodate for window resizing)
      int w, h;
      int display_w, display_h;
      glfwGetWindowSize(bd->Window, &w, &h);
      glfwGetFramebufferSize(bd->Window, &display_w, &display_h);
      io.DisplaySize = ImVec2((float)w, (float)h);
      if (w > 0 && h > 0)
         io.DisplayFramebufferScale = ImVec2((float)display_w / (float)w, (float)display_h / (float)h);

      // Setup time step
      double current_time = glfwGetTime();
      io.DeltaTime = bd->Time > 0.0 ? (float)(current_time - bd->Time) : (float)(1.0f / 60.0f);
      mTime = current_time;

      //ImGui_ImplGlfw_UpdateMouseData();
      {
         if (glfwGetInputMode(bd->Window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
         {
            io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
            return;
         }

         #ifdef __EMSCRIPTEN__
            const bool is_app_focused = true;
         #else
            const bool is_app_focused = glfwGetWindowAttrib(bd->Window, GLFW_FOCUSED) != 0;
         #endif

         if (is_app_focused)
         {
            // (Optional) Set OS mouse position from Dear ImGui if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
            if (io.WantSetMousePos)
               glfwSetCursorPos(bd->Window, (double)io.MousePos.x, (double)io.MousePos.y);

            // (Optional) Fallback to provide mouse position when focused (ImGui_ImplGlfw_CursorPosCallback already provides this when hovered or captured)
            if (is_app_focused && bd->MouseWindow == nullptr)
            {
               double mouse_x, mouse_y;
               glfwGetCursorPos(bd->Window, &mouse_x, &mouse_y);
               io.AddMousePosEvent((float)mouse_x, (float)mouse_y);
               bd->LastValidMousePos = ImVec2((float)mouse_x, (float)mouse_y);
            }
         }
      }

      //ImGui_ImplGlfw_UpdateMouseCursor();
      {
         if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(bd->Window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
            return;

         ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
         if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
         {
            // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
            glfwSetInputMode(bd->Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
         }
         else
         {
            // Show OS mouse cursor
            // FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with GLFW 3.2, but 3.3 works here.
            glfwSetCursor(bd->Window, bd->MouseCursors[imgui_cursor] ? bd->MouseCursors[imgui_cursor] : bd->MouseCursors[ImGuiMouseCursor_Arrow]);
            glfwSetInputMode(bd->Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
         }
      }

      // Update game controllers (if enabled and available)
      //ImGui_ImplGlfw_UpdateGamepads();
      {
         if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0) // FIXME: Technically feeding gamepad shouldn't depend on this now that they are regular inputs.
            return;

         io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;

         #if GLFW_HAS_GAMEPAD_API
            GLFWgamepadstate gamepad;
            if (!glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepad))
               return;
            #define MAP_BUTTON(KEY_NO, BUTTON_NO, _UNUSED)          do { io.AddKeyEvent(KEY_NO, gamepad.buttons[BUTTON_NO] != 0); } while (0)
            #define MAP_ANALOG(KEY_NO, AXIS_NO, _UNUSED, V0, V1)    do { float v = gamepad.axes[AXIS_NO]; v = (v - V0) / (V1 - V0); io.AddKeyAnalogEvent(KEY_NO, v > 0.10f, Saturate(v)); } while (0)
         #else
            int axes_count = 0, buttons_count = 0;
            const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);
            const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttons_count);
            if (axes_count == 0 || buttons_count == 0)
               return;
            #define MAP_BUTTON(KEY_NO, _UNUSED, BUTTON_NO)          do { io.AddKeyEvent(KEY_NO, (buttons_count > BUTTON_NO && buttons[BUTTON_NO] == GLFW_PRESS)); } while (0)
            #define MAP_ANALOG(KEY_NO, _UNUSED, AXIS_NO, V0, V1)    do { float v = (axes_count > AXIS_NO) ? axes[AXIS_NO] : V0; v = (v - V0) / (V1 - V0); io.AddKeyAnalogEvent(KEY_NO, v > 0.10f, Saturate(v)); } while (0)
         #endif

         io.BackendFlags |= ImGuiBackendFlags_HasGamepad;

         MAP_BUTTON(ImGuiKey_GamepadStart, GLFW_GAMEPAD_BUTTON_START, 7);
         MAP_BUTTON(ImGuiKey_GamepadBack, GLFW_GAMEPAD_BUTTON_BACK, 6);
         MAP_BUTTON(ImGuiKey_GamepadFaceLeft, GLFW_GAMEPAD_BUTTON_X, 2);     // Xbox X, PS Square
         MAP_BUTTON(ImGuiKey_GamepadFaceRight, GLFW_GAMEPAD_BUTTON_B, 1);     // Xbox B, PS Circle
         MAP_BUTTON(ImGuiKey_GamepadFaceUp, GLFW_GAMEPAD_BUTTON_Y, 3);     // Xbox Y, PS Triangle
         MAP_BUTTON(ImGuiKey_GamepadFaceDown, GLFW_GAMEPAD_BUTTON_A, 0);     // Xbox A, PS Cross
         MAP_BUTTON(ImGuiKey_GamepadDpadLeft, GLFW_GAMEPAD_BUTTON_DPAD_LEFT, 13);
         MAP_BUTTON(ImGuiKey_GamepadDpadRight, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, 11);
         MAP_BUTTON(ImGuiKey_GamepadDpadUp, GLFW_GAMEPAD_BUTTON_DPAD_UP, 10);
         MAP_BUTTON(ImGuiKey_GamepadDpadDown, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, 12);
         MAP_BUTTON(ImGuiKey_GamepadL1, GLFW_GAMEPAD_BUTTON_LEFT_BUMPER, 4);
         MAP_BUTTON(ImGuiKey_GamepadR1, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER, 5);
         MAP_ANALOG(ImGuiKey_GamepadL2, GLFW_GAMEPAD_AXIS_LEFT_TRIGGER, 4, -0.75f, +1.0f);
         MAP_ANALOG(ImGuiKey_GamepadR2, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER, 5, -0.75f, +1.0f);
         MAP_BUTTON(ImGuiKey_GamepadL3, GLFW_GAMEPAD_BUTTON_LEFT_THUMB, 8);
         MAP_BUTTON(ImGuiKey_GamepadR3, GLFW_GAMEPAD_BUTTON_RIGHT_THUMB, 9);
         MAP_ANALOG(ImGuiKey_GamepadLStickLeft, GLFW_GAMEPAD_AXIS_LEFT_X, 0, -0.25f, -1.0f);
         MAP_ANALOG(ImGuiKey_GamepadLStickRight, GLFW_GAMEPAD_AXIS_LEFT_X, 0, +0.25f, +1.0f);
         MAP_ANALOG(ImGuiKey_GamepadLStickUp, GLFW_GAMEPAD_AXIS_LEFT_Y, 1, -0.25f, -1.0f);
         MAP_ANALOG(ImGuiKey_GamepadLStickDown, GLFW_GAMEPAD_AXIS_LEFT_Y, 1, +0.25f, +1.0f);
         MAP_ANALOG(ImGuiKey_GamepadRStickLeft, GLFW_GAMEPAD_AXIS_RIGHT_X, 2, -0.25f, -1.0f);
         MAP_ANALOG(ImGuiKey_GamepadRStickRight, GLFW_GAMEPAD_AXIS_RIGHT_X, 2, +0.25f, +1.0f);
         MAP_ANALOG(ImGuiKey_GamepadRStickUp, GLFW_GAMEPAD_AXIS_RIGHT_Y, 3, -0.25f, -1.0f);
         MAP_ANALOG(ImGuiKey_GamepadRStickDown, GLFW_GAMEPAD_AXIS_RIGHT_Y, 3, +0.25f, +1.0f);
         #undef MAP_BUTTON
         #undef MAP_ANALOG
      }
   }*/

   ImGui::NewFrame();

   ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
   ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
   ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
   ImGui::End();

   // Rendering
   ImGui::Render();
   //ImDrawData* draw_data = ImGui::GetDrawData();


   // Record dear imgui primitives into command buffer
   // Must be called between vkCmdBeginRenderPass and vkCmdEndRenderPass
   //ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);
   /*{
      // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
      int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
      int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
      if (fb_width <= 0 || fb_height <= 0)
         return;

      ImGui_ImplVulkan_Data* bd = ImGui_ImplVulkan_GetBackendData();
      ImGui_ImplVulkan_InitInfo* v = &bd->VulkanInitInfo;
      if (pipeline == VK_NULL_HANDLE)
         pipeline = bd->Pipeline;

      // Allocate array to store enough vertex/index buffers
      ImGui_ImplVulkanH_WindowRenderBuffers* wrb = &bd->MainWindowRenderBuffers;
      if (wrb->FrameRenderBuffers == nullptr)
      {
         wrb->Index = 0;
         wrb->Count = v->ImageCount;
         wrb->FrameRenderBuffers = (ImGui_ImplVulkanH_FrameRenderBuffers*)IM_ALLOC(sizeof(ImGui_ImplVulkanH_FrameRenderBuffers) * wrb->Count);
         memset(wrb->FrameRenderBuffers, 0, sizeof(ImGui_ImplVulkanH_FrameRenderBuffers) * wrb->Count);
      }
      IM_ASSERT(wrb->Count == v->ImageCount);
      wrb->Index = (wrb->Index + 1) % wrb->Count;
      ImGui_ImplVulkanH_FrameRenderBuffers* rb = &wrb->FrameRenderBuffers[wrb->Index];

      if (draw_data->TotalVtxCount > 0)
      {
         // Create or resize the vertex/index buffers
         size_t vertex_size = draw_data->TotalVtxCount * sizeof(ImDrawVert);
         size_t index_size = draw_data->TotalIdxCount * sizeof(ImDrawIdx);
         if (rb->VertexBuffer == VK_NULL_HANDLE || rb->VertexBufferSize < vertex_size)
            CreateOrResizeBuffer(rb->VertexBuffer, rb->VertexBufferMemory, rb->VertexBufferSize, vertex_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
         if (rb->IndexBuffer == VK_NULL_HANDLE || rb->IndexBufferSize < index_size)
            CreateOrResizeBuffer(rb->IndexBuffer, rb->IndexBufferMemory, rb->IndexBufferSize, index_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

         // Upload vertex/index data into a single contiguous GPU buffer
         ImDrawVert* vtx_dst = nullptr;
         ImDrawIdx* idx_dst = nullptr;
         vkMapMemory(v->Device, rb->VertexBufferMemory, 0, rb->VertexBufferSize, 0, (void**)(&vtx_dst));
         vkMapMemory(v->Device, rb->IndexBufferMemory, 0, rb->IndexBufferSize, 0, (void**)(&idx_dst));

         for (int n = 0; n < draw_data->CmdListsCount; n++)
         {
            const ImDrawList* cmd_list = draw_data->CmdLists[n];
            memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
            memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
            vtx_dst += cmd_list->VtxBuffer.Size;
            idx_dst += cmd_list->IdxBuffer.Size;
         }

         VkMappedMemoryRange range[2] = {};
         range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
         range[0].memory = rb->VertexBufferMemory;
         range[0].size = VK_WHOLE_SIZE;
         range[1].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
         range[1].memory = rb->IndexBufferMemory;
         range[1].size = VK_WHOLE_SIZE;

         vkFlushMappedMemoryRanges(v->Device, 2, range);
         vkUnmapMemory(v->Device, rb->VertexBufferMemory);
         vkUnmapMemory(v->Device, rb->IndexBufferMemory);
      }

      // Setup desired Vulkan state
      ImGui_ImplVulkan_SetupRenderState(draw_data, pipeline, command_buffer, rb, fb_width, fb_height);

      // Will project scissor/clipping rectangles into framebuffer space
      ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
      ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

      // Render command lists
      // (Because we merged all buffers into a single one, we maintain our own offset into them)
      int global_vtx_offset = 0;
      int global_idx_offset = 0;
      for (int n = 0; n < draw_data->CmdListsCount; n++)
      {
         const ImDrawList* cmd_list = draw_data->CmdLists[n];
         for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
         {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != nullptr)
            {
               // User callback, registered via ImDrawList::AddCallback()
               // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
               if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                  ImGui_ImplVulkan_SetupRenderState(draw_data, pipeline, command_buffer, rb, fb_width, fb_height);
               else
                  pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
               // Project scissor/clipping rectangles into framebuffer space
               ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x, (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
               ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x, (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);

               // Clamp to viewport as vkCmdSetScissor() won't accept values that are off bounds
               if (clip_min.x < 0.0f) { clip_min.x = 0.0f; }
               if (clip_min.y < 0.0f) { clip_min.y = 0.0f; }
               if (clip_max.x > fb_width) { clip_max.x = (float)fb_width; }
               if (clip_max.y > fb_height) { clip_max.y = (float)fb_height; }
               if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                  continue;

               // Apply scissor/clipping rectangle
               VkRect2D scissor;
               scissor.offset.x = (int32_t)(clip_min.x);
               scissor.offset.y = (int32_t)(clip_min.y);
               scissor.extent.width = (uint32_t)(clip_max.x - clip_min.x);
               scissor.extent.height = (uint32_t)(clip_max.y - clip_min.y);
               vkCmdSetScissor(command_buffer, 0, 1, &scissor);

               // Bind DescriptorSet with font or user texture
               VkDescriptorSet desc_set[1] = {(VkDescriptorSet)pcmd->TextureId};
               if (sizeof(ImTextureID) < sizeof(ImU64))
               {
                  // We don't support texture switches if ImTextureID hasn't been redefined to be 64-bit. Do a flaky check that other textures haven't been used.
                  IM_ASSERT(pcmd->TextureId == (ImTextureID)bd->FontDescriptorSet);
                  desc_set[0] = bd->FontDescriptorSet;
               }
               vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, bd->PipelineLayout, 0, 1, desc_set, 0, nullptr);

               // Draw
               vkCmdDrawIndexed(command_buffer, pcmd->ElemCount, 1, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset, 0);
            }
         }

         global_idx_offset += cmd_list->IdxBuffer.Size;
         global_vtx_offset += cmd_list->VtxBuffer.Size;
      }

      // Note: at this point both vkCmdSetViewport() and vkCmdSetScissor() have been called.
      // Our last values will leak into user/application rendering IF:
      // - Your app uses a pipeline with VK_DYNAMIC_STATE_VIEWPORT or VK_DYNAMIC_STATE_SCISSOR dynamic state
      // - And you forgot to call vkCmdSetViewport() and vkCmdSetScissor() yourself to explicitly set that state.
      // If you use VK_DYNAMIC_STATE_VIEWPORT or VK_DYNAMIC_STATE_SCISSOR you are responsible for setting the values before rendering.
      // In theory we should aim to backup/restore those values but I am not sure this is possible.
      // We perform a call to vkCmdSetScissor() to set back a full viewport which is likely to fix things for 99% users but technically this is not perfect. (See github #4644)
      VkRect2D scissor = {{ 0, 0 }, { (uint32_t)fb_width, (uint32_t)fb_height }};
      vkCmdSetScissor(command_buffer, 0, 1, &scissor);
   }*/
}

/// React on environmental change                                             
void GUISystem::Refresh() {

}





/*


static ImGuiKey ImGui_ImplGlfw_KeyToImGuiKey(int key)
{
   switch (key)
   {
   case GLFW_KEY_TAB: return ImGuiKey_Tab;
   case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
   case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
   case GLFW_KEY_UP: return ImGuiKey_UpArrow;
   case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
   case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
   case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
   case GLFW_KEY_HOME: return ImGuiKey_Home;
   case GLFW_KEY_END: return ImGuiKey_End;
   case GLFW_KEY_INSERT: return ImGuiKey_Insert;
   case GLFW_KEY_DELETE: return ImGuiKey_Delete;
   case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
   case GLFW_KEY_SPACE: return ImGuiKey_Space;
   case GLFW_KEY_ENTER: return ImGuiKey_Enter;
   case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
   case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
   case GLFW_KEY_COMMA: return ImGuiKey_Comma;
   case GLFW_KEY_MINUS: return ImGuiKey_Minus;
   case GLFW_KEY_PERIOD: return ImGuiKey_Period;
   case GLFW_KEY_SLASH: return ImGuiKey_Slash;
   case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
   case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
   case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
   case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
   case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
   case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
   case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
   case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
   case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
   case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
   case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
   case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
   case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
   case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
   case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
   case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
   case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
   case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
   case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
   case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
   case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
   case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
   case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
   case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
   case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
   case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
   case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
   case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
   case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
   case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
   case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
   case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
   case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
   case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
   case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
   case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
   case GLFW_KEY_MENU: return ImGuiKey_Menu;
   case GLFW_KEY_0: return ImGuiKey_0;
   case GLFW_KEY_1: return ImGuiKey_1;
   case GLFW_KEY_2: return ImGuiKey_2;
   case GLFW_KEY_3: return ImGuiKey_3;
   case GLFW_KEY_4: return ImGuiKey_4;
   case GLFW_KEY_5: return ImGuiKey_5;
   case GLFW_KEY_6: return ImGuiKey_6;
   case GLFW_KEY_7: return ImGuiKey_7;
   case GLFW_KEY_8: return ImGuiKey_8;
   case GLFW_KEY_9: return ImGuiKey_9;
   case GLFW_KEY_A: return ImGuiKey_A;
   case GLFW_KEY_B: return ImGuiKey_B;
   case GLFW_KEY_C: return ImGuiKey_C;
   case GLFW_KEY_D: return ImGuiKey_D;
   case GLFW_KEY_E: return ImGuiKey_E;
   case GLFW_KEY_F: return ImGuiKey_F;
   case GLFW_KEY_G: return ImGuiKey_G;
   case GLFW_KEY_H: return ImGuiKey_H;
   case GLFW_KEY_I: return ImGuiKey_I;
   case GLFW_KEY_J: return ImGuiKey_J;
   case GLFW_KEY_K: return ImGuiKey_K;
   case GLFW_KEY_L: return ImGuiKey_L;
   case GLFW_KEY_M: return ImGuiKey_M;
   case GLFW_KEY_N: return ImGuiKey_N;
   case GLFW_KEY_O: return ImGuiKey_O;
   case GLFW_KEY_P: return ImGuiKey_P;
   case GLFW_KEY_Q: return ImGuiKey_Q;
   case GLFW_KEY_R: return ImGuiKey_R;
   case GLFW_KEY_S: return ImGuiKey_S;
   case GLFW_KEY_T: return ImGuiKey_T;
   case GLFW_KEY_U: return ImGuiKey_U;
   case GLFW_KEY_V: return ImGuiKey_V;
   case GLFW_KEY_W: return ImGuiKey_W;
   case GLFW_KEY_X: return ImGuiKey_X;
   case GLFW_KEY_Y: return ImGuiKey_Y;
   case GLFW_KEY_Z: return ImGuiKey_Z;
   case GLFW_KEY_F1: return ImGuiKey_F1;
   case GLFW_KEY_F2: return ImGuiKey_F2;
   case GLFW_KEY_F3: return ImGuiKey_F3;
   case GLFW_KEY_F4: return ImGuiKey_F4;
   case GLFW_KEY_F5: return ImGuiKey_F5;
   case GLFW_KEY_F6: return ImGuiKey_F6;
   case GLFW_KEY_F7: return ImGuiKey_F7;
   case GLFW_KEY_F8: return ImGuiKey_F8;
   case GLFW_KEY_F9: return ImGuiKey_F9;
   case GLFW_KEY_F10: return ImGuiKey_F10;
   case GLFW_KEY_F11: return ImGuiKey_F11;
   case GLFW_KEY_F12: return ImGuiKey_F12;
   default: return ImGuiKey_None;
   }
}

static int ImGui_ImplGlfw_KeyToModifier(int key)
{
   if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
      return GLFW_MOD_CONTROL;
   if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT)
      return GLFW_MOD_SHIFT;
   if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT)
      return GLFW_MOD_ALT;
   if (key == GLFW_KEY_LEFT_SUPER || key == GLFW_KEY_RIGHT_SUPER)
      return GLFW_MOD_SUPER;
   return 0;
}

static void ImGui_ImplGlfw_UpdateKeyModifiers(int mods)
{
   ImGuiIO& io = ImGui::GetIO();
   io.AddKeyEvent(ImGuiMod_Ctrl, (mods & GLFW_MOD_CONTROL) != 0);
   io.AddKeyEvent(ImGuiMod_Shift, (mods & GLFW_MOD_SHIFT) != 0);
   io.AddKeyEvent(ImGuiMod_Alt, (mods & GLFW_MOD_ALT) != 0);
   io.AddKeyEvent(ImGuiMod_Super, (mods & GLFW_MOD_SUPER) != 0);
}

void ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
   ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
   if (bd->PrevUserCallbackMousebutton != nullptr && window == bd->Window)
      bd->PrevUserCallbackMousebutton(window, button, action, mods);

   ImGui_ImplGlfw_UpdateKeyModifiers(mods);

   ImGuiIO& io = ImGui::GetIO();
   if (button >= 0 && button < ImGuiMouseButton_COUNT)
      io.AddMouseButtonEvent(button, action == GLFW_PRESS);
}

void ImGui_ImplGlfw_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
   ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
   if (bd->PrevUserCallbackScroll != nullptr && window == bd->Window)
      bd->PrevUserCallbackScroll(window, xoffset, yoffset);

   ImGuiIO& io = ImGui::GetIO();
   io.AddMouseWheelEvent((float)xoffset, (float)yoffset);
}

static int ImGui_ImplGlfw_TranslateUntranslatedKey(int key, int scancode)
{
#if GLFW_HAS_GETKEYNAME && !defined(__EMSCRIPTEN__)
   // GLFW 3.1+ attempts to "untranslate" keys, which goes the opposite of what every other framework does, making using lettered shortcuts difficult.
   // (It had reasons to do so: namely GLFW is/was more likely to be used for WASD-type game controls rather than lettered shortcuts, but IHMO the 3.1 change could have been done differently)
   // See https://github.com/glfw/glfw/issues/1502 for details.
   // Adding a workaround to undo this (so our keys are translated->untranslated->translated, likely a lossy process).
   // This won't cover edge cases but this is at least going to cover common cases.
   if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_EQUAL)
      return key;
   GLFWerrorfun prev_error_callback = glfwSetErrorCallback(nullptr);
   const char* key_name = glfwGetKeyName(key, scancode);
   glfwSetErrorCallback(prev_error_callback);
#if (GLFW_VERSION_COMBINED >= 3300) // Eat errors (see #5908)
   (void)glfwGetError(NULL);
#endif
   if (key_name && key_name[0] != 0 && key_name[1] == 0)
   {
      const char char_names[] = "`-=[]\\,;\'./";
      const int char_keys[] = {GLFW_KEY_GRAVE_ACCENT, GLFW_KEY_MINUS, GLFW_KEY_EQUAL, GLFW_KEY_LEFT_BRACKET, GLFW_KEY_RIGHT_BRACKET, GLFW_KEY_BACKSLASH, GLFW_KEY_COMMA, GLFW_KEY_SEMICOLON, GLFW_KEY_APOSTROPHE, GLFW_KEY_PERIOD, GLFW_KEY_SLASH, 0};
      IM_ASSERT(IM_ARRAYSIZE(char_names) == IM_ARRAYSIZE(char_keys));
      if (key_name[0] >= '0' && key_name[0] <= '9') { key = GLFW_KEY_0 + (key_name[0] - '0'); }
      else if (key_name[0] >= 'A' && key_name[0] <= 'Z') { key = GLFW_KEY_A + (key_name[0] - 'A'); }
      else if (key_name[0] >= 'a' && key_name[0] <= 'z') { key = GLFW_KEY_A + (key_name[0] - 'a'); }
      else if (const char* p = strchr(char_names, key_name[0])) { key = char_keys[p - char_names]; }
   }
   // if (action == GLFW_PRESS) printf("key %d scancode %d name '%s'\n", key, scancode, key_name);
#else
   IM_UNUSED(scancode);
#endif
   return key;
}

void ImGui_ImplGlfw_KeyCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods)
{
   ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
   if (bd->PrevUserCallbackKey != nullptr && window == bd->Window)
      bd->PrevUserCallbackKey(window, keycode, scancode, action, mods);

   if (action != GLFW_PRESS && action != GLFW_RELEASE)
      return;

   // Workaround: X11 does not include current pressed/released modifier key in 'mods' flags. https://github.com/glfw/glfw/issues/1630
   if (int keycode_to_mod = ImGui_ImplGlfw_KeyToModifier(keycode))
      mods = (action == GLFW_PRESS) ? (mods | keycode_to_mod) : (mods & ~keycode_to_mod);
   ImGui_ImplGlfw_UpdateKeyModifiers(mods);

   keycode = ImGui_ImplGlfw_TranslateUntranslatedKey(keycode, scancode);

   ImGuiIO& io = ImGui::GetIO();
   ImGuiKey imgui_key = ImGui_ImplGlfw_KeyToImGuiKey(keycode);
   io.AddKeyEvent(imgui_key, (action == GLFW_PRESS));
   io.SetKeyEventNativeData(imgui_key, keycode, scancode); // To support legacy indexing (<1.87 user code)
}

void ImGui_ImplGlfw_WindowFocusCallback(GLFWwindow* window, int focused)
{
   ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
   if (bd->PrevUserCallbackWindowFocus != nullptr && window == bd->Window)
      bd->PrevUserCallbackWindowFocus(window, focused);

   ImGuiIO& io = ImGui::GetIO();
   io.AddFocusEvent(focused != 0);
}

void ImGui_ImplGlfw_CursorPosCallback(GLFWwindow* window, double x, double y)
{
   ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
   if (bd->PrevUserCallbackCursorPos != nullptr && window == bd->Window)
      bd->PrevUserCallbackCursorPos(window, x, y);
   if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
      return;

   ImGuiIO& io = ImGui::GetIO();
   io.AddMousePosEvent((float)x, (float)y);
   bd->LastValidMousePos = ImVec2((float)x, (float)y);
}

// Workaround: X11 seems to send spurious Leave/Enter events which would make us lose our position,
// so we back it up and restore on Leave/Enter (see https://github.com/ocornut/imgui/issues/4984)
void ImGui_ImplGlfw_CursorEnterCallback(GLFWwindow* window, int entered)
{
   ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
   if (bd->PrevUserCallbackCursorEnter != nullptr && window == bd->Window)
      bd->PrevUserCallbackCursorEnter(window, entered);
   if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
      return;

   ImGuiIO& io = ImGui::GetIO();
   if (entered)
   {
      bd->MouseWindow = window;
      io.AddMousePosEvent(bd->LastValidMousePos.x, bd->LastValidMousePos.y);
   }
   else if (!entered && bd->MouseWindow == window)
   {
      bd->LastValidMousePos = io.MousePos;
      bd->MouseWindow = nullptr;
      io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
   }
}

void ImGui_ImplGlfw_CharCallback(GLFWwindow* window, unsigned int c)
{
   ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
   if (bd->PrevUserCallbackChar != nullptr && window == bd->Window)
      bd->PrevUserCallbackChar(window, c);

   ImGuiIO& io = ImGui::GetIO();
   io.AddInputCharacter(c);
}

void ImGui_ImplGlfw_MonitorCallback(GLFWmonitor*, int)
{
   // Unused in 'master' branch but 'docking' branch will use this, so we declare it ahead of it so if you have to install callbacks you can install this one too.
}

void ImGui_ImplGlfw_InstallCallbacks(GLFWwindow* window)
{
   ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
   IM_ASSERT(bd->InstalledCallbacks == false && "Callbacks already installed!");
   IM_ASSERT(bd->Window == window);

   bd->PrevUserCallbackWindowFocus = glfwSetWindowFocusCallback(window, ImGui_ImplGlfw_WindowFocusCallback);
   bd->PrevUserCallbackCursorEnter = glfwSetCursorEnterCallback(window, ImGui_ImplGlfw_CursorEnterCallback);
   bd->PrevUserCallbackCursorPos = glfwSetCursorPosCallback(window, ImGui_ImplGlfw_CursorPosCallback);
   bd->PrevUserCallbackMousebutton = glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
   bd->PrevUserCallbackScroll = glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
   bd->PrevUserCallbackKey = glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
   bd->PrevUserCallbackChar = glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
   bd->PrevUserCallbackMonitor = glfwSetMonitorCallback(ImGui_ImplGlfw_MonitorCallback);
   bd->InstalledCallbacks = true;
}

//-----------------------------------------------------------------------------
// SHADERS
//-----------------------------------------------------------------------------

// glsl_shader.vert, compiled with:
// # glslangValidator -V -x -o glsl_shader.vert.u32 glsl_shader.vert

#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec4 aColor;
layout(push_constant) uniform uPushConstant { vec2 uScale; vec2 uTranslate; } pc;

out gl_PerVertex { vec4 gl_Position; };
layout(location = 0) out struct { vec4 Color; vec2 UV; } Out;

void main()
{
    Out.Color = aColor;
    Out.UV = aUV;
    gl_Position = vec4(aPos * pc.uScale + pc.uTranslate, 0, 1);
}

static uint32_t __glsl_shader_vert_spv[] =
{
    0x07230203,0x00010000,0x00080001,0x0000002e,0x00000000,0x00020011,0x00000001,0x0006000b,
    0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
    0x000a000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000b,0x0000000f,0x00000015,
    0x0000001b,0x0000001c,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
    0x00000000,0x00030005,0x00000009,0x00000000,0x00050006,0x00000009,0x00000000,0x6f6c6f43,
    0x00000072,0x00040006,0x00000009,0x00000001,0x00005655,0x00030005,0x0000000b,0x0074754f,
    0x00040005,0x0000000f,0x6c6f4361,0x0000726f,0x00030005,0x00000015,0x00565561,0x00060005,
    0x00000019,0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,0x00000019,0x00000000,
    0x505f6c67,0x7469736f,0x006e6f69,0x00030005,0x0000001b,0x00000000,0x00040005,0x0000001c,
    0x736f5061,0x00000000,0x00060005,0x0000001e,0x73755075,0x6e6f4368,0x6e617473,0x00000074,
    0x00050006,0x0000001e,0x00000000,0x61635375,0x0000656c,0x00060006,0x0000001e,0x00000001,
    0x61725475,0x616c736e,0x00006574,0x00030005,0x00000020,0x00006370,0x00040047,0x0000000b,
    0x0000001e,0x00000000,0x00040047,0x0000000f,0x0000001e,0x00000002,0x00040047,0x00000015,
    0x0000001e,0x00000001,0x00050048,0x00000019,0x00000000,0x0000000b,0x00000000,0x00030047,
    0x00000019,0x00000002,0x00040047,0x0000001c,0x0000001e,0x00000000,0x00050048,0x0000001e,
    0x00000000,0x00000023,0x00000000,0x00050048,0x0000001e,0x00000001,0x00000023,0x00000008,
    0x00030047,0x0000001e,0x00000002,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,
    0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040017,
    0x00000008,0x00000006,0x00000002,0x0004001e,0x00000009,0x00000007,0x00000008,0x00040020,
    0x0000000a,0x00000003,0x00000009,0x0004003b,0x0000000a,0x0000000b,0x00000003,0x00040015,
    0x0000000c,0x00000020,0x00000001,0x0004002b,0x0000000c,0x0000000d,0x00000000,0x00040020,
    0x0000000e,0x00000001,0x00000007,0x0004003b,0x0000000e,0x0000000f,0x00000001,0x00040020,
    0x00000011,0x00000003,0x00000007,0x0004002b,0x0000000c,0x00000013,0x00000001,0x00040020,
    0x00000014,0x00000001,0x00000008,0x0004003b,0x00000014,0x00000015,0x00000001,0x00040020,
    0x00000017,0x00000003,0x00000008,0x0003001e,0x00000019,0x00000007,0x00040020,0x0000001a,
    0x00000003,0x00000019,0x0004003b,0x0000001a,0x0000001b,0x00000003,0x0004003b,0x00000014,
    0x0000001c,0x00000001,0x0004001e,0x0000001e,0x00000008,0x00000008,0x00040020,0x0000001f,
    0x00000009,0x0000001e,0x0004003b,0x0000001f,0x00000020,0x00000009,0x00040020,0x00000021,
    0x00000009,0x00000008,0x0004002b,0x00000006,0x00000028,0x00000000,0x0004002b,0x00000006,
    0x00000029,0x3f800000,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,
    0x00000005,0x0004003d,0x00000007,0x00000010,0x0000000f,0x00050041,0x00000011,0x00000012,
    0x0000000b,0x0000000d,0x0003003e,0x00000012,0x00000010,0x0004003d,0x00000008,0x00000016,
    0x00000015,0x00050041,0x00000017,0x00000018,0x0000000b,0x00000013,0x0003003e,0x00000018,
    0x00000016,0x0004003d,0x00000008,0x0000001d,0x0000001c,0x00050041,0x00000021,0x00000022,
    0x00000020,0x0000000d,0x0004003d,0x00000008,0x00000023,0x00000022,0x00050085,0x00000008,
    0x00000024,0x0000001d,0x00000023,0x00050041,0x00000021,0x00000025,0x00000020,0x00000013,
    0x0004003d,0x00000008,0x00000026,0x00000025,0x00050081,0x00000008,0x00000027,0x00000024,
    0x00000026,0x00050051,0x00000006,0x0000002a,0x00000027,0x00000000,0x00050051,0x00000006,
    0x0000002b,0x00000027,0x00000001,0x00070050,0x00000007,0x0000002c,0x0000002a,0x0000002b,
    0x00000028,0x00000029,0x00050041,0x00000011,0x0000002d,0x0000001b,0x0000000d,0x0003003e,
    0x0000002d,0x0000002c,0x000100fd,0x00010038
};

// glsl_shader.frag, compiled with:
// # glslangValidator -V -x -o glsl_shader.frag.u32 glsl_shader.frag

#version 450 core
layout(location = 0) out vec4 fColor;
layout(set=0, binding=0) uniform sampler2D sTexture;
layout(location = 0) in struct { vec4 Color; vec2 UV; } In;
void main()
{
    fColor = In.Color * texture(sTexture, In.UV.st);
}

static uint32_t __glsl_shader_frag_spv[] =
{
    0x07230203,0x00010000,0x00080001,0x0000001e,0x00000000,0x00020011,0x00000001,0x0006000b,
    0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
    0x0007000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000d,0x00030010,
    0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
    0x00000000,0x00040005,0x00000009,0x6c6f4366,0x0000726f,0x00030005,0x0000000b,0x00000000,
    0x00050006,0x0000000b,0x00000000,0x6f6c6f43,0x00000072,0x00040006,0x0000000b,0x00000001,
    0x00005655,0x00030005,0x0000000d,0x00006e49,0x00050005,0x00000016,0x78655473,0x65727574,
    0x00000000,0x00040047,0x00000009,0x0000001e,0x00000000,0x00040047,0x0000000d,0x0000001e,
    0x00000000,0x00040047,0x00000016,0x00000022,0x00000000,0x00040047,0x00000016,0x00000021,
    0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,
    0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,
    0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,0x00040017,0x0000000a,0x00000006,
    0x00000002,0x0004001e,0x0000000b,0x00000007,0x0000000a,0x00040020,0x0000000c,0x00000001,
    0x0000000b,0x0004003b,0x0000000c,0x0000000d,0x00000001,0x00040015,0x0000000e,0x00000020,
    0x00000001,0x0004002b,0x0000000e,0x0000000f,0x00000000,0x00040020,0x00000010,0x00000001,
    0x00000007,0x00090019,0x00000013,0x00000006,0x00000001,0x00000000,0x00000000,0x00000000,
    0x00000001,0x00000000,0x0003001b,0x00000014,0x00000013,0x00040020,0x00000015,0x00000000,
    0x00000014,0x0004003b,0x00000015,0x00000016,0x00000000,0x0004002b,0x0000000e,0x00000018,
    0x00000001,0x00040020,0x00000019,0x00000001,0x0000000a,0x00050036,0x00000002,0x00000004,
    0x00000000,0x00000003,0x000200f8,0x00000005,0x00050041,0x00000010,0x00000011,0x0000000d,
    0x0000000f,0x0004003d,0x00000007,0x00000012,0x00000011,0x0004003d,0x00000014,0x00000017,
    0x00000016,0x00050041,0x00000019,0x0000001a,0x0000000d,0x00000018,0x0004003d,0x0000000a,
    0x0000001b,0x0000001a,0x00050057,0x00000007,0x0000001c,0x00000017,0x0000001b,0x00050085,
    0x00000007,0x0000001d,0x00000012,0x0000001c,0x0003003e,0x00000009,0x0000001d,0x000100fd,
    0x00010038
};

static void ImGui_ImplVulkan_SetupRenderState(ImDrawData* draw_data, VkPipeline pipeline, VkCommandBuffer command_buffer, ImGui_ImplVulkanH_FrameRenderBuffers* rb, int fb_width, int fb_height)
{
   ImGui_ImplVulkan_Data* bd = ImGui_ImplVulkan_GetBackendData();

   // Bind pipeline:
   {
      vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
   }

   // Bind Vertex And Index Buffer:
   if (draw_data->TotalVtxCount > 0)
   {
      VkBuffer vertex_buffers[1] = {rb->VertexBuffer};
      VkDeviceSize vertex_offset[1] = {0};
      vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, vertex_offset);
      vkCmdBindIndexBuffer(command_buffer, rb->IndexBuffer, 0, sizeof(ImDrawIdx) == 2 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32);
   }

   // Setup viewport:
   {
      VkViewport viewport;
      viewport.x = 0;
      viewport.y = 0;
      viewport.width = (float)fb_width;
      viewport.height = (float)fb_height;
      viewport.minDepth = 0.0f;
      viewport.maxDepth = 1.0f;
      vkCmdSetViewport(command_buffer, 0, 1, &viewport);
   }

   // Setup scale and translation:
   // Our visible imgui space lies from draw_data->DisplayPps (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). 
   // DisplayPos is (0,0) for single viewport apps.
   {
      float scale[2];
      scale[0] = 2.0f / draw_data->DisplaySize.x;
      scale[1] = 2.0f / draw_data->DisplaySize.y;
      float translate[2];
      translate[0] = -1.0f - draw_data->DisplayPos.x * scale[0];
      translate[1] = -1.0f - draw_data->DisplayPos.y * scale[1];
      vkCmdPushConstants(command_buffer, bd->PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, sizeof(float) * 0, sizeof(float) * 2, scale);
      vkCmdPushConstants(command_buffer, bd->PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, sizeof(float) * 2, sizeof(float) * 2, translate);
   }
}

static void ImGui_ImplVulkan_CreateShaderModules(VkDevice device, const VkAllocationCallbacks* allocator)
{
   // Create the shader modules
   ImGui_ImplVulkan_Data* bd = ImGui_ImplVulkan_GetBackendData();
   if (bd->ShaderModuleVert == VK_NULL_HANDLE)
   {
      VkShaderModuleCreateInfo vert_info = {};
      vert_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
      vert_info.codeSize = sizeof(__glsl_shader_vert_spv);
      vert_info.pCode = (uint32_t*)__glsl_shader_vert_spv;
      VkResult err = vkCreateShaderModule(device, &vert_info, allocator, &bd->ShaderModuleVert);
      check_vk_result(err);
   }
   if (bd->ShaderModuleFrag == VK_NULL_HANDLE)
   {
      VkShaderModuleCreateInfo frag_info = {};
      frag_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
      frag_info.codeSize = sizeof(__glsl_shader_frag_spv);
      frag_info.pCode = (uint32_t*)__glsl_shader_frag_spv;
      VkResult err = vkCreateShaderModule(device, &frag_info, allocator, &bd->ShaderModuleFrag);
      check_vk_result(err);
   }
}
*/