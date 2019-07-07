#include"libge/core.hpp"




namespace ge{




core_display::
core_display(gbstd::widgets::operating_node&  root, core&  cor) noexcept:
m_core(cor)
{
  m_bg_style.first_color  = gbstd::color(0,0,7);
  m_bg_style.second_color = gbstd::color(2,2,5);
}




void
core_display::
push_underlay(gbstd::point  pt) noexcept
{
  m_underlay_stack.emplace_back(pt);
}


void
core_display::
pop_underlay() noexcept
{
  m_underlay_stack.pop_back();
}


void
core_display::
clear_underlay_stack() noexcept
{
  m_underlay_stack.clear();
}




void
core_display::
render_rect(gbstd::rectangle  rect, const gbstd::canvas&  cv, int  pixel_size) const noexcept
{
    if(!pixel_size)
    {
      pixel_size = m_pixel_size;
    }


  cv.draw_double_rectangle(gbstd::colors::white,gbstd::colors::black,
    pixel_size*rect.x,
    pixel_size*rect.y,
    pixel_size*rect.w,
    pixel_size*rect.h
  );
}


void
core_display::
render_grid(const gbstd::canvas&  cv, int  pixel_size) const noexcept
{
    if(!pixel_size)
    {
      pixel_size = m_pixel_size;
    }


  int  w = cv.get_width() ;
  int  h = cv.get_height();

  int  ww = w/pixel_size;
  int  hh = h/pixel_size;

    for(int  x = 0;  x < ww;  ++x)
    {
      cv.draw_vline(gbstd::colors::gray,pixel_size*x,0,h);
    }


    for(int  y = 0;  y < hh;  ++y)
    {
      cv.draw_hline(gbstd::colors::gray,0,pixel_size*y,w);
    }


  ww /= 8;
  hh /= 8;

    for(int  x = 0;  x < ww;  ++x)
    {
      cv.draw_vline(gbstd::colors::light_gray,pixel_size*8*x,0,h);
    }


    for(int  y = 0;  y < hh;  ++y)
    {
      cv.draw_hline(gbstd::colors::light_gray,0,pixel_size*8*y,w);
    }


  

  cv.draw_hline(gbstd::colors::white,0,(h/2)  ,w);
  cv.draw_vline(gbstd::colors::white,  (w/2),0,h);
}


void
core_display::
render_background(const gbstd::canvas&  cv, int  pixel_size) const noexcept
{
    if(!pixel_size)
    {
      pixel_size = m_pixel_size;
    }


  int  w = cv.get_width() ;
  int  h = cv.get_height();

  cv.draw_stripe_rectangle(m_bg_style.first_color,m_bg_style.second_color,pixel_size/2,0,0,w,h);
}


void
core_display::
render_underlay(const gbstd::canvas&  cv, int  pixel_size) const noexcept
{
    if(!pixel_size)
    {
      pixel_size = m_pixel_size;
    }


    for(auto&  pt: m_underlay_stack)
    {
      auto  src_cv = m_core.get_source().get_canvas(pt);

      render_canvas(src_cv,cv,0);
    }
}


void
core_display::
render_canvas(const gbstd::canvas&  src_cv, const gbstd::canvas&  dst_cv, int  pixel_size) const noexcept
{
    if(!pixel_size)
    {
      pixel_size = m_pixel_size;
    }


  int  w = src_cv.get_width() ;
  int  h = src_cv.get_height();

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto&  pix = *src_cv.get_pixel_pointer(x,y);

        if(pix.color)
        {
          dst_cv.fill_rectangle(pix.color,pixel_size*x,pixel_size*y,pixel_size,pixel_size);
        }
    }}
}




gbstd::widgets::node&
core_display::
create_background_widget(gbstd::widgets::operating_node&  root) noexcept
{
  auto&  ch1bg_btn = root.create_button(u"Change bg1 color",gbstd::colors::black)
    .set_callback([](gbstd::widgets::button_event  evt)
    {
        if(evt.is_release())
        {
          auto&  dsp = evt->get_userdata<core_display>();

          dsp.m_bg_style.first_color = dsp.m_core.get_paint().get_drawing_color();

          dsp.m_core.request_redraw();
        }
    }
  );


  auto&  ch2bg_btn = root.create_button(u"Change bg2 color",gbstd::colors::black)
    .set_callback([](gbstd::widgets::button_event  evt)
    {
        if(evt.is_release())
        {
          auto&  dsp = evt->get_userdata<core_display>();

          dsp.m_bg_style.second_color = dsp.m_core.get_paint().get_drawing_color();

          dsp.m_core.request_redraw();
        }
    }
  );


  ch1bg_btn.set_userdata(*this);
  ch2bg_btn.set_userdata(*this);

  return root.create_table_column({ch1bg_btn,ch2bg_btn});
}




}




