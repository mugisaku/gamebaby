#ifndef ww_manage_section_hpp_is_included
#define ww_manage_section_hpp_is_included


#include"libww/ww_character.hpp"
#include"libww/ww_bar.hpp"
#include"libww/ww_spilling_text.hpp"
#include"libww/ww_party.hpp"
#include"libgbstd/menu.hpp"
#include"libgbstd/character.hpp"
#include<list>
#include<vector>




namespace ww{




struct
manage_section
{
  uint32_t  m_money=0;

  uint32_t      m_empty_land=0;
  uint32_t      m_waste_land=0;
  uint32_t   m_building_land=0;
  uint32_t       m_farm_land=0;
  uint32_t     m_forest_land=0;
  uint32_t  m_reservoir_land=0;

  uint32_t  m_water=0;
  uint32_t  m_food=0;
  uint32_t  m_wood=0;
  uint32_t  m_fuel=0;


  gbstd::clock  m_clock;
  gbstd::task   m_task;

  gbstd::windows::stack  m_window_stack;

  gbstd::menus::table  m_first_menu_table;
  gbstd::menus::view   m_first_menu_view;

  gbstd::menus::table  m_second_menu_table;
  gbstd::menus::view   m_second_menu_view;

  gbstd::menus::table  m_third_menu_table;
  gbstd::menus::view   m_third_menu_view;

  gbstd::windows::object  m_face_window;
  gbstd::windows::object  m_text_window;


  static constexpr int  m_text_table_width  = 24;
  static constexpr int  m_text_table_height =  4;

  char16_t  m_text_buffer[1024];

  const char16_t*   m_text_read_pointer;
        char16_t*  m_text_write_pointer;

  gbstd::text   m_text;
  gbstd::color  m_text_color;

  int  m_text_cursor_position;

  int  m_face_animation_index;

  static void  charactick(manage_section&  man) noexcept;

  bool  has_remain_text() const noexcept{return *m_text_read_pointer;}

  void  initialize_menu() noexcept;

  void  startup(gbstd::execution&  exec) noexcept;

  void  clear_text_queue() noexcept;

  void  push_text(std::u16string_view  sv) noexcept;

  void  clear_and_push_text(std::u16string_view  sv) noexcept;

  void  pump_text() noexcept;

  manage_section(gbstd::execution&  exec) noexcept;

};


}




#endif




