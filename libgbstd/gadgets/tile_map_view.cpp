#include"libgbstd/gadget.hpp"




namespace gbstd{




namespace{
void
render_tile_line(const pixel*  src, int  w, pixel*&  dst) noexcept
{
    while(w--)
    {
      *dst++ = *src++;
    }
}
}


void
tile_map_view::
render_line(int  src_x, int  src_y, int  tile_w, int  tile_h, pixel*  dst) const noexcept
{
  const int  x        = src_x/tile_w;
  const int  x_remain = src_x%tile_w;

  const int  y        = src_y/tile_h;
  const int  y_remain = src_y%tile_h;

  auto  const src_begin   = m_map->get_index_pointer(                 0,y);
  auto        src_current = m_map->get_index_pointer(                 x,y);
  auto  const src_end     = m_map->get_index_pointer(m_map->get_width(),y);

  int  n = m_width/tile_w;

    if(x_remain)
    {
      --n;

      auto  i = *src_current++;

        if(src_current >= src_end)
        {
          src_current = src_begin;
        }


      const pixel*  src = m_source->get_pixel_pointer(i);

      src += m_source->get_width()*y_remain;

      render_tile_line(src+x_remain,tile_w-x_remain,dst);
    }


    while(n--)
    {
      auto  i = *src_current++;

        if(src_current >= src_end)
        {
          src_current = src_begin;
        }


      const pixel*  src = m_source->get_pixel_pointer(i);

      src += m_source->get_width()*y_remain;

      render_tile_line(src,tile_w,dst);
    }


    if(x_remain)
    {
      auto  i = *src_current++;

        if(src_current >= src_end)
        {
          src_current = src_begin;
        }


      const pixel*  src = m_source->get_pixel_pointer(i);

      src += m_source->get_width()*y_remain;

      render_tile_line(src,x_remain,dst);
    }
}


void
tile_map_view::
render(const canvas&  cv) const noexcept
{
  int  x = m_offset.x%m_width ;
  int  y = m_offset.y%m_height;

    if(x < 0){x += m_width ;}
    if(y < 0){y += m_height;}


  int  tile_w = m_source->get_width();
  int  tile_h = m_source->get_height();

  auto  dst = cv.get_pixel_pointer(0,0);

    for(int  n = 0;  n < m_height;  ++n)
    {
      render_line(x,y++,tile_w,tile_h,dst);

        if(y >= m_height)
        {
          y = 0;
        }


      dst += cv.get_source_width();
    }
}




}




