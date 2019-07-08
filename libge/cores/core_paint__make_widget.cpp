#include"libge/core.hpp"



namespace ge{




gbstd::widgets::node&
core_paint::
create_operation_widget(gbstd::widgets::operating_node&  root) noexcept
{
  auto&  undo_btn = root.create_button(u"Undo")
    .set_callback([](gbstd::widgets::button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core_paint>().undo();
      }
  });


  auto&  copy_btn = root.create_button(u"Copy")
    .set_callback([](gbstd::widgets::button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core_paint>().take_copy();
     }
  });


  auto&  rou_btn = root.create_button(u"Rotate ↑")
    .set_callback([](gbstd::widgets::button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core_paint>().shift_up(true);
     }
  });


  auto&  rol_btn = root.create_button(u"Rotate ←")
    .set_callback([](gbstd::widgets::button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core_paint>().shift_left(true);
     }
  });


  auto&  ror_btn = root.create_button(u"Rotate →")
    .set_callback([](gbstd::widgets::button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core_paint>().shift_right(true);
     }
  });


  auto&  rod_btn = root.create_button(u"Rotate ↓")
    .set_callback([](gbstd::widgets::button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core_paint>().shift_down(true);
     }
  });




  auto&  rvl_btn = root.create_button(u"Revolve →")
    .set_callback([](gbstd::widgets::button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core_paint>().revolve();
     }
  });

  auto&  rvh_btn = root.create_button(u"Reverse -")
    .set_callback([](gbstd::widgets::button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core_paint>().reverse_horizontally();
      }
  });

  auto&  rvv_btn = root.create_button(u"Reverse |")
    .set_callback([](gbstd::widgets::button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core_paint>().reverse_vertically();
      }
  });

  auto&  mir_btn = root.create_button(u"Mirror |")
    .set_callback([](gbstd::widgets::button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core_paint>().mirror_vertically();
      }
  });




  gbstd::widgets::set_userdata({
    rou_btn,
    rol_btn,
    ror_btn,
    rod_btn,
    undo_btn,
    copy_btn,
    rvl_btn,
    rvh_btn,
    rvv_btn,
    mir_btn},*this);

  auto&  ro_col = root.create_table_column({rou_btn,rol_btn,ror_btn,rod_btn});
  auto&  ed_row = root.create_table_row(   {undo_btn,copy_btn});
  auto&  tr_col = root.create_table_column({rvl_btn,rvh_btn,rvv_btn,mir_btn});

  auto&  row = root.create_table_row({ro_col,tr_col});

  return root.create_frame(u"operation").set_content(root.create_table_column({row,ed_row}));
}


gbstd::widgets::node&
core_paint::
create_tool_widget(gbstd::widgets::operating_node&  root) noexcept
{
  auto  cb = [](gbstd::widgets::checkbox_event  evt){
    auto&  pai = evt->get_common_userdata<core_paint>();

    pai.cancel_drawing();

      switch(evt->get_entry_number())
      {
    case(0): pai.change_mode_to_draw_dot();break;
    case(1): pai.change_mode_to_draw_line();break;
    case(2): pai.change_mode_to_draw_rectangle();break;
    case(3): pai.change_mode_to_fill_rectangle();break;
    case(4): pai.change_mode_to_fill_area();break;
    case(5): pai.change_mode_to_select();break;
    case(6): pai.change_mode_to_paste();break;
    case(7): pai.change_mode_to_layer();break;
      }
  };


  auto&  container = root.create_node();

  auto&  first = root.create_radio_button().set_string(u"draw dot");

  first.set_callback(cb)
    .set_common_userdata(*this)
    .check()
  ;


  container.add_child_as_column({
    first,
    root.create_radio_button(first).set_string(u"draw line"),
    root.create_radio_button(first).set_string(u"draw rectangle"),
    root.create_radio_button(first).set_string(u"fill rectangle"),
    root.create_radio_button(first).set_string(u"fill area"),
    root.create_radio_button(first).set_string(u"select"),
    root.create_radio_button(first).set_string(u"paste"),
    root.create_radio_button(first).set_string(u"layer")
  });


  return root.create_frame(u"paint").set_content(container);
}




}




