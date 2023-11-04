add_rules("mode.debug", "mode.release")

add_requires("libsdl")
add_requires("freetype")
add_requires("soloud")

target("CatWareEngine")
    set_kind("shared")

	set_languages("cxx20")

    add_defines("CATWARE_BUILD")

	-- some config rules
	if is_plat("windows") then
		add_defines("CW_PLATFORM_WIN64")
	end

	if is_mode("debug") then
		add_defines("CW_DEBUG")
	end

	-- library include dirs
	add_includedirs("Engine/Lib/glad/include")
	add_includedirs("Engine/Lib/glm")
	add_includedirs("Engine/Lib/ImGui")
	add_includedirs("Engine/Lib/ImGui/backends")
	add_includedirs("Engine/Lib/Single header")

    add_packages("libsdl", "freetype", "soloud")

	-- self include dir
	add_includedirs("Engine/Source/")

	-- library sources
    add_files("Engine/Lib/stb_image.cpp")
	add_files("Engine/Lib/ImGui/*.cpp")
	add_files("Engine/Lib/ImGui/backends/imgui_impl_sdl2.cpp")
	add_files("Engine/Lib/ImGui/backends/imgui_impl_opengl3.cpp")
	add_files("Engine/Lib/glad/src/*.c")

    add_files("Engine/Source/**.cpp")
    add_headerfiles("Engine/Source/**.h")


target("Sandbox")
	set_kind("binary")

    add_defines("CW_PLATFORM_WIN64")
	add_deps("CatWareEngine")

    add_packages("libsdl", "freetype", "soloud")

	set_rundir("$(projectdir)/WorkDir")

	add_includedirs("Engine/Lib/glad/include")
	add_includedirs("Engine/Lib/glm")
	add_includedirs("Engine/Lib/ImGui")
	add_includedirs("Engine/Lib/ImGui/backends")
	add_includedirs("Engine/Lib/Single header")

    add_includedirs("Engine/Source/")

	add_files("Sandbox/Source/**.cpp")
