#ifndef ge_color_manager_hpp_was_included
#define ge_color_manager_hpp_was_included


#include"libgbstd/widget.hpp"
#include"libgbstd/file_op.hpp"


namespace ge{




class color_holder;


class
color_maker: public gbstd::widgets::node
{
  class sample;

  gbstd::widgets::dial&  m_r_dial;
  gbstd::widgets::dial&  m_g_dial;
  gbstd::widgets::dial&  m_b_dial;

  sample&  m_sample;

  gbstd::color&  m_color;

  static void  update_color_internal(gbstd::widgets::dial_event  evt) noexcept;

  void  build_color() const noexcept;

public:
  color_maker(gbstd::widgets::operating_node&  root, gbstd::color&  color) noexcept;

  gbstd::color  get_color() const noexcept{return m_color;}

  void  update() const noexcept;

};


class
color_holder: public gbstd::widgets::node
{
  int  m_index=0;

  std::vector<gbstd::color>  m_colors;

  gbstd::color&  m_color;

public:
  color_holder(int  size, gbstd::color&  color) noexcept;
  color_holder(std::initializer_list<gbstd::color>  colors, gbstd::color&  color) noexcept;

  color_holder&  set_color(gbstd::color  col) noexcept;

  void  render(const gbstd::canvas&  cv) noexcept override;

  void  on_mouse_act(gbstd::point  mouse_pos) noexcept override;

};


struct
color_handler
{
  gbstd::widgets::node&   maker;
  gbstd::widgets::node&  holder;

};


color_handler  make_color_handler(gbstd::widgets::operating_node&  root, gbstd::color&  color) noexcept;




}


#endif



