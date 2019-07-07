#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{




bool
canvas::
operator==(const canvas&  rhs) const noexcept
{
  return((m_pointer      == rhs.m_pointer     ) &&
         (m_width        == rhs.m_width       ) &&
         (m_height       == rhs.m_height      ) &&
         (m_source_width == rhs.m_source_width));
}




canvas&
canvas::
assign(pixel*  ptr, int  src_w, int  w, int  h) noexcept
{
  m_pointer = ptr;

  m_source_width = src_w;

  m_width  = w;
  m_height = h;

  return *this;
}


canvas&
canvas::
assign(const image&  img) noexcept
{
  return assign(img,0,0,img.get_width(),img.get_height());
}


canvas&
canvas::
assign(const image&  img, int  x, int  y, int  w, int  h) noexcept
{
  m_pointer = img.get_pixel_pointer(x,y);

  m_source_width = img.get_width();

  m_width  = w;
  m_height = h;


  return *this;
}


canvas&
canvas::
assign(const canvas&  cv, int  x, int  y, int  w, int  h) noexcept
{
  m_pointer = cv.m_pointer+(cv.m_source_width*y)+x;

  m_source_width = cv.m_source_width;

  m_width  = w;
  m_height = h;


  return *this;
}




image
canvas::
make_image() const noexcept
{
  int  w = get_width() ;
  int  h = get_height();

  image  img(w,h);

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto  pix = *get_pixel_pointer(x,y);

      *img.get_pixel_pointer(x,y) = pix;
    }}


  return std::move(img);
}


void
canvas::
fill(color  c) const noexcept
{
  fill(c,0,0,m_width,m_height);
}


void
canvas::
fill(color  c, int  x, int  y, int  w, int  h) const noexcept
{
  auto  p = get_pixel_pointer(x,y);

    for(int  yy = 0;  yy < h;  ++yy)
    {
      auto  pp = p                  ;
                 p += m_source_width;

        for(int  xx = 0;  xx < w;  ++xx)
        {
          auto&  dst = *pp++;

          dst.color = c;
          dst.z     = 0;
        }
    }
}




namespace{
int
clamp(int  n) noexcept
{
  return (n <= 0)? 0:
         (n >= 7)? 7:n;
}
}


void
canvas::
add_rgb(color_modifier  mod) const noexcept
{
  add_rgb(mod,0,0,m_width,m_height);
}


void
canvas::
add_rgb(color_modifier  mod, int  x, int  y, int  w, int  h) const noexcept
{
  auto  p = get_pixel_pointer(x,y);

    while(h--)
    {
      auto  pp = p                  ;
                 p += m_source_width;

      auto  end = pp+w;

        while(pp !=end)
        {
          auto&  pix = *pp++;

            if(pix.color)
            {
              auto  new_r = clamp(pix.color.get_r7()+mod.r);
              auto  new_g = clamp(pix.color.get_g7()+mod.g);
              auto  new_b = clamp(pix.color.get_b7()+mod.b);

              pix.color = color(new_r,new_g,new_b);
            }
        }
    }
}


void
canvas::
reverse_rgb() const noexcept
{
  reverse_rgb(0,0,m_width,m_height);
}


void
canvas::
reverse_rgb(int  x, int  y, int  w, int  h) const noexcept
{
  auto  p = get_pixel_pointer(x,y);

    while(h--)
    {
      auto  pp = p                  ;
                 p += m_source_width;

      auto  end = pp+w;

        while(pp !=end)
        {
          auto&  pix = *pp++;

            if(pix.color)
            {
              auto  new_r = (7-pix.color.get_r7());
              auto  new_g = (7-pix.color.get_g7());
              auto  new_b = (7-pix.color.get_b7());

              pix.color = color(new_r,new_g,new_b);
            }
        }
    }
}




}





