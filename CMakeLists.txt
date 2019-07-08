cmake_minimum_required(VERSION 2.8)
project(gamebaby)


set(CMAKE_VERBOSE_MAKEFILE ON)


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set(CCACHE_CMD "CCACHE_DIR=/tmp/ccachedir ccache")

  if(SLOW)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "sleep 1;  ${CCACHE_CMD}")
  else()
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_CMD}")
  endif()
endif(CCACHE_FOUND)




if(OPT)
endif()


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


set(CMAKE_C_COMPILER  "/usr/local/bin/gcc")
set(CMAKE_C_FLAGS   "-std=c1z -Werror")
set(CMAKE_CXX_COMPILER  "/usr/local/bin/g++")
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror")
set(CMAKE_CXX_LINK_FLAGS "-lSDL2 -lSDL2main -lstdc++")


set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS}   -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS}   -O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} -O0 -g")


if(EMSC)
#  set(PRELOADS "--preload-file ")


  set(EXECUTABLE_EXTENSION ".html")
  set(CMAKE_CXX_COMPILER  em++)
  set(CMAKE_CXX_FLAGS "-Werror -std=c++1z -Wno-switch -Wno-unused-command-line-argument -Os")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS}")
  set(EMSC_PORTS "-s -s DISABLE_EXCEPTION_CATCHING=0 -s USE_SDL=2 -s USE_ZLIB=1")
  set(CMAKE_CXX_LINK_FLAGS  "${CMAKE_CXX_LINK_FLAGS} --shell-file ../shell.html ${PRELOADS} ${EMSC_PORTS}") 
endif()




find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
#  set(CMAKE_CXX_COMPILER_LAUNCHER "sleep 2; CCACHE_DIR=/tmp/ccachedir ccache  ${CMAKE_CXX_COMPILER}")
endif(CCACHE_FOUND)




set(SDL_SOURCES
  sdl_screen.cpp
  sdl_control.cpp
  sdl_sound.cpp
)


add_subdirectory(libgbstd)
#add_subdirectory(libonch)
add_subdirectory(libge)
add_subdirectory(libgbpng)
#add_subdirectory(libsubiso)
#add_subdirectory(libww)
add_subdirectory(libmkfont)


include_directories(./)
include_directories(/usr/include/SDL2)
add_executable("test_sdl${EXECUTABLE_EXTENSION}" test_sdl.cpp ${SDL_SOURCES})
#add_executable("ww_sdl${EXECUTABLE_EXTENSION}" ww_sdl.cpp ${SDL_SOURCES})
add_executable("mkfont_sdl${EXECUTABLE_EXTENSION}" mkfont_sdl.cpp ${SDL_SOURCES})
#add_executable("mksnd_sdl${EXECUTABLE_EXTENSION}" mksnd_sdl.cpp ${SDL_SOURCES})
#add_executable("vgi_sdl${EXECUTABLE_EXTENSION}" vgi_sdl.cpp ${SDL_SOURCES})
#add_executable("waview_sdl${EXECUTABLE_EXTENSION}" waview_sdl.cpp ${SDL_SOURCES})
#add_executable("earth_maker_sdl${EXECUTABLE_EXTENSION}" earth_maker_sdl.cpp ${SDL_SOURCES})
#add_executable("tbgloop_sdl${EXECUTABLE_EXTENSION}" tbgloop_sdl.cpp ${SDL_SOURCES})
#add_executable("png2lit${EXECUTABLE_EXTENSION}" png2lit.cpp)
add_executable("mkicon_sdl${EXECUTABLE_EXTENSION}" mkicon_sdl.cpp ${SDL_SOURCES})
add_executable("mkanigra_sdl${EXECUTABLE_EXTENSION}" mkanigra_sdl.cpp ${SDL_SOURCES})
#add_executable("mkbg_sdl${EXECUTABLE_EXTENSION}" mkbg_sdl.cpp ${SDL_SOURCES})
#add_executable("mkeyesquat${EXECUTABLE_EXTENSION}" mkeyesquat.cpp ${SDL_SOURCES})
#target_link_libraries("ww_sdl${EXECUTABLE_EXTENSION}" "ww" "gbstd" "gbpng" "z")
target_link_libraries("mkfont_sdl${EXECUTABLE_EXTENSION}" "mkfont" "gbstd" "gbpng" "z")
#target_link_libraries("png2lit${EXECUTABLE_EXTENSION}" "onch" "gbstd" "gbpng" "z")
target_link_libraries("test_sdl${EXECUTABLE_EXTENSION}" "gbstd" "gbpng" "z")
#target_link_libraries("mksnd_sdl${EXECUTABLE_EXTENSION}" "onch" "gbstd" "gbpng" "z")
#target_link_libraries("vgi_sdl${EXECUTABLE_EXTENSION}" "onch" "gbstd" "gbpng" "z")
#target_link_libraries("waview_sdl${EXECUTABLE_EXTENSION}" "onch" "gbstd" "gbpng" "z")
#target_link_libraries("earth_maker_sdl${EXECUTABLE_EXTENSION}" "subiso" "gbstd" "gbpng" "z")
#target_link_libraries("tbgloop_sdl${EXECUTABLE_EXTENSION}" "gbstd" "gbpng" "z")
#target_link_libraries("mkeyesquat${EXECUTABLE_EXTENSION}" "gbstd" "gbpng" "z")
target_link_libraries("mkicon_sdl${EXECUTABLE_EXTENSION}" "ge" "gbstd" "gbpng" "z")
target_link_libraries("mkanigra_sdl${EXECUTABLE_EXTENSION}" "ge" "gbstd" "gbpng" "z")
#target_link_libraries("mkbg_sdl${EXECUTABLE_EXTENSION}" "ge" "gbstd" "gbpng" "z")


if(EMSC)
endif()




