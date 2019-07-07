#include"libge/core.hpp"




namespace ge{




void
core_paint::
draw_line(gbstd::color  color) noexcept
{
  auto  cv = m_core.get_canvas();

  gbstd::liner  l(m_saved_drawing_point,m_drawing_point);

  try_to_push_nonsolid_record();

    for(;;)
    {
      modify_dot(cv,color,{l.get_x(),l.get_y()});

        if(!l.get_distance())
        {
          break;
        }


      l.step();
    }


  try_to_push_solid_record();
}


void
core_paint::
draw_rect(gbstd::color  color, gbstd::rectangle  rect) noexcept
{
  auto  cv = m_core.get_canvas();

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < rect.h;  ++yy)
    {
      modify_dot(cv,color,{rect.x         ,rect.y+yy});
      modify_dot(cv,color,{rect.x+rect.w-1,rect.y+yy});
    }


    for(int  xx = 0;  xx < rect.w;  ++xx)
    {
      modify_dot(cv,color,{rect.x+xx,rect.y         });
      modify_dot(cv,color,{rect.x+xx,rect.y+rect.h-1});
    }


  try_to_push_solid_record();
}


void
core_paint::
fill_rect(gbstd::color  color, gbstd::rectangle  rect) noexcept
{
  auto  cv = m_core.get_canvas();

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < rect.h;  ++yy){
    for(int  xx = 0;  xx < rect.w;  ++xx){
      modify_dot(cv,color,{rect.x+xx,rect.y+yy});
    }}


  try_to_push_solid_record();
}


void
core_paint::
fill_area(gbstd::color  color) noexcept
{
  auto  cv = m_core.get_canvas();

  auto  target_color = cv.get_pixel_pointer(m_drawing_point.x,m_drawing_point.y)->color;

    if(target_color == color)
    {
      return;
    }


  const int  w = cv.get_width() ;
  const int  h = cv.get_height();

  std::vector<uint8_t>  table(w*h,0);

  try_to_push_nonsolid_record();


  std::vector<gbstd::point>  stack;

  int  i = 0;

  stack.emplace_back(m_drawing_point);

    while(i < stack.size())
    {
      auto  pt = stack[i++];

      auto&  flag = table[(w*pt.y)+pt.x];

        if(!flag)
        {
          flag = 1;

          auto&  pix = *cv.get_pixel_pointer(pt.x,pt.y);

            if(pix.color == target_color)
            {
              m_recorder.put(pix.color,pt.x,
                                       pt.y);

              pix.color = color;

                if(pt.x      ){stack.emplace_back(pt.x-1,pt.y  );}
                if(pt.y      ){stack.emplace_back(pt.x  ,pt.y-1);}
                if(pt.x < w-1){stack.emplace_back(pt.x+1,pt.y  );}
                if(pt.y < h-1){stack.emplace_back(pt.x  ,pt.y+1);}
            }
        }
    }


  try_to_push_solid_record();

  m_drawing_is_fixed = true;
}




}




