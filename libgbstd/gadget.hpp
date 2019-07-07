#ifndef gbstd_gadget_HPP
#define gbstd_gadget_HPP


#include"libgbstd/image.hpp"




namespace gbstd{




class
tile_source
{
  int  m_width =0;
  int  m_height=0;

  int  m_number_of_tiles=0;

  std::vector<pixel>  m_stream;

public:
  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  int  get_number_of_tiles() const noexcept{return m_height;}

  tile_source&  make(int  w, int  h, int  n) noexcept;

  canvas  get_canvas(int  i) noexcept{return canvas(get_pixel_pointer(i),m_width,m_width,m_height);}

        pixel*  get_pixel_pointer(int  i)       noexcept{return &m_stream[m_width*m_height*i];}
  const pixel*  get_pixel_pointer(int  i) const noexcept{return &m_stream[m_width*m_height*i];}

};


class
tile_map
{
  int  m_width =0;
  int  m_height=0;

  std::vector<int>  m_table;

public:
  tile_map&  resize(int  w, int  h) noexcept;

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

        int*  get_index_pointer(int  x, int  y)       noexcept{return &m_table[(m_width*y)+x];}
  const int*  get_index_pointer(int  x, int  y) const noexcept{return &m_table[(m_width*y)+x];}

};


class
tile_map_view
{
  const tile_map*        m_map=nullptr;
  const tile_source*  m_source=nullptr;

  int  m_width =0;
  int  m_height=0;

  point  m_offset;

  void  render_line(int  src_x, int  src_y, int  tile_w, int  tile_h, pixel*  dst) const noexcept;

public:
  tile_map_view() noexcept{}

  tile_map_view&  set_source(const tile_source*  src) noexcept{  m_source = src;  return *this;}
  tile_map_view&  set_map(   const tile_map*     map) noexcept{  m_map    = map;  return *this;}

  tile_map_view&  set_width( int  v) noexcept{  m_width  = v;  return *this;}
  tile_map_view&  set_height(int  v) noexcept{  m_height = v;  return *this;}

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  void  set_offset(int  x, int  y) noexcept{m_offset  = point(x,y);}
  void  add_offset(int  x, int  y) noexcept{m_offset += point(x,y);}

  tile_map_view&  add_offset_x(int  v) noexcept{  m_offset.x += v;  return *this;}
  tile_map_view&  add_offset_y(int  v) noexcept{  m_offset.y += v;  return *this;}

  const point&  get_offset() const noexcept{return m_offset;}

  void  correct_offset() noexcept;

  void  render(const canvas&  cv) const noexcept;

};



}




#endif




