target("CatWareEngine")
    set_kind("shared")

	set_languages("cxx20")

    add_defines("CW_PLATFORM_WIN64", "CATWARE_BUILD")

	-- library include dirs
	add_includedirs("Engine/Lib/freetype/include")
	add_includedirs("Engine/Lib/glad/include")
	add_includedirs("Engine/Lib/glm")
	add_includedirs("Engine/Lib/ImGui")
	add_includedirs("Engine/Lib/ImGui/backends")
	add_includedirs("Engine/Lib/SDL2/include")
	add_includedirs("Engine/Lib/Single header")

	-- self include dir
	add_includedirs("Engine/Source/")

	-- library sources
	add_files("Engine/Lib/ImGui/*.cpp")
	add_files("Engine/Lib/ImGui/backends/imgui_impl_sdl2.cpp")
	add_files("Engine/Lib/ImGui/backends/imgui_impl_opengl3.cpp")
	add_files("Engine/Lib/glad/src/*.c")

    add_files("Engine/Source/CatWare/**.cpp")
    add_headerfiles("Engine/Source/**.h")


target("Sandbox")
	set_kind("binary")

    add_defines("CW_PLATFORM_WIN64")
	add_deps("CatWareEngine")
    
	add_includedirs("Engine/Lib/freetype/include")
	add_includedirs("Engine/Lib/glad/include")
	add_includedirs("Engine/Lib/glm")
	add_includedirs("Engine/Lib/ImGui")
	add_includedirs("Engine/Lib/ImGui/backends")
	add_includedirs("Engine/Lib/SDL2/include")
	add_includedirs("Engine/Lib/Single header")

    add_includedirs("Engine/Source/")

	add_files("Sandbox/Source/**.cpp")
    add_headerfiles("Engine/Source/**.h")
