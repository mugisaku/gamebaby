#include"libgbstd/widget.hpp"




using namespace gbstd;




struct
character
{
  static constexpr int  m_size = 8;

  char16_t  m_unicode;

  uint8_t  m_data[m_size];

  void  render(const canvas&  cv, int  pixel_size) const noexcept;

};


struct
combined
{
  char16_t  m_unicode;

  char16_t  m_upper;
  char16_t  m_lower;

};


extern std::vector<character>  g_characters;
extern std::vector<combined>   g_combineds;

extern std::reference_wrapper<character>  g_current_character;
extern std::reference_wrapper<combined>   g_current_combined;


character&  get_character_by_unicode(char16_t  unicode) noexcept;

void  initialize(std::string_view  path) noexcept;



struct editor;


class
bitmap: public widgets::node
{
  static constexpr int  m_pixel_size = 12;

  editor&  m_editor;

  point  m_cursor_position;

public:
  bitmap(editor&  e) noexcept;

  void  on_mouse_act(point  pt) noexcept override;

  void  render(const canvas&  cv) noexcept override;

};


class
character_selector: public widgets::node
{
  static constexpr int  m_x_number = 8;
  static constexpr int  m_y_number = 8;

  static constexpr int  m_pixel_size = 2;

  editor&  m_editor;

  point  m_cursor_position;

  int  m_offset=0;

public:
  character_selector(editor&  e) noexcept;

  void    up() noexcept{if(m_offset){m_offset -= 2;  request_redraw();}}
  void  down() noexcept{            {m_offset += 2;  request_redraw();}}

  void  set_offset(int  v) noexcept{  m_offset = v;  request_redraw();}

  void  on_mouse_act(point  pt) noexcept override;

  void  render(const canvas&  cv) noexcept override;

};


class
combined_selector: public widgets::node
{
  static constexpr int  m_x_number = 24;
  static constexpr int  m_y_number = 12;

  static constexpr int  m_pixel_size = 1;

  editor&  m_editor;

  int  m_offset=0;

public:
  combined_selector(editor&  e) noexcept;

  void    up() noexcept{  if(m_offset){m_offset -= 2;}  request_redraw();}
  void  down() noexcept{              {m_offset += 2;}  request_redraw();}

  void  render(const canvas&  cv) noexcept override;

};


struct
editor
{
  bitmap  m_bitmap;
  character_selector  m_chrsel;
  combined_selector   m_cmbsel;

  widgets::label&  m_character_label;
  widgets::label&  m_selected_label;

  widgets::slider&  m_character_table_slider;

  widgets::button&  m_character_up_button;
  widgets::button&  m_character_down_button;

  widgets::button&  m_combined_up_button;
  widgets::button&  m_combined_down_button;

  widgets::button&  m_shift_up_button;
  widgets::button&  m_shift_left_button;
  widgets::button&  m_shift_right_button;
  widgets::button&  m_shift_down_button;

  widgets::button&  m_save_button;
  widgets::button&  m_save_as_combined_button;

  editor(widgets::operating_node&  root) noexcept;

  void  update() noexcept;

};





