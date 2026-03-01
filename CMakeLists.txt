cmake_minimum_required(VERSION 3.16)
project(BlackholeSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

# ---- SFML ----
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# ---- ImGui ----
include(FetchContent)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.5
)
FetchContent_MakeAvailable(imgui)

# ---- imgui-sfml ----
FetchContent_Declare(imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG        master
)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_MakeAvailable(imgui-sfml)

# Pass SFML's include directories to imgui-sfml manually —
# it can't find them automatically when IMGUI_SFML_FIND_SFML is OFF
get_target_property(SFML_INCLUDE_DIRS SFML::Graphics INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(ImGui-SFML PUBLIC ${SFML_INCLUDE_DIRS})
target_link_libraries(ImGui-SFML PUBLIC SFML::Graphics SFML::Window SFML::System)

# ---- Main executable ----
add_executable(blackhole src/main.cpp)

target_include_directories(blackhole PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
)

target_link_libraries(blackhole
    SFML::Graphics
    SFML::Window
    SFML::System
    ImGui-SFML::ImGui-SFML
)