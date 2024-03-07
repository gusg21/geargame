set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES

    editor/asseteditor.c
    editor/codeeditor.c
    editor/editor.c
    editor/ggwidgets.c
    editor/luaconsole.c
    editor/outputconsole.c

    actor.c
    actorspec.c
    assets.c
    camera.c
    cimgui_impl_raylib.c
    grid.c
    keys.c
    ldefs.c
    log.c
    memory.c
    scene.c
    scenespec.c
    scripting.c
    state.c
    texture.c
    tiled.c
    tiledobject.c
    tileset.c
    transform.c
    vector.c
    window.c
)

add_library(gglib STATIC

    ${SOURCE_FILES}

    )

add_subdirectory(lua54)
target_link_libraries(gglib lua54)

add_subdirectory(raylib)
target_link_libraries(gglib raylib)

add_subdirectory(parson)
target_link_libraries(gglib parson)

set(IMGUI_STATIC ON)
add_subdirectory(cimgui)
target_link_libraries(gglib cimgui)

# rlImGui is now poorly packaged into raylib :3
# add_subdirectory(rlImGui)
# target_link_libraries(gglib rlImGui)