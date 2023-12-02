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

# -----------------------------------------------------
# CatWareEngine
# -----------------------------------------------------
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE CW_SOURCE_FILES Engine/Source/*.cpp)
file(GLOB_RECURSE CW_HEADER_FILES Engine/Source/*.h)
file(GLOB_RECURSE CW_GLAD Engine/Lib/Glad/*.c)


source_group(CW_SOURCE_FILES Source)
source_group(CW_HEADER_FILES Source)

add_compile_definitions(CW_PLATFORM_WIN64)

# import all libraries
add_subdirectory("Engine/Lib/box2d")
link_libraries(box2d)
set_target_properties(box2d PROPERTIES FOLDER "Libraries")

add_subdirectory("Engine/Lib/freetype")
link_libraries(freetype)
set_target_properties(freetype PROPERTIES FOLDER "Libraries")

add_subdirectory("Engine/Lib/SDL2" EXCLUDE_FROM_ALL)
link_libraries(SDL2-static)
set_target_properties(SDL2-static PROPERTIES FOLDER "Libraries")

# group some stupid fucking dependencies that came out of fucking nowhere ( mostly box2d )
set_target_properties(glad PROPERTIES FOLDER "Libraries")
set_target_properties(glfw PROPERTIES FOLDER "Libraries")
set_target_properties(imgui PROPERTIES FOLDER "Libraries")
set_target_properties(sajson PROPERTIES FOLDER "Libraries")
set_target_properties(testbed PROPERTIES FOLDER "Libraries")
set_target_properties(unit_test PROPERTIES FOLDER "Libraries")
set_target_properties(sdl_headers_copy PROPERTIES FOLDER "Libraries")

include_directories(
	"Engine/Source"

	"Engine/Lib/SDL2/include"
	"Engine/Lib/freetype/include"
	
	"Engine/Lib/soloud/include"
	"Engine/Lib/box2d/include"
	
	"Engine/Lib/Glad/include"
	"Engine/Lib/glm"
	"Engine/Lib/ImGui"
	"Engine/Lib/ImGui/backends"
	"Engine/Lib/Single header"
)

add_link_options(
    /LARGEADDRESSAWARE
    /DEBUG
    $<$<CONFIG:DEBUG>:/INCREMENTAL> # Do incremental linking.
    $<$<CONFIG:DEBUG>:/OPT:NOREF> # No unreferenced data elimination.
    $<$<CONFIG:DEBUG>:/OPT:NOICF> # No Identical COMDAT folding.
)

add_library(CatWareEngine SHARED ${CW_SOURCE_FILES} ${CW_GLAD} ${CW_HEADER_FILES})
target_compile_definitions(CatWareEngine PRIVATE CATWARE_BUILD)

# test script
include_directories("Engine/Source/")
link_libraries(CatWareEngine)

add_library(TestScript SHARED TestScript/Source/Main.cpp)

set(CMAKE_CXX_STANDARD 20)

set_target_properties(TestScript PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/WorkDir/Scripts)
set_target_properties(TestScript PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/WorkDir/Scripts)


# -----------------------------------------------------
# Runtime
# -----------------------------------------------------


link_libraries(
	CatWareEngine
	TestScript
)	

file(GLOB_RECURSE RUNTIME_SOURCE_FILES Runtime/Source/*.cpp)
file(GLOB_RECURSE RUNTIME_HEADER_FILES Runtime/Source/*.h)

set(CMAKE_CXX_STANDARD 20)

include_directories(
	"Runtime/Source"
	"Engine/Source"
)

add_executable(Runtime ${RUNTIME_SOURCE_FILES} ${RUNTIME_HEADER_FILES})


GroupSourcesByFolder(CatWareEngine)
GroupSourcesByFolder(Runtime)