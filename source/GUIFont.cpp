///                                                                           
/// Langulus::Module::ImGui                                                   
/// Copyright(C) 2022 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "GUIItem.hpp"
#include "GUI.hpp"
#include <Math/Color.hpp>
#include <Flow/Verbs/Interpret.hpp>


/// GUI item construction                                                     
///   @param producer - the system producer                                   
///   @param descriptor - instructions for configuring the font               
GUIFont::GUIFont(GUISystem* producer, const Neat& descriptor)
   : A::UI::Unit {MetaOf<GUIFont>()}
   , ProducedFrom {producer, descriptor} {
   VERBOSE_GUI("Initializing...");

   // Font filename/system name to load                                 
   Text filename;
   SeekValueAux<Traits::Name>(descriptor, filename);
   LANGULUS_ASSERT(filename, Construct,
      "Empty system name/filename for font");
   filename = filename.Terminate();

   // Font size                                                         
   float size;
   if (not SeekTraitAux<Traits::Size>(descriptor, size)) {
      // Not found, use default                                         
      size = 16.0f;
   }

   /* 3rd parameter, consider these
struct ImFontConfig {
    void*           FontData;               //          // TTF/OTF data
    int             FontDataSize;           //          // TTF/OTF data size
    bool            FontDataOwnedByAtlas;   // true     // TTF/OTF data ownership taken by the container ImFontAtlas (will delete memory itself).
    int             FontNo;                 // 0        // Index of font within TTF/OTF file
    float           SizePixels;             //          // Size in pixels for rasterizer (more or less maps to the resulting font height).
    int             OversampleH;            // 3        // Rasterize at higher quality for sub-pixel positioning. Note the difference between 2 and 3 is minimal so you can reduce this to 2 to save memory. Read https://github.com/nothings/stb/blob/master/tests/oversample/README.md for details.
    int             OversampleV;            // 1        // Rasterize at higher quality for sub-pixel positioning. This is not really useful as we don't use sub-pixel positions on the Y axis.
    bool            PixelSnapH;             // false    // Align every glyph to pixel boundary. Useful e.g. if you are merging a non-pixel aligned font with the default font. If enabled, you can set OversampleH/V to 1.
    ImVec2          GlyphExtraSpacing;      // 0, 0     // Extra spacing (in pixels) between glyphs. Only X axis is supported for now.
    ImVec2          GlyphOffset;            // 0, 0     // Offset all glyphs from this font input.
    const ImWchar*  GlyphRanges;            // NULL     // THE ARRAY DATA NEEDS TO PERSIST AS LONG AS THE FONT IS ALIVE. Pointer to a user-provided list of Unicode range (2 value per range, values are inclusive, zero-terminated list).
    float           GlyphMinAdvanceX;       // 0        // Minimum AdvanceX for glyphs, set Min to align font icons, set both Min/Max to enforce mono-space font
    float           GlyphMaxAdvanceX;       // FLT_MAX  // Maximum AdvanceX for glyphs
    bool            MergeMode;              // false    // Merge into previous ImFont, so you can combine multiple inputs font into one ImFont (e.g. ASCII font + icons + Japanese glyphs). You may want to use GlyphOffset.y when merge font of different heights.
    unsigned int    FontBuilderFlags;       // 0        // Settings for custom font builder. THIS IS BUILDER IMPLEMENTATION DEPENDENT. Leave as zero if unsure.
    float           RasterizerMultiply;     // 1.0f     // Brighten (>1.0f) or darken (<1.0f) font output. Brightening small fonts may be a good workaround to make them more readable.
    ImWchar         EllipsisChar;           // -1       // Explicitly specify unicode codepoint of ellipsis character. When fonts are being merged first specified ellipsis will be used.

    // [Internal]
    char            Name[40];               // Name (strictly to ease debugging)
    ImFont*         DstFont;

    IMGUI_API ImFontConfig();
}; */

   /* 4th parameter, consider these
      GetGlyphRangesCyrillic
      GetGlyphRangesThai
      GetGlyphRangesVietnamese
      GetGlyphRangesJapanese
      GetGlyphRangesChineseSimplifiedCommon
      GetGlyphRangesChineseFull
      GetGlyphRangesKorean
      GetGlyphRangesGreek
      GetGlyphRangesDefault*/

   // Create the font                                                   
   // If the file cannot be loaded, the function will return NULL       
   auto io = producer->GetIO()->Fonts;

   if (filename.Lowercase() == "default")
      mFont = io->AddFontDefault();
   else
      mFont = io->AddFontFromFileTTF(
         filename.GetRaw(), size, NULL, io->GetGlyphRangesCyrillic()
      );

   LANGULUS_ASSERT(mFont, Construct,
      "Couldn't AddFontFromFileTTF: ", filename);

   // The fonts will be rasterized at a given size (w/ oversampling)    
   // and stored into a texture when calling ImFontAtlas::Build() or    
   // GetTexDataAsXXXX()                                                
   // Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use  
   // Freetype for higher quality font rendering.                       
   // Read 'docs/FONTS.md' for more instructions and details.           
   
   // Lets generate and upload the texture to the content system and    
   // VRAM                                                              
   unsigned char* fontData;
   int fontAtlasWidth, fontAtlasHeight;
   io->GetTexDataAsRGBA32(&fontData, &fontAtlasWidth, &fontAtlasHeight);

   auto pixelCount = static_cast<Count>(fontAtlasWidth * fontAtlasHeight);
   Text fontName {"Font ", filename, ' ', size};
   Verbs::Create createTexture {
      Construct::From<A::Image>(
         Traits::Name {fontName},
         Traits::Path {filename},
         Traits::Size {fontAtlasWidth, fontAtlasHeight},
         Traits::Data {
            Block {{}, MetaOf<RGBA>(), pixelCount, fontData}
         }
      )
   };

   // Store atlas and identifier                                        
   mAtlas = RunIn(createTexture)->As<A::Image*>();
   io->SetTexID(mAtlas->GetGPUHandle());


   //ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
   {
      /*
      ImGui_ImplVulkan_Data* bd = ImGui_ImplVulkan_GetBackendData();
      ImGui_ImplVulkan_InitInfo* v = &bd->VulkanInitInfo;

      // Create the Image:
      {
         VkImageCreateInfo info = {};
         info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
         info.imageType = VK_IMAGE_TYPE_2D;
         info.format = VK_FORMAT_R8G8B8A8_UNORM;
         info.extent.width = width;
         info.extent.height = height;
         info.extent.depth = 1;
         info.mipLevels = 1;
         info.arrayLayers = 1;
         info.samples = VK_SAMPLE_COUNT_1_BIT;
         info.tiling = VK_IMAGE_TILING_OPTIMAL;
         info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
         info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         vkCreateImage(v->Device, &info, v->Allocator, &bd->FontImage);
         VkMemoryRequirements req;
         vkGetImageMemoryRequirements(v->Device, bd->FontImage, &req);
         VkMemoryAllocateInfo alloc_info = {};
         alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
         alloc_info.allocationSize = req.size;
         alloc_info.memoryTypeIndex = ImGui_ImplVulkan_MemoryType(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, req.memoryTypeBits);
         vkAllocateMemory(v->Device, &alloc_info, v->Allocator, &bd->FontMemory);
         vkBindImageMemory(v->Device, bd->FontImage, bd->FontMemory, 0);
      }

      // Create the Image View:
      {
         VkImageViewCreateInfo info = {};
         info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
         info.image = bd->FontImage;
         info.viewType = VK_IMAGE_VIEW_TYPE_2D;
         info.format = VK_FORMAT_R8G8B8A8_UNORM;
         info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         info.subresourceRange.levelCount = 1;
         info.subresourceRange.layerCount = 1;
         vkCreateImageView(v->Device, &info, v->Allocator, &bd->FontView);
      }

      // Create the Descriptor Set:
      bd->FontDescriptorSet = (VkDescriptorSet)ImGui_ImplVulkan_AddTexture(bd->FontSampler, bd->FontView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

      // Create the Upload Buffer:
      {
         VkBufferCreateInfo buffer_info = {};
         buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
         buffer_info.size = upload_size;
         buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
         buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         vkCreateBuffer(v->Device, &buffer_info, v->Allocator, &bd->UploadBuffer);
         VkMemoryRequirements req;
         vkGetBufferMemoryRequirements(v->Device, bd->UploadBuffer, &req);
         bd->BufferMemoryAlignment = (bd->BufferMemoryAlignment > req.alignment) ? bd->BufferMemoryAlignment : req.alignment;
         VkMemoryAllocateInfo alloc_info = {};
         alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
         alloc_info.allocationSize = req.size;
         alloc_info.memoryTypeIndex = ImGui_ImplVulkan_MemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, req.memoryTypeBits);
         vkAllocateMemory(v->Device, &alloc_info, v->Allocator, &bd->UploadBufferMemory);
         vkBindBufferMemory(v->Device, bd->UploadBuffer, bd->UploadBufferMemory, 0);
      }

      // Upload to Buffer:
      {
         char* map = nullptr;
         vkMapMemory(v->Device, bd->UploadBufferMemory, 0, upload_size, 0, (void**)(&map));
         memcpy(map, pixels, upload_size); //upload generated pixels from imgui
         VkMappedMemoryRange range[1] = {};
         range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
         range[0].memory = bd->UploadBufferMemory;
         range[0].size = upload_size;
         vkFlushMappedMemoryRanges(v->Device, 1, range);
         vkUnmapMemory(v->Device, bd->UploadBufferMemory);
      }

      // Copy to Image:
      {
         VkImageMemoryBarrier copy_barrier[1] = {};
         copy_barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
         copy_barrier[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
         copy_barrier[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         copy_barrier[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
         copy_barrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
         copy_barrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
         copy_barrier[0].image = bd->FontImage;
         copy_barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         copy_barrier[0].subresourceRange.levelCount = 1;
         copy_barrier[0].subresourceRange.layerCount = 1;
         vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, copy_barrier);

         VkBufferImageCopy region = {};
         region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         region.imageSubresource.layerCount = 1;
         region.imageExtent.width = width;
         region.imageExtent.height = height;
         region.imageExtent.depth = 1;
         vkCmdCopyBufferToImage(command_buffer, bd->UploadBuffer, bd->FontImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

         VkImageMemoryBarrier use_barrier[1] = {};
         use_barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
         use_barrier[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
         use_barrier[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
         use_barrier[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
         use_barrier[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
         use_barrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
         use_barrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
         use_barrier[0].image = bd->FontImage;
         use_barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         use_barrier[0].subresourceRange.levelCount = 1;
         use_barrier[0].subresourceRange.layerCount = 1;
         vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, use_barrier);
      }

      // Store our identifier
      io->SetTexID((ImTextureID)bd->FontDescriptorSet);*/
   }
   VERBOSE_GUI("Initialized");
}