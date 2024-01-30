cmake_minimum_required(VERSION 3.20)

project(CatWare)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Stolen from https://github.com/TheLartians/GroupSourcesByFolder.cmake
function(GroupSourcesByFolder target)
  set(SOURCE_GROUP_DELIMITER "/")
  set(last_dir "")
  set(files "")

  get_target_property(sources ${target} SOURCES)

  foreach(file ${sources})
    file(RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}" ${file})
    get_filename_component(dir "${relative_file}" PATH)
    if(NOT "${dir}" STREQUAL "${last_dir}")
      if(files)
        source_group("${last_dir}" FILES ${files})
      endif()
      set(files "")
    endif()
    set(files ${files} ${file})
    set(last_dir "${dir}")
  endforeach()

  if(files)
    source_group("${last_dir}" FILES ${files})
  endif()
endfunction()

# ImGui
file(GLOB CW_IMGUI Engine/Lib/ImGui/*.cpp)


# -----------------------------------------------------
# CatWareEngine
# -----------------------------------------------------
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE CW_SOURCE_FILES Engine/Source/*.cpp)
file(GLOB_RECURSE CW_HEADER_FILES Engine/Source/*.h)
file(GLOB_RECURSE CW_GLAD Engine/Lib/Glad/*.c)

source_group(CW_SOURCE_FILES Source)
source_group(CW_HEADER_FILES Source)

# platforms
if (WIN32)
    add_compile_definitions(CW_PLATFORM_WIN64)
    add_compile_definitions()
else()
    # Not sure if i should call this platform linux or just generally unix
    # cause it probably also works on bsd and other unix like systems
    add_compile_definitions(CW_PLATFORM_UNIX64)
    add_compile_options(-fPIC)
endif()

add_library(CatWareEngine SHARED ${CW_SOURCE_FILES} ${CW_GLAD} ${CW_HEADER_FILES} ${CW_IMGUI} ${PROJECT_SOURCE_DIR}/Engine/Lib/ImGui/backends/imgui_impl_opengl3.cpp ${PROJECT_SOURCE_DIR}/Engine/Lib/ImGui/backends/imgui_impl_sdl2.cpp)

target_compile_definitions(CatWareEngine PRIVATE CATWARE_BUILD)

add_subdirectory("Engine/Lib/freetype")
target_link_libraries(CatWareEngine PRIVATE freetype)
set_target_properties(freetype PROPERTIES FOLDER "Libraries")

add_subdirectory("Engine/Lib/SDL2" EXCLUDE_FROM_ALL)
target_link_libraries(CatWareEngine PRIVATE SDL2-static)
set_target_properties(SDL2-static PROPERTIES FOLDER "Libraries")

add_subdirectory("Engine/Lib/yaml-cpp")
target_link_libraries(CatWareEngine PRIVATE yaml-cpp)
set_target_properties(yaml-cpp PROPERTIES FOLDER "Libraries")

set_target_properties(sdl_headers_copy PROPERTIES FOLDER "Libraries")


target_include_directories(
		CatWareEngine PUBLIC
		"Engine/Source"
		"Engine/Lib/freetype/include"
		"Engine/Lib/SDL2/include"
		"Engine/Lib/glm"
		"Engine/Lib/ImGui"
		"Engine/Lib/ImGui/backends"
		"Engine/Lib/miniaudio"
)

target_include_directories(
        CatWareEngine
        PRIVATE

        "Engine/Source"
        "Engine/Lib/yaml-cpp/include"
        "Engine/Lib/Glad/include"
        "Engine/Lib/Single header"
)


if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	target_compile_definitions(CatWareEngine PRIVATE CW_DEBUG)
endif()
# -----------------------------------------------------
# TestScript
# -----------------------------------------------------

option(buildTestScript "buildTestScript" OFF)

if (buildTestScript)
	include_directories("Engine/Source/")
	link_libraries(CatWareEngine)

	set(CMAKE_CXX_STANDARD 17)

	add_library(TestScript SHARED TestScript/Source/Main.cpp)

	target_include_directories(
		TestScript
		PRIVATE

		"Engine/Source"
	)


	set_target_properties(TestScript PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/WorkDir/Scripts)
	set_target_properties(TestScript PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/WorkDir/Scripts)
	set_target_properties(TestScript PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/WorkDir/Scripts)
	set_target_properties(TestScript PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/WorkDir/Scripts)

endif()



# -----------------------------------------------------
# Runtime
# -----------------------------------------------------

link_libraries(CatWareEngine)	

if (buildTestScript)
	link_libraries(TestScript)
endif()

file(GLOB_RECURSE RUNTIME_SOURCE_FILES Runtime/Source/*.cpp)
file(GLOB_RECURSE RUNTIME_HEADER_FILES Runtime/Source/*.h)

add_executable(Runtime ${RUNTIME_SOURCE_FILES} ${RUNTIME_HEADER_FILES})

target_include_directories(
	Runtime
	PRIVATE

	"Runtime/Source"
	"Engine/Source"
)


#[[
# -----------------------------------------------------
# Meowditor
# -----------------------------------------------------

file(GLOB_RECURSE EDITOR_SOURCE_FILES Editor/Source/*.cpp)
file(GLOB_RECURSE EDITOR_HEADER_FILES Editor/Source/*.h)

add_executable(Meowditor ${EDITOR_SOURCE_FILES} ${EDITOR_HEADER_FILES} ${CW_IMGUI} ${PROJECT_SOURCE_DIR}/Engine/Lib/ImGui/backends/imgui_impl_opengl3.cpp ${PROJECT_SOURCE_DIR}/Engine/Lib/ImGui/backends/imgui_impl_sdl2.cpp)

target_include_directories(
        Meowditor
        PRIVATE

        "Editor/Source"
		"Engine/Source"
)

GroupSourcesByFolder(CatWareEngine)
GroupSourcesByFolder(Meowditor)
GroupSourcesByFolder(Runtime)
#]]
