#ifndef ge_hpp_was_included
#define ge_hpp_was_included


#include"libgbstd/widget.hpp"
#include"libgbstd/file_op.hpp"
#include"libge/core.hpp"


namespace ge{




class
aniview: public gbstd::widgets::node
{
  core&  m_core;

  int  m_index=0;

  uint32_t  m_delay_time=0;
  uint32_t  m_next_time=0;

  gbstd::widgets::label&  m_state_label;

  std::vector<gbstd::point>  m_points;

  static void   psh(gbstd::widgets::button_event  evt) noexcept;
  static void   pop(gbstd::widgets::button_event  evt) noexcept;
  static void  dial_callback(gbstd::widgets::dial_event  evt) noexcept;

public:
  aniview(gbstd::widgets::operating_node&  root, core&  cor) noexcept;

  void  clear() noexcept;
  void  check() noexcept;

  void  process_before_reform() noexcept override;

  void  save_as_apng(const char*  filename) const noexcept;

  void  update_state_label() noexcept;

  void  render(const gbstd::canvas&  cv) noexcept override;

  gbstd::widgets::node&  create_save_button(gbstd::widgets::operating_node&  root) noexcept;

};


/*
class
seamless_pattern_view: public gbstd::widgets::node
{

public:
  seamless_pattern_view(context&  ctx) noexcept;

  void  process_before_reform() noexcept override;

  void  render(const gbstd::canvas&  cv) noexcept override;

};



class
previewer: public gbstd::widgets::node
{
  gbstd::canvas  m_a_base;
  gbstd::canvas  m_b_base;
  gbstd::canvas  m_top_edge;
  gbstd::canvas  m_bottom_edge;
  gbstd::canvas  m_left_edge;
  gbstd::canvas  m_right_edge;

public:
  previewer(const gbstd::image&  img) noexcept;

  void  process_before_reform() noexcept override;

  void  render_product(gbstd::point  pt, int  flags, const gbstd::canvas&  cv) noexcept;

  void  render(const gbstd::canvas&  cv) noexcept override;

};
*/



}


#endif



