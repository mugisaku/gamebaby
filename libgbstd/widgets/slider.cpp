#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




namespace{
constexpr int  g_shift_amount = 16;
constexpr int  g_half_unit = (1<<g_shift_amount)/2;

constexpr int  g_short_length = 16;
constexpr int  g_offset = (g_short_length/2);
}




slider::
slider(operating_node&  root, kind  k) noexcept:
m_kind(k)
{
}




void
slider::
process_before_reform() noexcept
{
  set_content_width( is_vertical()? g_short_length:m_length);
  set_content_height(is_vertical()? m_length:g_short_length);
}


slider&
slider::
be_horizontal() noexcept
{
  m_kind = kind::horizontal;

  request_reform();

  return *this;
}


slider&
slider::
be_vertical() noexcept
{
  m_kind = kind::vertical;

  request_reform();

  return *this;
}


slider&
slider::
set_length(int  v) noexcept
{
  m_length = (v <= (g_short_length*3))? (g_short_length*3)
            : v;

  request_reform();

  return *this;
}


slider&
slider::
set_value_max(int  v) noexcept
{
  m_value_max = v;

  request_reform();

  return *this;
}


slider&
slider::
set_position(int  v) noexcept
{
  m_position = (v <=                         0)? 0
              :(v >= (m_length-g_short_length))? (m_length-g_short_length)
              : v;

  request_redraw();

  return *this;
}


slider&
slider::
set_value(int  v) noexcept
{
  auto  base = (((m_length-g_short_length)<<g_shift_amount)+g_half_unit)/m_value_max;

  m_position = (base*v)>>g_shift_amount;

    if(m_callback)
    {
      m_callback({*this,v});
    }


  request_redraw();

  return *this;
}


int
slider::
get_value() const noexcept
{
  int  base = (m_value_max<<g_shift_amount)+g_half_unit;

  return (base/(m_length-g_short_length)*m_position)>>g_shift_amount;
}


void
slider::
on_mouse_act(point  mouse_pos) noexcept
{
    if(g_input.test_mouse_left())
    {
      int  v = is_vertical()? mouse_pos.y:mouse_pos.x;

      int  cur = m_position+g_offset;

        if(v < cur)
        {
          m_position = std::max(v,g_offset)-g_offset;

            if(m_callback)
            {
              m_callback({*this,get_value()});
            }


          request_redraw();
        }

      else
        if(v > cur)
        {
          m_position = std::min(v,m_length-g_offset)-g_offset;

            if(m_callback)
            {
              m_callback({*this,get_value()});
            }


          request_redraw();
        }
    }
}


void
slider::
render(const canvas&  cv) noexcept
{
  cv.fill(m_style.get_background_color());

    if(is_vertical())
    {
      cv.fill_rectangle(colors::black,2,0,g_short_length-4,m_length);

      cv.fill_rectangle(colors::white,0,m_position,g_short_length,g_short_length);
    }

  else
    {
      cv.fill_rectangle(colors::black,0,2,m_length,g_short_length-4);

      cv.fill_rectangle(colors::white,m_position,0,g_short_length,g_short_length);
    }
}




}}




