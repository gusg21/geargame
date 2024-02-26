set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES actor.c
    assets.c
    camera.c
    cimgui_impl_raylib.c
    editor.c
    grid.c
    keys.c
    ldefs.c
    log.c
    scene.c
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

add_subdirectory(cimgui)
target_link_libraries(gglib cimgui)

# rlImGui is now poorly packaged into raylib :3
# add_subdirectory(rlImGui)
# target_link_libraries(gglib rlImGui)