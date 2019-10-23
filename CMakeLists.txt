cmake_minimum_required(VERSION 2.8)
project(gamebaby)


set(CMAKE_VERBOSE_MAKEFILE ON)


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set(CCACHE_CMD "CCACHE_DIR=/tmp/ccachedir ccache")

  if(SLOW)
message("I am sleepy.")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "sleep 1;  ${CCACHE_CMD}")
  else()
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_CMD}")
  endif()
endif(CCACHE_FOUND)




if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


set(CMAKE_C_FLAGS   "-std=c1z -Werror")
set(CMAKE_C_LINK_FLAGS   "")
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror")


set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS}   -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS}   -O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} -O0 -g")


if(EMSC)
#  set(PRELOADS "--preload-file ")


  set(CMAKE_CXX_COMPILER  /usr/local/emsdk/fastcomp/emscripten/em++)
  set(CMAKE_CXX_FLAGS "-Werror -std=c++1z -Wno-switch -Wno-unused-command-line-argument -Os")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS}")
  set(EMSC_PORTS "-s USE_SDL=2 -s USE_ZLIB=1")
  set(CMAKE_CXX_LINK_FLAGS  "${EMSC_PORTS}") 

  set(BASIC_LIBS "")
else()
  set(BASIC_LIBS "z" "SDL2" "SDL2main" "stdc++")
endif()




add_subdirectory(libsdlglu)
add_subdirectory(libgbstd)
#add_subdirectory(libonch)
#add_subdirectory(libge)
add_subdirectory(libgbpng)
#add_subdirectory(libsubiso)
#add_subdirectory(libww)
#add_subdirectory(libmkfont)


set(CHILDREN
#  children/children.cpp
# children/FallBlockPazzle/FallBlockPazzle_context.cpp
#  children/FallBlockPazzle/FallBlockPazzle_space.cpp
# children/FallBlockPazzle/FallBlockPazzle_stage.cpp
)


include_directories(./ /usr/local/include)
link_directories(/usr/local/lib)

add_executable("test_sdl${EXECUTABLE_EXTENSION}" test_sdl.cpp)
add_executable("ks_sdl${EXECUTABLE_EXTENSION}" ks_sdl.cpp "${CHILDREN}")
#add_executable("ww_sdl${EXECUTABLE_EXTENSION}" ww_sdl.cpp)
#add_executable("mkfont_sdl${EXECUTABLE_EXTENSION}" mkfont_sdl.cpp)
#add_executable("mksnd_sdl${EXECUTABLE_EXTENSION}" mksnd_sdl.cpp)
#add_executable("vgi_sdl${EXECUTABLE_EXTENSION}" vgi_sdl.cpp)
#add_executable("waview_sdl${EXECUTABLE_EXTENSION}" waview_sdl.cpp)
#add_executable("earth_maker_sdl${EXECUTABLE_EXTENSION}" earth_maker_sdl.cpp)
#add_executable("tbgloop_sdl${EXECUTABLE_EXTENSION}" tbgloop_sdl.cpp)
#add_executable("png2lit${EXECUTABLE_EXTENSION}" png2lit.cpp)
#add_executable("mkicon_sdl${EXECUTABLE_EXTENSION}" mkicon_sdl.cpp)
#add_executable("mkanigra_sdl${EXECUTABLE_EXTENSION}" mkanigra_sdl.cpp)
#add_executable("mkbg_sdl${EXECUTABLE_EXTENSION}" mkbg_sdl.cpp)
#add_executable("mkeyesquat${EXECUTABLE_EXTENSION}" mkeyesquat.cpp)
#add_executable("mkeyesquat2${EXECUTABLE_EXTENSION}" mkeyesquat2.cpp)

target_link_libraries("test_sdl${EXECUTABLE_EXTENSION}" "sdlglu" "gbstd" "gbpng" ${BASIC_LIBS})
target_link_libraries("ks_sdl${EXECUTABLE_EXTENSION}" "sdlglu" "gbstd" "gbpng" ${BASIC_LIBS})
#target_link_libraries("ww_sdl${EXECUTABLE_EXTENSION}" "sdlglu" "ww" "gbstd" "gbpng" ${BASIC_LIBS})
#target_link_libraries("mkfont_sdl${EXECUTABLE_EXTENSION}" "sdlglu" "mkfont" "gbstd" "gbpng" ${BASIC_LIBS})
#target_link_libraries("png2lit${EXECUTABLE_EXTENSION}" "sdlglu" "onch" "gbstd" "gbpng" ${BASIC_LIBS})
#target_link_libraries("mksnd_sdl${EXECUTABLE_EXTENSION}" "sdlglu" "onch" "gbstd" "gbpng" ${BASIC_LIBS})
#target_link_libraries("vgi_sdl${EXECUTABLE_EXTENSION}" "onch" "gbstd" "gbpng" ${BASIC_LIBS})
#target_link_libraries("waview_sdl${EXECUTABLE_EXTENSION}" "sdlglu" "onch" "gbstd" "gbpng" ${BASIC_LIBS})
#target_link_libraries("earth_maker_sdl${EXECUTABLE_EXTENSION}" "sdlglu" "subiso" "gbstd" "gbpng" ${BASIC_LIBS})
#target_link_libraries("tbgloop_sdl${EXECUTABLE_EXTENSION}" "sdlglu" "gbstd" "gbpng" ${BASIC_LIBS})
#target_link_libraries("mkeyesquat${EXECUTABLE_EXTENSION}" "gbpng" "z")
#target_link_libraries("mkeyesquat2${EXECUTABLE_EXTENSION}" "gbpng" "z")
#target_link_libraries("mkicon_sdl${EXECUTABLE_EXTENSION}" "sdlglu" "ge" "gbstd" "gbpng" ${BASIC_LIBS})
#target_link_libraries("mkanigra_sdl${EXECUTABLE_EXTENSION}" "sdlglu" "ge" "gbstd" "gbpng" ${BASIC_LIBS})
#target_link_libraries("mkbg_sdl${EXECUTABLE_EXTENSION}" "sdlglu" "ge" "gbstd" "gbpng" ${BASIC_LIBS})


if(EMSC)
endif()




