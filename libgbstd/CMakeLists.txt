

set(SOURCES
  math.cpp
  random.cpp
  sha256.cpp
  sns/sns_record.cpp
  sns/sns_record_table.cpp
  sns/sns_platform.cpp
  sns/sns_article.cpp
  sns/sns_account.cpp
  sns/sns_account_observer.cpp
  sns/sns_timeline.cpp
  sns/sns_timeline_view.cpp
  sns/sns_timeline_node.cpp
  sns/sns_tester.cpp
  sns/sns_index_list.cpp
  parsers/token.cpp
  parsers/token_block.cpp
  parsers/tokenizer.cpp
  parsers/tokenizer__skip.cpp
  parsers/tokenizer__read_number.cpp
  characters/unicode.cpp
  characters/font.cpp
  characters/text.cpp
  characters/string_set.cpp
  characters/string_form.cpp
  images/image.cpp
  images/image__png.cpp
  images/canvas.cpp
  images/canvas__draw.cpp
  images/canvas__draw_string.cpp
  images/liner.cpp
  tile_maps/tile_source.cpp
  tile_maps/tile_map.cpp
  tile_maps/tile_map_view.cpp
  values/object.cpp
  values/array.cpp
  values/list.cpp
  widgets/operating_node.cpp
  widgets/node.cpp
  widgets/node_iterator.cpp
  widgets/label.cpp
  widgets/button.cpp
  widgets/frame.cpp
  widgets/icon.cpp
  widgets/iconshow.cpp
  widgets/dial.cpp
  widgets/checkbox.cpp
  widgets/radio_button.cpp
#  widgets/menu_handler.cpp
  widgets/slider.cpp
  file_ops/file_op.cpp
  file_ops/riff_chunk.cpp
  file_ops/riff_subchunk_view.cpp
  file_ops/wave.cpp
  file_ops/wave_format.cpp
  file_ops/ogg_page.cpp
  sounds/sound_device.cpp
  sounds/sine_wave_device.cpp
  sounds/square_wave_device.cpp
  sounds/triangle_wave_device.cpp
  sounds/sawtooth_wave_device.cpp
  sounds/noise_device.cpp
  sounds/short_noise_device.cpp
  sounds/scale_table.cpp
  control_execution.cpp
  control_asm_element.cpp
  control_program_space.cpp
  control_program_execution.cpp
  gpfs_directory.cpp
  gpfs_node.cpp
  gpfs_node_reference.cpp
  gpfs_path.cpp
  vms/vm_function.cpp
  vms/vm_context.cpp
  vms/vm_context__operate.cpp
  vms/vm_value.cpp
  vms/vm_operand.cpp
  vms/vm_operation.cpp
  vms/vm_codeline.cpp
  typesystems/struct_type_info.cpp
  typesystems/union_type_info.cpp
  typesystems/enum_type_info.cpp
  typesystems/type_info.cpp
  typesystems/type_entry.cpp
  typesystems/type_collection.cpp
  typesystems/parameter_list.cpp
  ilc_javascript.cpp
  utility.cpp
  video.cpp
  video_window.cpp
  gbstd.cpp
)


include_directories(./)
include_directories(../)
add_library("gbstd" STATIC ${SOURCES})




