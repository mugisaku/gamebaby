#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




button::
button(operating_node&  root) noexcept
{
  be_autonomous();
}




button&
button::
set_content(node&  nd) noexcept
{
  clear();

  nd.get_style()
    .set_top_padding(4)
    .set_left_padding(4)
    .set_right_padding(4)
    .set_bottom_padding(4)
  ;


  add_child(nd,{});

  return *this;
}


void
button::
on_mouse_leave() noexcept
{
    if(is_pressed())
    {
      m_state = state::released;

        if(m_callback)
        {
          m_callback({*this,button_event::kind::release});
        }


      request_redraw();
    }
}


void
button::
on_mouse_act(point  mouse_pos) noexcept
{
    if(is_released())
    {
        if(get_keys().test_mol())
        {
          m_state = state::pressed;

            if(m_callback)
            {
              m_callback({*this,button_event::kind::press});
            }


          request_redraw();
        }
    }

  else
    {
        if(!get_keys().test_mol())
        {
          on_mouse_leave();
        }
    }
}




void
button::
render(const canvas&  cv) noexcept
{
  int  w = get_width() ;
  int  h = get_height();

    if(is_pressed())
    {
      cv.draw_hline(colors::black,0  ,0  ,w);
      cv.draw_hline(colors::white,0  ,h-1,w);
      cv.draw_vline(colors::black,0  ,0  ,h);
      cv.draw_vline(colors::white,w-1,0  ,h);

      cv.fill_rectangle(color(2,2,2),1,1,w-2,h-2);
    }

  else
    {
      cv.draw_hline(colors::white,0  ,0  ,w);
      cv.draw_hline(colors::black,0  ,h-1,w);
      cv.draw_vline(colors::white,0  ,0  ,h);
      cv.draw_vline(colors::black,w-1,0  ,h);

      cv.fill_rectangle(color(3,3,3),1,1,w-2,h-2);
    }
}




}}




