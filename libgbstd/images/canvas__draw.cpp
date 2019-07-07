#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{




void
canvas::
draw_dot(color  i, int  x, int  y) const noexcept
{
    if(i)
    {
      get_pixel_pointer(x,y)->color = i;
    }
}


void
canvas::
draw_dot_safely(color  i, int  x, int  y) const noexcept
{
    if((x >=        0) &&
       (x <   m_width) &&
       (y >=        0) &&
       (y <  m_height))
    {
      draw_dot(i,x,y);
    }
}


void
canvas::
draw_vline(color  i, int  x, int  y, int  l) const noexcept
{
    while(l--)
    {
      draw_dot(i,x,y++);
    }
}


void
canvas::
draw_vline_safely(color  i, int  x, int  y, int  l) const noexcept
{
    if((x >=       0) &&
       (x <  m_width))
    {
        if(y < 0)
        {
          l += y    ;
               y = 0;
        }

      else
        if(y+l >= m_height)
        {
          l = m_height-y;
        }


        if(l > 0)
        {
            while(l--)
            {
              draw_dot(i,x,y);

              y += 1;
            }
        }
    }
}


void
canvas::
draw_hline(color  i, int  x, int  y, int  l) const noexcept
{
    while(l--)
    {
      draw_dot(i,x++,y);
    }
}


void
canvas::
draw_hline_safely(color  i, int  x, int  y, int  l) const noexcept
{
    if((y >=        0) &&
       (y <  m_height))
    {
        if(x < 0)
        {
          l += x    ;
               x = 0;
        }

      else
        if(x+l >= m_width)
        {
          l = m_width-x;
        }


        if(l > 0)
        {
            while(l--)
            {
              draw_dot(i,x,y);

              x += 1;
            }
        }
    }
}


void
canvas::
draw_line(color  i, int  x0, int  y0, int  x1, int  y1) const noexcept
{
  liner  lnr(x0,y0,x1,y1);

    for(;;)
    {
      draw_dot(i,lnr.get_x(),lnr.get_y());

        if(!lnr.get_distance())
        {
          break;
        }


      lnr.step();
    }
}


void
canvas::
draw_line_safely(color  i, int  x0, int  y0, int  x1, int  y1) const noexcept
{
  liner  lnr(x0,y0,x1,y1);

    for(;;)
    {
      draw_dot_safely(i,lnr.get_x(),lnr.get_y());

        if(!lnr.get_distance())
        {
          break;
        }


      lnr.step();
    }
}


void
canvas::
draw_rectangle(color  i, int  x, int  y, int  w, int  h) const noexcept
{
    if(i)
    {
      draw_hline(i,x,y    ,w);
      draw_hline(i,x,y+h-1,w);

      draw_vline(i,x    ,y+1,h-2);
      draw_vline(i,x+w-1,y+1,h-2);
    }
}


void
canvas::
draw_rectangle_safely(color  i, int  x, int  y, int  w, int  h) const noexcept
{
  draw_hline_safely(i,x,y    ,w);
  draw_hline_safely(i,x,y+h-1,w);

  draw_vline_safely(i,x    ,y+1,h-2);
  draw_vline_safely(i,x+w-1,y+1,h-2);
}


void
canvas::
fill_rectangle(color  i, int  x, int  y, int  w, int  h) const noexcept
{
    while(h--)
    {
      draw_hline(i,x,y,w);

      ++y;
    }
}


void
canvas::
fill_rectangle_safely(color  i, int  x, int  y, int  w, int  h) const noexcept
{
    while(h--)
    {
      draw_hline_safely(i,x,y,w);

      ++y;
    }
}




void
canvas::
draw_double_rectangle(color  out, color  in, int  x, int  y, int  w, int  h) const noexcept
{
  draw_rectangle(out,x  ,y  ,w  ,h  );
  draw_rectangle( in,x+1,y+1,w-2,h-2);
}


void
canvas::
draw_stripe_rectangle(color  first, color  second, int  interval, int  x, int  y, int  w, int  h) const noexcept
{
    if(interval)
    {
        for(;;)
        {
            for(int  n = 0;  n < interval;  ++n)
            {
                if(!h--)
                {
                  return;
                }


              draw_hline(first,x,y++,w);
            }


            for(int  n = 0;  n < interval;  ++n)
            {
                if(!h--)
                {
                  return;
                }


              draw_hline(second,x,y++,w);
            }
        }
    }
}




namespace{
bool
correct(rectangle&  src_rect, int&  dst_x, int&  dst_y, int  dst_w, int  dst_h) noexcept
{
  auto&  src_w = src_rect.w;
  auto&  src_h = src_rect.h;

    if(dst_x < 0)
    {
        if((src_w+dst_x) <= 0)
        {
          return false;
        }


      src_rect.x = -dst_x;

      src_w += dst_x    ;
               dst_x = 0;
    }


    if(dst_y < 0)
    {
        if((src_h+dst_y) <= 0)
        {
          return false;
        }


      src_rect.y = -dst_y;

      src_h += dst_y    ;
               dst_y = 0;
    }


    if((dst_x+src_w) >= dst_w)
    {
      src_w = (dst_w-dst_x);

        if(src_w <= 0)
        {
          return false;
        }
    }


    if((dst_y+src_h) >= dst_h)
    {
      src_h = (dst_h-dst_y);

        if(src_h <= 0)
        {
          return false;
        }
    }


  return true;
}


template<int  INC, bool  COPY>
void
transfer_line(const pixel*  src, pixel*  dst, int  n) noexcept
{
    while(n--)
    {
        if(COPY || *src)
        {
          *dst = *src;
        }


      dst +=   1;
      src += INC;
    }
}


template<bool  COPY, bool  REVERSE>
void
transfer(const canvas&  orig_src, const canvas&  dst, int  dst_x, int  dst_y) noexcept
{
  rectangle  src_rect(0,0,orig_src.get_width(),orig_src.get_height());

    if(correct(src_rect,dst_x,dst_y,dst.get_width(),dst.get_height()))
    {
      auto  src_pixptr_base = orig_src(REVERSE? orig_src.get_width()-1-src_rect.x:src_rect.x,src_rect.y);
      auto  dst_pixptr_base =      dst(dst_x,dst_y);

      auto  src_w = src_rect.w;
      auto  src_h = src_rect.h;

        for(int  y = 0;  y < src_h;  ++y)
        {
          auto  dst_pixptr = dst_pixptr_base                          ;
                             dst_pixptr_base += dst.get_source_width();

          auto  src_pixptr = src_pixptr_base                               ;
                             src_pixptr_base += orig_src.get_source_width();

            if(REVERSE){transfer_line<-1,COPY>(src_pixptr,dst_pixptr,src_w);}
          else         {transfer_line< 1,COPY>(src_pixptr,dst_pixptr,src_w);}
        }
    }
}
}


void
canvas::
draw_canvas(const canvas&  cv, int   x, int  y) const noexcept
{
  transfer<false,false>(cv,*this,x,y);
}


void
canvas::
copy_canvas(const canvas&  cv, int   x, int  y) const noexcept
{
  transfer<true,false>(cv,*this,x,y);
}


void
canvas::
draw_canvas_reversely(const canvas&  cv, int   x, int  y) const noexcept
{
  transfer<false,true>(cv,*this,x,y);
}


void
canvas::
copy_canvas_reversely(const canvas&  cv, int   x, int  y) const noexcept
{
  transfer<true,true>(cv,*this,x,y);
}


void
canvas::
draw_canvas(const canvas&  cv, int  permill, int  x, int  y) const noexcept
{
  int  src_w = cv.get_width() ;
  int  src_h = cv.get_height();

  int  w = ((src_w<<10)/100*permill)>>10;
  int  h = ((src_h<<10)/100*permill)>>10;

  int  x_unit = (src_w<<10)/w;
  int  y_unit = (src_h<<10)/h;

    for(int  yy = 0;  yy < h;  ++yy){
    for(int  xx = 0;  xx < w;  ++xx){
      int  xxx = (x_unit*xx)>>10;
      int  yyy = (y_unit*yy)>>10;

        if((xxx < src_w) &&
           (yyy < src_h))
        {
          auto  pix = *cv.get_pixel_pointer(xxx,yyy);

          draw_dot(pix,x+xx,y+yy);
        }
    }}
}




void
canvas::
blend_canvas(const canvas&  cv, int  x, int  y) const noexcept
{
  rectangle  src_rect(0,0,cv.get_width(),cv.get_height());

    if(correct(src_rect,x,y,get_width(),get_height()))
    {
      canvas  src_cv(cv,src_rect);

      auto  src_pixptr_base = src_cv.get_pixel_pointer(0,0);
      auto  dst_pixptr_base =        get_pixel_pointer(x,y);

      auto  src_w = src_cv.get_width() ;
      auto  src_h = src_cv.get_height();

        for(int  yy = 0;  yy < src_h;  ++yy)
        {
          auto  dst_pixptr = dst_pixptr_base                     ;
                             dst_pixptr_base += get_source_width();

          auto  src_pixptr = src_pixptr_base                            ;
                             src_pixptr_base += src_cv.get_source_width();

            for(int  xx = 0;  xx < src_w;  ++xx)
            {
              auto&  pix = *src_pixptr;

                if(pix.color && (dst_pixptr->z <= pix.z))
                {
                  *dst_pixptr = pix;
                }


              ++dst_pixptr;
              ++src_pixptr;
            }
        }
    }
}


void
canvas::
blend_canvas(const canvas&  cv, int  x, int  y, int  z_base, int  z_add_amount) const noexcept
{
  rectangle  src_rect(0,0,cv.get_width(),cv.get_height());

    if(correct(src_rect,x,y,get_width(),get_height()))
    {
      canvas  src_cv(cv,src_rect);

      auto  src_pixptr_base = src_cv.get_pixel_pointer(0,0);
      auto  dst_pixptr_base =        get_pixel_pointer(x,y);

      auto  src_w = src_cv.get_width() ;
      auto  src_h = src_cv.get_height();

        for(int  yy = 0;  yy < src_h;  ++yy)
        {
          auto  dst_pixptr = dst_pixptr_base                     ;
                             dst_pixptr_base += get_source_width();

          auto  src_pixptr = src_pixptr_base                            ;
                             src_pixptr_base += src_cv.get_source_width();

            for(int  xx = 0;  xx < src_w;  ++xx)
            {
              auto&  pix = *src_pixptr;

                if(pix.color && (dst_pixptr->z <= z_base))
                {
                  dst_pixptr->color =  pix.color;
                  dst_pixptr->z     =     z_base;
                }


              ++dst_pixptr;
              ++src_pixptr;
            }


          z_base += z_add_amount;
        }
    }
}




}





