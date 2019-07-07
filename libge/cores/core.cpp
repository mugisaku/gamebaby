#include"libge/core.hpp"




namespace ge{


core::
core(gbstd::widgets::operating_node&  root) noexcept:
m_source(root,*this),
m_display(root,*this),
m_paint(root,*this)
{
}




core&
core::
set_pixel_base_point(gbstd::point  pt) noexcept
{
  m_pixel_base_point = pt;

  m_paint.reset();

  request_redraw();

  return *this;
}




void
core::
on_mouse_enter() noexcept
{
  m_focus = true;

  request_redraw();
}


void
core::
on_mouse_leave() noexcept
{
  m_paint.cancel_drawing();

  m_focus = false;

  request_redraw();
}




void
core::
on_mouse_act(gbstd::point  mouse_pos) noexcept
{
  int  x = mouse_pos.x/m_display.get_pixel_size();
  int  y = mouse_pos.y/m_display.get_pixel_size();

  gbstd::point  pt(x,y);

    if(pt != m_paint.get_drawing_point())
    {
      m_paint.set_drawing_point(pt);
    }


  m_paint();

  m_paint.update_brush_label();
}


void
core::
process_before_reform() noexcept
{
  int  w = m_source.get_cell_width() ;
  int  h = m_source.get_cell_height();

  set_content_width( m_display.get_pixel_size()*w);
  set_content_height(m_display.get_pixel_size()*h);
}


void
core::
render(const gbstd::canvas&  cv) noexcept
{
  m_display.render_background(cv);

    if(m_status.test(flags::underlay))
    {
      m_display.render_underlay(cv);
    }


  m_display.render_canvas(get_canvas(),cv);

    if(m_focus)
    {
        if(m_status.test(flags::grid))
        {
           m_display.render_grid(cv);
        }
    }


  auto  rect = m_paint.get_operation_rectangle();

    if(rect.w && rect.h)
    {
      m_display.render_rect(rect,cv);
    }
}


}




