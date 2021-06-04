

set(SOURCES
  math.cpp
  random.cpp
  sha256.cpp
  tree_struct.cpp
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
  characters/code_text_iterator.cpp
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
#  values/object.cpp
#  values/array.cpp
#  values/list.cpp
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
  syntaxes/syntax_token.cpp
  syntaxes/syntax_token_iterator.cpp
  syntaxes/syntax_tokenizer.cpp
  syntaxes/syntax_tokenizer__read_number.cpp
  syntaxes/syntax_operand.cpp
  syntaxes/syntax_expression.cpp
  syntaxes/syntax_definition.cpp
  syntaxes/syntax_rule.cpp
  syntaxes/syntax_branch.cpp
  syntaxes/syntax_branch_element.cpp
  syntaxes/syntax_parser.cpp
  syntaxes/syntax_parser__lower.cpp
  vms/typesystems/struct.cpp
  vms/typesystems/union.cpp
  vms/typesystems/enum.cpp
  vms/typesystems/type_info.cpp
  vms/typesystems/function_signature.cpp
  vms/expressions/expression.cpp
  vms/expressions/postfix_element.cpp
  vms/expressions/operand.cpp
  vms/expressions/simple_value.cpp
  vms/irs/ir_operation.cpp
  vms/irs/ir_statement.cpp
  vms/irs/ir_function.cpp
  vms/irs/ir_global_space.cpp
  vms/irs/ir_value.cpp
  vms/irs/ir_memory.cpp
  vms/irs/ir__assemble.cpp
  vms/irs/ir_processor.cpp
#  vms/assemblies/asm_parameter.cpp
#  vms/assemblies/asm_line.cpp
#  vms/assemblies/asm_context.cpp
#  vms/assemblies/asm_processor.cpp
#  vms/spaces/function.cpp
#  vms/spaces/basic_space.cpp
#  vms/spaces/basic_space__read_type.cpp
#  vms/spaces/basic_space__read_statement.cpp
#  vms/spaces/global_space.cpp
#  vms/spaces/block_space.cpp
#  vms/spaces/space_node.cpp
#  vms/statements/statement.cpp
#  vms/statements/compile_expression.cpp
#  vms/statements/compile_primary_expression.cpp
#  vms/statements/compile_unary_expression.cpp
#  vms/statements/expression_statement.cpp
#  vms/statements/block_statement.cpp
#  vms/statements/return_statement.cpp
#  vms/statements/let_statement.cpp
#  vms/statements/label_statement.cpp
#  vms/statements/jump_statement.cpp
#  vms/statements/control_statement.cpp
#  vms/statements/if_statement.cpp
#  vms/statements/if_string_statement.cpp
#  vms/statements/for_statement.cpp
#  vms/statements/while_statement.cpp
#  vms/statements/switch_statement.cpp
#  vms/statements/case_statement.cpp
#  vms/compile_context.cpp
  ilc_javascript.cpp
  utility.cpp
  video.cpp
  video_window.cpp
  gbstd.cpp
)


include_directories(./)
include_directories(../)
add_library("gbstd" STATIC ${SOURCES})




