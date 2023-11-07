#pragma once

#include "Assets.h"

#include "CatWare/Graphics/Renderer/Texture.h"
#include "CatWare/Audio/Audio.h"

namespace CatWare
{
    class TextureAsset : public Asset<Rendering::Texture2D>
    {
    public:
        TextureAsset( std::string path ) : Asset( path ) { }

        ~TextureAsset( )
        {
            Unload( Get( ) );
        }

        Rendering::Texture2D* Load( std::string path ) override
        {
            CW_ENGINE_LOG->Info( "Loading texture from %s", path.c_str( ) );

            isLoaded = true;
            return Rendering::Texture2D::Create( path, Rendering::TextureFilter::LINEAR );
        }

        void Unload( Rendering::Texture2D* asset ) override
        {
            CW_ENGINE_LOG->Info( "Unloading texture %s", GetPath( ).c_str( ) );

            isLoaded = false;
            delete asset;
        }
    };

    class SoundAsset : public Asset<Sound>
    {
    public:
        SoundAsset( std::string path ) : Asset( path ) { }
        ~SoundAsset( ) { Unload( Get( ) ); }

        Sound* Load( std::string path )
        {
            CW_ENGINE_LOG->Info( "Loading sound from %s", path.c_str( ) );
            isLoaded = true;

            return new Sound( path );
        }

        void Unload( Sound* asset )
        {
            CW_ENGINE_LOG->Info( "Unloading sound %s", GetPath( ).c_str( ) );
            isLoaded = false;

            delete asset;
        }

        bool IsInUse( Sound* asset ) override
        {
            return ( asset->useCount > 0 );
        }
    };
}