#include"libge/color_handler.hpp"




namespace ge{


class
color_maker::
sample: public gbstd::widgets::node
{
  static constexpr int  size = 32;

public:
  sample(color_maker&  cm) noexcept
  {
    set_content_width( size);
    set_content_height(size);
    set_userdata(cm);
  }

  void  render(const gbstd::canvas&  cv) noexcept override
  {
    cv.fill_rectangle(get_userdata<color_maker>().get_color(),0,0,size,size);
  }

};




color_maker::
color_maker(gbstd::widgets::operating_node&  root, gbstd::color&  color) noexcept:
m_r_dial(root.create_dial()),
m_g_dial(root.create_dial()),
m_b_dial(root.create_dial()),
m_sample(*new sample(*this)),
m_color(color)
{
  m_r_dial.set_max(7).set_callback(update_color_internal).set_userdata(*this);
  m_g_dial.set_max(7).set_callback(update_color_internal).set_userdata(*this);
  m_b_dial.set_max(7).set_callback(update_color_internal).set_userdata(*this);


  auto&  r_label = root.create_label();
  auto&  g_label = root.create_label();
  auto&  b_label = root.create_label();

  r_label.set_string(u"[R]");
  g_label.set_string(u"[G]");
  b_label.set_string(u"[B]");

  add_child_as_column({root.create_table_row({r_label,m_r_dial}),
                       root.create_table_row({g_label,m_g_dial}),
                       root.create_table_row({b_label,m_b_dial}),m_sample});

  update();
}




void
color_maker::
update_color_internal(gbstd::widgets::dial_event  evt) noexcept
{
  evt->get_userdata<color_maker>().build_color();
}


void
color_maker::
build_color() const noexcept
{
  m_color = gbstd::color(m_r_dial.get_current(),
                         m_g_dial.get_current(),
                         m_b_dial.get_current());

  get_userdata<color_holder>().set_color(m_color);
}


void
color_maker::
update() const noexcept
{
  m_r_dial.set_current(m_color.get_r7());
  m_g_dial.set_current(m_color.get_g7());
  m_b_dial.set_current(m_color.get_b7());

  m_sample.request_redraw();
}




}




