cmake_minimum_required (VERSION 2.6)
project (Arcade)

# Version number
set (Arcade_VERSION_MAJOR 0)
set (Arcade_VERSION_MINOR 1)

# configuration header
# configure_file(
#   "${PROJECT_SOURCE_DIR}/include/config.hpp.in"
#   "${PROJECT_BINARY_DIR}/config.hpp"
#   )
# include_directories(
#   "${PROJECT_SOURCE_DIR}/include"
#   "${PROJECT_BINARY_DIR}"
#   )

# compile options
add_compile_options(-std=c++1y)
add_compile_options(-O2)
add_compile_options(-Wall -Werror)
add_compile_options(-Wextra -Wno-unused-parameter -Wno-missing-field-initializers)

# library dependencies
find_package(PkgConfig REQUIRED)
macro(target_find_pkg target name message)
  pkg_search_module(${name} ${name})
  if (${name}_FOUND)
    target_include_directories(${target} PRIVATE ${${name}_INCLUDE_DIRS})
    target_link_libraries(${target} PRIVATE ${${name}_LIBRARIES})
    link_directories(${${name}_LIBRARY_DIRS})
  else()
    message(FATAL_ERROR ${message})
  endif ()
endmacro ()

add_executable(arcade
  events.cpp
  background.cpp
  bullet.cpp
  comet.cpp
  explosion.cpp
  gameobject.cpp
  main.cpp
  spaceship.cpp
  sprite.cpp
  phi.cpp
  menuview.cpp
  gameview.cpp
  lostview.cpp
  )

target_find_pkg(arcade sdl2 "Unable to find sdl2")
target_find_pkg(arcade SDL2_image "Unable to find SDL2_image")
target_find_pkg(arcade SDL2_ttf "Unable to find SDL2_ttf")
target_find_pkg(arcade SDL2_mixer "Unable to find SDL2_mixer")
