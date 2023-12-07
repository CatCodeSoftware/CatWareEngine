#include <CatWare.h>
#include <CatWare/EntryPoint.h>

#include <imgui.h>

#include "EditorViewport.h"

using namespace CatWare;

class Editor : public Scene
{
public:
    Vector2D spaceAvailable;

    Editor( )
    {
    }

    void Draw() override
    {
        // Resize camera
        delete Renderer::camera2D;
        Renderer::camera2D = new OrthoCamera( spaceAvailable.x, spaceAvailable.y );

        Renderer::Clear( { 0, 0, 0, 255 } );

        Renderer::DrawRect( { 20, 20 }, { 30, 30 }, { 255, 255, 255, 255 } );
    }

    void DrawGUI() override
    {
        ImGui::DockSpaceOverViewport( ImGui::GetMainViewport() );

        ImGui::Begin( "Object properties" );

        ImGui::End( );

        ImGui::Begin( "Tools" );

        ImGui::End( );

        ImGui::Begin( "Tool properties" );

        ImGui::End( );

        spaceAvailable = DrawEditorViewport( { 0, 0 } );
    }
};

class Meowditor : public Application
{
public:
    Meowditor( )
    {
        initConfig.windowResizable = true;
    }

    void PostInit() override
    {
        SceneManager::SetScene( new Editor );
    }
};

CW_REGISTER_APP( Meowditor );