add_rules("mode.debug", "mode.release")

add_requires(
	"libsdl",
	"freetype",
	"soloud",
	"box2d"
)

option("buildTestScript")
	set_default(true)
    set_showmenu(true)

option("useRuntime")
	set_default(true)
	set_showmenu(true)

target("CatWareEngine")
    set_kind("shared")
	set_targetdir("Build/")

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

    add_packages(
		"libsdl",
		"freetype",
		"soloud", 
		"box2d"
	)

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


if has_config("buildTestScript") then
	target("TestScript")
		set_kind("shared")
		set_targetdir("WorkDir/Scripts")

		set_group("scripts")

		set_languages("cxx20")

		add_packages("libsdl", "freetype", "soloud", "box2d")

		add_defines("CW_PLATFORM_WIN64")
		add_deps("CatWareEngine")
		add_includedirs("Engine/Source/")

		add_includedirs("Engine/Lib/glm")
		add_includedirs("Engine/Lib/ImGui")

		add_files("TestScript/Source/**.cpp")
		add_headerfiles("TestScript/Source/**.h")
end

if has_config("useRuntime") then
	target("Runtime")
		set_kind("binary")
		set_targetdir("Build/")
		set_basename("game_$(mode)_$(arch)")

		set_languages("cxx20")

		add_defines("CW_PLATFORM_WIN64")
		add_deps("CatWareEngine")

		if has_config("buildTestScript") then
			add_deps("TestScript")
		end

		add_packages("libsdl", "freetype", "soloud", "box2d")

		set_rundir("$(projectdir)/WorkDir")

		add_includedirs("Engine/Lib/glm")
		add_includedirs("Engine/Lib/ImGui")

		add_includedirs("Engine/Source/")

		add_files("Runtime/Source/**.cpp")
		add_headerfiles("Runtime/Source/**.h")

end
