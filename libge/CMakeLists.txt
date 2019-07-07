

set(SOURCES
  color_handlers/color_maker.cpp
  color_handlers/color_holder.cpp
  color_handlers/color_handler.cpp
  cores/core.cpp
  cores/core_source.cpp
  cores/core_display.cpp
  cores/core_paint.cpp
  cores/core_paint__draw.cpp
  cores/core_paint__operate.cpp
  cores/core_paint__make_widget.cpp
  cores/drawing_recorder.cpp
  cores/underlay_stacker.cpp
  aniview.cpp
#  previewer.cpp
#  seamless_pattern_view.cpp
)


include_directories(./)
include_directories(../)
add_library("ge" STATIC ${SOURCES})




