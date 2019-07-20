



set(SOURCES
  sdl_screen.cpp
  sdl_sound.cpp
  sdl_control.cpp
)


include_directories(./ ../ /usr/include/SDL2)
add_library("sdlglu" STATIC ${SOURCES})
target_link_libraries("SDL2")




