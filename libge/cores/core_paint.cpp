#include"libge/core.hpp"




namespace ge{




core_paint::
core_paint(gbstd::widgets::operating_node&  root, core&  cor) noexcept:
m_core(cor),
m_brush_label(root.create_label(u"X: -- Y: -- PIX: ---"))
{
  reset();
}




core_paint&
core_paint::
reset() noexcept
{
  m_recorder.clear();

  m_drawing_is_fixed = true;

  m_operation_rect.x = 0;
  m_operation_rect.y = 0;
  m_operation_rect.w = m_core.get_canvas().get_width() ;
  m_operation_rect.h = m_core.get_canvas().get_height();

  return *this;
}




core_paint&
core_paint::
cancel_drawing() noexcept
{
  m_pointing_count = 0;

    if(!m_drawing_is_fixed)
    {
        if((m_mode == mode::paste) ||
           (m_mode == mode::layer))
        {
          undo();
        }


      m_drawing_is_fixed = true;
    }


  return *this;
}


core_paint&
core_paint::
cancel_select() noexcept
{
  m_operation_rect.x = 0;
  m_operation_rect.y = 0;
  m_operation_rect.w = 0;
  m_operation_rect.h = 0;

  return *this;
}


void
core_paint::
modify_dot(const gbstd::canvas&  cv, gbstd::color  color, gbstd::point  pt) noexcept
{
  auto&  pix = *cv.get_pixel_pointer(pt.x,pt.y);

    if(pix.color != color)
    {
      m_recorder.put(pix.color,pt.x,pt.y);

      pix.color = color;
    }
}




core_paint&
core_paint::
take_copy() noexcept
{
  m_clipped_image.resize(m_operation_rect.w,m_operation_rect.h);

  auto  cv = m_core.get_canvas();

    for(int  y = 0;  y < m_operation_rect.h;  ++y){
    for(int  x = 0;  x < m_operation_rect.w;  ++x){
      auto  pix = *cv.get_pixel_pointer(m_operation_rect.x+x,m_operation_rect.y+y);

      *m_clipped_image.get_pixel_pointer(x,y) = pix;
    }}


  return *this;
}




void
core_paint::
try_to_push_solid_record() noexcept
{
    if(m_recorder.push(true))
    {
      m_drawing_is_fixed = false;
    }
}


void
core_paint::
try_to_push_nonsolid_record() noexcept
{
    if(m_drawing_is_fixed)
    {
      m_recorder.push(false);
    }

  else
    {
      m_drawing_is_fixed = true;
    }


  m_core.request_redraw();
}


void
core_paint::
undo() noexcept
{
  m_recorder.rollback(m_core.get_canvas());

  m_core.request_redraw();
}


const gbstd::pixel&
core_paint::
get_current_pixel() const noexcept
{
  return *m_core.get_canvas().get_pixel_pointer(m_drawing_point.x,m_drawing_point.y);
}


core_paint&
core_paint::
operator()() noexcept
{
  auto  cv = m_core.get_canvas();

    switch(m_mode)
    {
  case(mode::draw_dot):
        if(gbstd::get_keys().test_mol())
        {
          modify_dot(cv,m_drawing_color,m_drawing_point);

          m_core.request_redraw();
        }

      else
        if(gbstd::get_keys().test_mor())
        {
          modify_dot(cv,gbstd::color(),m_drawing_point);

          m_core.request_redraw();
        }
      break;
  case(mode::draw_line):
        if(m_pointing_count)
        {
            if(gbstd::get_keys().test_mol())
            {
                if(!m_drawing_is_fixed)
                {
                  undo();
                }


              draw_line(m_drawing_color);
            }

          else
            if(gbstd::get_keys().test_mor())
            {
                if(!m_drawing_is_fixed)
                {
                  undo();
                }


              draw_line(gbstd::color());
            }

          else
            {
              m_drawing_is_fixed = true;

              m_pointing_count = 0;
            }
        }

      else
        {
            if(gbstd::get_keys().test_mol() ||
               gbstd::get_keys().test_mor())
            {
              m_saved_drawing_point = m_drawing_point;

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::draw_rectangle):
        if(m_pointing_count)
        {
          gbstd::rectangle  rect(m_saved_drawing_point,m_drawing_point);

            if(gbstd::get_keys().test_mol())
            {
                if(!m_drawing_is_fixed)
                {
                  undo();
                }


              draw_rect(m_drawing_color,rect);
            }

          else
            if(gbstd::get_keys().test_mor())
            {
                if(!m_drawing_is_fixed)
                {
                  undo();
                }


              draw_rect(gbstd::color(),rect);
            }

          else
            {
              m_drawing_is_fixed = true;

              m_pointing_count = 0;
            }
        }

      else
        {
            if(gbstd::get_keys().test_mol() ||
               gbstd::get_keys().test_mor())
            {
              m_saved_drawing_point = m_drawing_point;

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::fill_rectangle):
        if(m_pointing_count)
        {
          gbstd::rectangle  rect(m_saved_drawing_point,m_drawing_point);

            if(gbstd::get_keys().test_mol())
            {
                if(!m_drawing_is_fixed)
                {
                  undo();
                }


              fill_rect(m_drawing_color,rect);
            }

          else
            if(gbstd::get_keys().test_mor())
            {
                if(!m_drawing_is_fixed)
                {
                  undo();
                }


              fill_rect(gbstd::color(),rect);
            }

          else
            {
              m_drawing_is_fixed = true;

              m_pointing_count = 0;
            }
        }

      else
        {
            if(gbstd::get_keys().test_mol() ||
               gbstd::get_keys().test_mor())
            {
              m_saved_drawing_point = m_drawing_point;

              m_pointing_count = 1;
            }
        }
      break;
  case(mode::fill_area):
        if(gbstd::get_keys().test_mol())
        {
          fill_area(m_drawing_color);
        }

      else
        if(gbstd::get_keys().test_mor())
        {
          fill_area(gbstd::color());
        }
      break;
  case(mode::select):
        if(m_pointing_count)
        {
            if(gbstd::get_keys().test_mol())
            {
              m_operation_rect = gbstd::rectangle(m_saved_drawing_point,m_drawing_point);

              m_core.request_redraw();
            }

          else
            {
              m_pointing_count = 0;
            }
        }

      else
        {
            if(gbstd::get_keys().test_mol())
            {
              m_saved_drawing_point = m_drawing_point;

              m_pointing_count = 1;
            }

          else
            if(gbstd::get_keys().test_mor())
            {
              auto  cv = m_core.get_canvas();

              m_operation_rect = gbstd::rectangle(0,0,cv.get_width(),cv.get_height());

              m_core.request_redraw();
            }
        }
      break;
  case(mode::paste):
        if(gbstd::get_keys().test_mol())
        {
          m_drawing_is_fixed = true;
        }

      else
        if(m_saved_drawing_point != m_drawing_point)
        {
            if(!m_drawing_is_fixed)
            {
              undo();
            }


          paste(false);
        }


      m_saved_drawing_point = m_drawing_point;
      break;
  case(mode::layer):
        if(gbstd::get_keys().test_mol())
        {
          m_drawing_is_fixed = true;
        }

      else
        if(m_saved_drawing_point != m_drawing_point)
        {
            if(!m_drawing_is_fixed)
            {
              undo();
            }


          paste(true);
        }


      m_saved_drawing_point = m_drawing_point;
      break;
    }


  return *this;
}




core_paint&
core_paint::
update_brush_label() noexcept
{
  gbstd::string_form  sf;

  auto  pt = get_drawing_point();

  auto  color = get_current_pixel().color;

    if(color)
    {
      int  r = color.get_r7();
      int  g = color.get_g7();
      int  b = color.get_b7();

      sf("X: %2d Y: %2d PIX: %d%d%d",pt.x,pt.y,r,g,b);
    }

  else
    {
      sf("X: %2d Y: %2d PIX: ---",pt.x,pt.y);
    }


  m_brush_label.set_string_without_reform(sf());


  return *this;
}




}




