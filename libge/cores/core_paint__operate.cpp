#include"libge/core.hpp"




namespace ge{


namespace{
void
transform(int&  a_w, int  b_x, int  b_w) noexcept
{
    if((b_x+a_w) >= b_w)
    {
      a_w = b_w-b_x;
    }
}
void
transform(gbstd::rectangle&  a, const gbstd::rectangle&  b) noexcept
{
  transform(a.w,b.x,b.w);
  transform(a.h,b.y,b.h);
}
}


core_paint&
core_paint::
paste(bool  layer) noexcept
{
  try_to_push_nonsolid_record();


  auto  cv = m_core.get_canvas();

  gbstd::rectangle  src_rect = m_clipped_image.get_rectangle();
  gbstd::rectangle  dst_rect(m_drawing_point,cv.get_width(),cv.get_height());

  transform(src_rect,dst_rect);

  gbstd::canvas  clipped_cv(m_clipped_image);

    for(int  y = 0;  y < src_rect.h;  ++y){
    for(int  x = 0;  x < src_rect.w;  ++x){
      auto  pix = *clipped_cv.get_pixel_pointer(src_rect.x+x,src_rect.y+y);

        if(!layer || pix.color)
        {
          modify_dot(cv,pix.color,{dst_rect.x+x,dst_rect.y+y});
        }
    }}


  try_to_push_solid_record();

  return *this;
}




core_paint&
core_paint::
revolve() noexcept
{
  auto  cv = m_core.get_canvas();

  auto  tmp = cv.make_image();

  const int  w = std::min(m_operation_rect.w,m_operation_rect.h);

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < w;  ++yy){
    for(int  xx = 0;  xx < w;  ++xx){
      auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y+yy);

      modify_dot(cv,pix,{m_operation_rect.x+w-1-yy,m_operation_rect.y+xx});
    }}


  try_to_push_solid_record();

  return *this;
}


core_paint&
core_paint::
reverse_horizontally() noexcept
{
  auto  cv = m_core.get_canvas();

  auto  tmp = cv.make_image();

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w;  ++xx){
      auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y+yy);

      modify_dot(cv,pix,{m_operation_rect.x+xx,m_operation_rect.y+m_operation_rect.h-1-yy});
    }}


  try_to_push_solid_record();

  return *this;
}


core_paint&
core_paint::
reverse_vertically() noexcept
{
  auto  cv = m_core.get_canvas();

  auto  tmp = cv.make_image();

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w;  ++xx){
      auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y+yy);

      modify_dot(cv,pix,{m_operation_rect.x+m_operation_rect.w-1-xx,m_operation_rect.y+yy});
    }}


  try_to_push_solid_record();

  return *this;
}


core_paint&
core_paint::
mirror_vertically() noexcept
{
  auto  cv = m_core.get_canvas();

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h  ;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w/2;  ++xx){
      auto  pix = *cv.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y+yy);

      modify_dot(cv,pix,{m_operation_rect.x+m_operation_rect.w-1-xx,m_operation_rect.y+yy});
    }}


  try_to_push_solid_record();

  return *this;
}




core_paint&
core_paint::
shift_up(bool  rotate) noexcept
{
  auto  cv = m_core.get_canvas();

  auto  tmp = cv.make_image();

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h-1;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w  ;  ++xx){
      auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y+yy+1);

      modify_dot(cv,pix,{m_operation_rect.x+xx,m_operation_rect.y+yy});
    }}


    if(rotate)
    {
        for(int  xx = 0;  xx < m_operation_rect.w;  ++xx)
        {
          auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y);

          modify_dot(cv,pix,{m_operation_rect.x+xx,m_operation_rect.y+m_operation_rect.h-1});
        }
    }


  try_to_push_solid_record();

  return *this;
}


core_paint&
core_paint::
shift_left(bool  rotate) noexcept
{
  auto  cv = m_core.get_canvas();

  auto  tmp = cv.make_image();

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h  ;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w-1;  ++xx){
      auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx+1,m_operation_rect.y+yy);

      modify_dot(cv,pix,{m_operation_rect.x+xx,m_operation_rect.y+yy});
    }}


    if(rotate)
    {
        for(int  yy = 0;  yy < m_operation_rect.h;  ++yy)
        {
          auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x,m_operation_rect.y+yy);

          modify_dot(cv,pix,{m_operation_rect.x+m_operation_rect.w-1,m_operation_rect.y+yy});
        }
    }


  try_to_push_solid_record();

  return *this;
}


core_paint&
core_paint::
shift_right(bool  rotate) noexcept
{
  auto  cv = m_core.get_canvas();

  auto  tmp = cv.make_image();

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h;  ++yy){
    for(int  xx = 1;  xx < m_operation_rect.w;  ++xx){
      auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx-1,m_operation_rect.y+yy);

      modify_dot(cv,pix,{m_operation_rect.x+xx,m_operation_rect.y+yy});
    }}


    if(rotate)
    {
        for(int  yy = 0;  yy < m_operation_rect.h;  ++yy)
        {
          auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+m_operation_rect.w-1,m_operation_rect.y+yy);

          modify_dot(cv,pix,{m_operation_rect.x,m_operation_rect.y+yy});
        }
    }


  try_to_push_solid_record();

  return *this;
}


core_paint&
core_paint::
shift_down(bool  rotate) noexcept
{
  auto  cv = m_core.get_canvas();

  auto  tmp = cv.make_image();

  try_to_push_nonsolid_record();

    for(int  yy = 1;  yy < m_operation_rect.h;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w;  ++xx){
      auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y+yy-1);

      modify_dot(cv,pix,{m_operation_rect.x+xx,m_operation_rect.y+yy});
    }}


    if(rotate)
    {
        for(int  xx = 0;  xx < m_operation_rect.w;  ++xx)
        {
          auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y+m_operation_rect.h-1);

          modify_dot(cv,pix,{m_operation_rect.x+xx,m_operation_rect.y});
        }
    }


  try_to_push_solid_record();

  return *this;
}




}




