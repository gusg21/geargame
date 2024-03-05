# Gear Game Engine

![Gear Game Logo](./gh-images/factory.png)

This is a hacky little game engine I've been putting together in my free time using C99, Lua, and Raylib as well as a few support libraries for JSON as well as IMGUI support.

The code is (currently) poorly documented although it should be fairly structural and consistent. My primary goals with this project are asset management, editor development, scripting interfacing, and practicing memory management in C.

## TODOS

- ~~Cleanup!~~
  - ~~So many `malloc()`s and nowhere near enough `free()`s~~
- SDL2 migration/support
  - Ideally you could switch out the backend; most of the rendering code is generic.
- Documentation pass
  - Especially focusing on making sure functions are labeled and knotty bits of string manip and Lua stack operations are clear + readable.
- Asset saving to disk from the gg_assets_t structure in the editor
- Better scripting API for Lua
  - maybe including some built-in lua code that wraps the somewhat ugly C-style calls?
