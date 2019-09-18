#include"libgbstd/tile_map.hpp"




namespace gbstd{




tile_source
tile_map::
make_from(const canvas&  cv, int  tile_w, int  tile_h) noexcept
{
  tile_source  src;

  src.make(tile_w,tile_h,0);

  resize(cv.get_width()/tile_w,cv.get_height()/tile_h);

    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x < m_width ;  ++x){
      canvas  tmp_cv(cv,tile_w*x,tile_h*y,tile_w,tile_h);

      *get_index_pointer(x,y) = src.append(tmp_cv,0,0);
    }}


  return std::move(src);
}


tile_map&
tile_map::
resize(int  w, int  h) noexcept
{
  m_width  = w;
  m_height = h;

  m_table.resize(w*h);

    for(auto&  i: m_table)
    {
      i = 0;
    }


  return *this;
}




}




