



set(SOURCES
  png_file_wrapper.cpp
  png_binary.cpp
  png_binary_view.cpp
  png_binary_cursor.cpp
  png__bitpack.cpp
  png__unbitpack.cpp
  png__unfilter.cpp
  png__filter.cpp
  png_chunk_list.cpp
  png_chunk_set.cpp
  png_chunk.cpp
  png_chunk__calculate_crc.cpp
  png_palette.cpp
  png_transparency_info.cpp
  png_background_info.cpp
  png_indexed_color_image.cpp
  png_direct_color_image.cpp
  png_image.cpp
  png_image_data.cpp
  png_image_header.cpp
  png_animation_control.cpp
  png_frame_control.cpp
  png_frame_data.cpp
  png_animation_builder.cpp
)


include_directories(./)
include_directories(../)
add_library("gbpng" STATIC ${SOURCES})
#add_executable(test_png_rw test_png_rw)
#add_executable(view_png  view_png.cpp)
#add_executable(mkapng  mkapng.cpp)
#target_link_libraries(test_png_rw "gbpng" "z")
#target_link_libraries(mkapng "gbpng" "z")
#target_link_libraries(view_png "gbpng" "z" "SDL2" "SDL2main")




