#include"libgbstd/video.hpp"




namespace gbstd{




int
window::
get_width() const noexcept
{
  return m_style.get_frame_left_width()
        +m_content_width
        +m_style.get_frame_right_width();
}


int
window::
get_height() const noexcept
{
  return m_style.get_frame_top_width()
        +m_content_height
        +m_style.get_frame_bottom_width();
}



canvas
window::
get_frame_top_canvas(const canvas&  base_cv) const noexcept
{
  return canvas(base_cv,get_x_position(),
                        get_y_position(),
                        get_width(),
                        get_style().get_frame_top_width());
}


canvas
window::
get_frame_left_canvas(const canvas&  base_cv) const noexcept
{
  return canvas(base_cv,get_x_position(),
                        get_content_y_position(),
                        get_style().get_frame_left_width(),
                        m_content_height);
}


canvas
window::
get_frame_right_canvas(const canvas&  base_cv) const noexcept
{
  return canvas(base_cv,get_content_x_position()+get_content_width(),
                        get_content_y_position(),
                        get_style().get_frame_right_width(),
                        m_content_height);
}


canvas
window::
get_frame_bottom_canvas(const canvas&  base_cv) const noexcept
{
  return canvas(base_cv,get_x_position(),
                        get_content_y_position()+get_content_height(),
                        get_width(),
                        get_style().get_frame_bottom_width());
}


canvas
window::
get_content_canvas(const canvas&  base_cv) const noexcept
{
  return canvas(base_cv,get_content_x_position(),
                        get_content_y_position(),
                        m_content_width,
                        m_content_height);
}


void
window::
render_frame(const canvas&  cv) noexcept
{
  auto     top = get_frame_top_canvas(cv);
  auto    left = get_frame_left_canvas(cv);
  auto   right = get_frame_right_canvas(cv);
  auto  bottom = get_frame_bottom_canvas(cv);

  auto  fill_color = m_style.get_fill_color();

  left.draw_vline(colors::gray      ,0,0,left.get_height());
  left.draw_vline(colors::light_gray,1,0,left.get_height());
  left.draw_vline(colors::white     ,2,0,left.get_height());
  left.draw_vline(colors::light_gray,3,0,left.get_height());
  left.draw_vline(colors::gray      ,4,0,left.get_height());
  left.draw_vline(fill_color        ,5,0,left.get_height());
  left.draw_vline(fill_color        ,6,0,left.get_height());
  left.draw_vline(fill_color        ,7,0,left.get_height());

  right.draw_vline(colors::gray      ,7,0,right.get_height());
  right.draw_vline(colors::light_gray,6,0,right.get_height());
  right.draw_vline(colors::white     ,5,0,right.get_height());
  right.draw_vline(colors::light_gray,4,0,right.get_height());
  right.draw_vline(colors::gray      ,3,0,right.get_height());
  right.draw_vline(fill_color        ,2,0,right.get_height());
  right.draw_vline(fill_color        ,1,0,right.get_height());
  right.draw_vline(fill_color        ,0,0,right.get_height());


  top.draw_hline(colors::gray      ,4,0,top.get_width()-8);
  top.draw_hline(colors::light_gray,4,1,top.get_width()-8);
  top.draw_hline(colors::white     ,4,2,top.get_width()-8);
  top.draw_hline(colors::light_gray,4,3,top.get_width()-8);
  top.draw_hline(colors::gray      ,4,4,top.get_width()-8);
  top.draw_hline(fill_color        ,4,5,top.get_width()-8);
  top.draw_hline(fill_color        ,4,6,top.get_width()-8);
  top.draw_hline(fill_color        ,4,7,top.get_width()-8);

  bottom.draw_hline(colors::gray      ,4,7,bottom.get_width()-8);
  bottom.draw_hline(colors::light_gray,4,6,bottom.get_width()-8);
  bottom.draw_hline(colors::white     ,4,5,bottom.get_width()-8);
  bottom.draw_hline(colors::white     ,4,4,bottom.get_width()-8);
  bottom.draw_hline(colors::light_gray,4,3,bottom.get_width()-8);
  bottom.draw_hline(colors::gray      ,4,2,bottom.get_width()-8);
  bottom.draw_hline(fill_color        ,4,1,bottom.get_width()-8);
  bottom.draw_hline(fill_color        ,4,0,bottom.get_width()-8);
}


canvas
window::
render(const canvas&  cv) noexcept
{
  render_frame(cv);


  auto  cont_cv = get_content_canvas(cv);

  cont_cv.fill(m_style.get_fill_color());

  return cont_cv;
}




}




