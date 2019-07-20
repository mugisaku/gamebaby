



set(SOURCES
  mkfont.cpp
  mkfont_bitmap.cpp
  mkfont_character_selector.cpp
  mkfont_combined_selector.cpp
)


include_directories(./)
include_directories(../)
add_library("mkfont" STATIC ${SOURCES})




