/**
* @file Assets.h
* 
* @brief Handles everything related to assets. Like automatic
* removal and getting asset by ID.
* 
* @author PointThink
*/

#pragma once

#include <string>
#include <unordered_map>

#include "CatWare/Core.h"
#include "CatWare/Log.h"
#include "CatWare/Types/Types.h"
#include "CatWare/Graphics/Renderer/Texture.h"
#include "CatWare/Graphics/Text.h"

namespace CatWare
{
    // Welcome to template hell

    template<typename AssetType>
    class Asset
    {
    public:
        Asset( std::string path )
        {
            this->path = path;
        }

        AssetType* Get( )
        {
            if ( !isLoaded )
                asset = Load( path );

            refrenceCount++;

            return asset;
        }

        inline std::string GetPath( ) { return path; }

        void UnloadIfUnused( )
        {
            if ( refrenceCount == 0 && isLoaded && !IsInUse( asset ) )
            {
                unusedFor++;

                if ( unusedFor > 200 )
                {
                    Unload( asset );
                    asset = nullptr;
                }

                
            }
            else
            {
                unusedFor = 0;
            }

            ResetRefrenceCount( );
        }

        inline void ResetRefrenceCount( )
        {
            refrenceCount = 0;
        }

        virtual AssetType* Load( std::string path ) = 0;
        virtual void Unload( AssetType* asset ) = 0;

        virtual bool IsInUse( AssetType* asset ) { return false; }

    protected:
        bool isLoaded = false; 

    private:
        unsigned int unusedFor = 0;

        std::string path;

        UInt16 refrenceCount = 0;
        AssetType* asset;
    };

    class TextureAsset : public Asset<Rendering::Texture2D>
    {
    public:
        TextureAsset( std::string path ) : Asset( path ) {} 

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

    /*
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
    */

    template<typename AssetClass, typename Asset>
    class AssetManager
    {
    public:
        AssetManager( )
        {

        }

        AssetManager( std::string fallbackPath )
        {
            Add( "fallback", fallbackPath );
        }

        Asset* GetAsset( std::string name )
        {
            auto iterator = assetMap.find( name );
            
            if ( iterator == assetMap.end( ) )
            {
                iterator = assetMap.find( "fallback" );
            }

            return iterator->second->Get( );
        }

        void Add( std::string name, std::string path )
        {
            assetMap.insert( { name, new AssetClass( path ) } );
        }

        void Remove( std::string name )
        {
            assetMap.erase( assetMap.find( name ) );
        }

        void RunCleanup( )
        {
            for ( auto keyVal : assetMap )
            {
                keyVal.second->UnloadIfUnused( );
            }
        }

        void Clear( )
        {
            for ( auto keyVal : assetMap )
            {
                delete keyVal.second;
            }

            assetMap.clear( );
        }

    private:
        std::unordered_map<std::string, AssetClass*> assetMap;
    };

    class CATWARE_API Assets
    {
    public:
        inline static AssetManager<TextureAsset, Rendering::Texture2D> textures;
        // static AssetManager<SoundAsset, Sound> sounds;
    };
}