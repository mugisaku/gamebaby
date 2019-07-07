#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{




image::
image(int  w, int  h, std::initializer_list<int>  ls) noexcept
{
  resize(w,h);

  int  n = std::min(w*h,(int)ls.size());

  auto  src = ls.begin();
  auto  dst =    begin();

    while(n--)
    {
      *dst++ = pixel(color(*src++));
    }
}


image::
image(int  w, int  h, std::initializer_list<color>  ls) noexcept
{
  resize(w,h);

  int  n = std::min(w*h,(int)ls.size());

  auto  src = ls.begin();
  auto  dst =    begin();

    while(n--)
    {
      *dst++ = *src++;
    }
}





image&
image::
assign(const image&  rhs) noexcept
{
    if(this != &rhs)
    {
      resize(rhs.m_width,rhs.m_height);
    }


  return *this;
}


image&
image::
assign(image&&  rhs) noexcept
{
    if(this != &rhs)
    {
      resize(0,0);

      std::swap(m_pixels,rhs.m_pixels);
      std::swap(m_width ,rhs.m_width );
      std::swap(m_height,rhs.m_height);
    }


  return *this;
}




void
image::
resize(int  w, int  h) noexcept
{
    if((w == m_width ) &&
       (h == m_height))
    {
      return;
    }


    if(!w || !h)
    {
      static pixel  null;

      delete[] m_pixels        ;
               m_pixels = &null;

      m_width  = 0;
      m_height = 0;

      return;
    }


  int  number_of_pixels = w*h;

    if(number_of_pixels > (m_width*m_height))
    {
      auto  old_pixels = m_pixels                              ;
                         m_pixels = new pixel[number_of_pixels];

        for(int  y = 0;  y < m_height;  ++y){
        for(int  x = 0;  x < m_width ;  ++x){
          m_pixels[(w*y)+x] = old_pixels[(m_width*y)+x];
        }}


      delete[] old_pixels;
    }


  m_width  = w;
  m_height = h;
}




void
image::
fill(color  c) const noexcept
{
    for(auto&  dst: *this)
    {
      dst.color = c;
      dst.z     = 0;
    }
}


std::vector<uint8_t>
image::
make_txt_stream() const noexcept
{
  auto  pngs = make_png_stream();

  std::vector<uint8_t>  buf;

  char  s[8];

    for(auto  c: pngs)
    {
      int  n = snprintf(s,sizeof(s),"%d,",c);

        for(int  i = 0;  i < n;  ++i)
        {
          buf.emplace_back(s[i]);
        }
    }


  return std::move(buf);
}


namespace{
const uint8_t
g_misc_png[] =
{
#include"misc.png.txt"
};
}


const image  g_misc_image(g_misc_png);


}





