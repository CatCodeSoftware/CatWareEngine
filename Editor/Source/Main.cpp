#include <CatWare.h>

#include <imgui.h>

class MainEditor : public CatWare::Scene
{
public:
	MainEditor( )
	{

	}

	~MainEditor( )
	{

	}

	void Update( )
	{
		ImGui::Begin( "Ass" );

		CatWare::Rendering::Texture2D* viewportTexture = CatWare::Rendering::Renderer::GetViewportTexture( );

		ImGui::Image(
			( ImTextureID ) viewportTexture->GetTextureID( ),
			{ 100, 100 }
		);

		ImGui::End( );
	}

	void Draw( )
	{
		CatWare::Rendering::Renderer::Clear( { 40, 40, 40, 255 } );
		CatWare::Rendering::Renderer::DrawRect( { 20, 20 }, { 100, 100 }, { 255, 255, 255, 255 } );
	}
};

class Editor : public CatWare::Application
{
public:
	Editor( )
	{

	}

	~Editor( )
	{

	}

	void PostInit( )
	{
		SetScene( new MainEditor );
	}
};

CW_REGISTER_APP( Editor )