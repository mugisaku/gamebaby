

set(SOURCES
  onch_word.cpp
  onch_text.cpp
  onch_cell.cpp
  onch_table.cpp
  onch_element.cpp
  onch_space.cpp
)


include_directories(./)
include_directories(../)
add_library("onch" ${SOURCES})




