cmake_minimum_required(VERSION 3.18.1)

if (NOT DEFINED MODULE_NAME)
    message(FATAL_ERROR "MODULE_NAME is not set")
else ()
    project(${MODULE_NAME})
endif ()

message("Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 20)

# Define OpenGL ES 3.0
add_definitions(-DIMGUI_IMPL_OPENGL_ES3)

set(LINKER_FLAGS "-ffixed-x18 -Wl,--hash-style=both")
set(C_FLAGS "-Werror=format -fdata-sections -ffunction-sections")
set(CXX_FLAGS "${CXX_FLAGS} -fno-exceptions -fno-rtti")

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(C_FLAGS "${C_FLAGS} -O2 -fvisibility=hidden -fvisibility-inlines-hidden")
    set(LINKER_FLAGS "${LINKER_FLAGS} -Wl,-exclude-libs,ALL -Wl,--gc-sections -Wl,--strip-all")
else ()
    set(C_FLAGS "${C_FLAGS} -O0")
endif ()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${C_FLAGS} ${CXX_FLAGS}")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_FLAGS}")

# Include directories
include_directories(
        xdl/include
        ImGui
        ImGui/backends
        Includes
        Includes/Dobby
        KittyMemory
        libs
)

# XDL sources
aux_source_directory(xdl xdl-src)

# ImGui sources
set(IMGUI_SRC
        ImGui/imgui.cpp
        ImGui/imgui_draw.cpp
        ImGui/imgui_tables.cpp
        ImGui/imgui_widgets.cpp
        ImGui/backends/imgui_impl_android.cpp
        ImGui/backends/imgui_impl_opengl3.cpp
)

# KittyMemory sources
set(KITTYMEMORY_SRC
        KittyMemory/MemoryPatch.cpp
        KittyMemory/KittyMemory.cpp
)

# Main sources
add_library(${MODULE_NAME} SHARED
        main.cpp
        ${xdl-src}
        ${IMGUI_SRC}
        ${KITTYMEMORY_SRC}
)

# Link libraries
target_link_libraries(${MODULE_NAME} 
        log
        android
        EGL
        GLESv3
        ${CMAKE_SOURCE_DIR}/libs/arm64-v8a/libdobby.a
)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
            COMMAND ${CMAKE_STRIP} --strip-all --remove-section=.comment "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib${MODULE_NAME}.so")
endif ()
