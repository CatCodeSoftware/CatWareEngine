### CatWare Engine
CatWare is a general purpouse game engine developed by the one and only PointThink and used by Cat Code Software to build games.

## Features
- OpenGL rendering
- Smart resource managment - Automatic loading and cleanup
- Font rendering
- Class hierarchy entity system - Because entity component systems are for losers
- Primitive logging system

## Planned features
- Full map editor
- Animation system
- Audio with support for DSP effects
- Save/Load system for scenes
- More advanced resource managment
- Support for multiple rendering APIs like Vulkan or Direct3D

## Potential features
These features may or may not get included in the future
- Scripting with C#, Lua or other languages
- Full Unity/Godot style editor - Big project, probably won't happen
- Full 3D support

## Building
CatWare uses xmake to generate projects. Install xmake here https://xmake.io
To generate a Visual Studio solution open your terminal and run ```xmake project -k vsxmake2022 -m "debug;release"```

## Docs
Documentiation is localized in the docs folder in the repository