#pragma once

#include "EditorViewport.h"

#include <imgui.h>

using namespace CatWare;

Vector2D DrawEditorViewport( Vector2D mousePos )
{
    Vector2D spaceAvailable;

    ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0, 0 ) );

    ImGui::Begin( "Viewport" );

    Rendering::Texture2D* frameBufferTexture = Renderer::GetRenderTarget( )->GetColorAttachment( );

    spaceAvailable = { ImGui::GetContentRegionAvail( ).x, ImGui::GetContentRegionAvail( ).y };
    ImGui::Image( ( ImTextureID ) frameBufferTexture->GetTextureID( ), ImGui::GetContentRegionAvail( ), ImVec2( 0, 1 ), ImVec2( 1, 0 ) );

    ImGui::End( );

    ImGui::PopStyleVar( );

    return spaceAvailable;
}