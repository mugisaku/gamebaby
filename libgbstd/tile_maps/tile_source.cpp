#include"libgbstd/tile_map.hpp"




namespace gbstd{




image
tile_source::
make_image() noexcept
{
  image  img;

  img.resize(m_width*m_number_of_tiles,m_height);

  canvas  cv(img);

    for(int  i = 0;  i < m_number_of_tiles;  ++i)
    {
      cv.draw_canvas(get_canvas(i),m_width*i,0);
    }


  return std::move(img);
}


int
tile_source::
append(const canvas&  cv, int  x, int  y) noexcept
{
  pixel  tmp[m_width*m_height];

  pixel*  p = tmp;

    for(int  yy = 0;  yy < m_height;  ++yy){
    for(int  xx = 0;  xx < m_width ;  ++xx){
      *p++ = *cv.get_pixel_pointer(x+xx,y+yy);
    }}


  auto  i = find(tmp);

    if(i < 0)
    {
      const pixel*  pp = tmp;

        while(pp < p)
        {
          m_stream.emplace_back(*pp++);
        }


      i = m_number_of_tiles++;
    }


  return i;
}


int
tile_source::
find(const pixel*  stream) const noexcept
{
  int  i = 0;

  int  n = m_width*m_height;

    while(i < m_number_of_tiles)
    {
      auto  p = &m_stream[n*i];

        if(std::memcmp(stream,p,sizeof(pixel)*n) == 0)
        {
          return i;
        }


      ++i;
    }


  return -1;
}


int
tile_source::
find(const canvas&  cv, int  x, int  y) const noexcept
{
  pixel  tmp[m_width*m_height];

  pixel*  p = tmp ;

    for(int  yy = 0;  yy < m_height;  ++yy){
    for(int  xx = 0;  xx < m_width ;  ++xx){
      *p++ = *cv.get_pixel_pointer(x+xx,y+yy);
    }}


  return find(tmp);
}




tile_source&
tile_source::
make(int  w, int  h, int  n) noexcept
{
  m_width  = w;
  m_height = h;

  m_number_of_tiles = n;

  m_stream.resize(w*h*n);

  return *this;
}




}




