#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{




template<bool  PointCheck>
void
draw_glyph_tmpl(const canvas&  cv, color  col, const glyph&  gl, int  x, int  y) noexcept
{
  color  nega_col(~col.get_r7(),~col.get_g7(),~col.get_b7());

  auto  p = &gl.data[0];

  uint32_t  t = 0;

  int  xxx    ;
  int  yyy = y;

    for(int  yy = 0;  yy < g_font_height;  yy += 1)
    {
      auto  code = *p++;

      auto   dst = cv.get_pixel_pointer(x,y+yy);
      auto  next = (yy < (g_font_height-1))? *p:0;

      auto  side_code = ((code<<1)|(code>>1))&~code;

      t &= 0xFFFF;
      t |= (((t>>8)^code)<<16);
      t |= ((       code)    );
      t |= ((  side_code)<<16);
      t |= ((  code^next)<<16);

        if constexpr(PointCheck)
        {
          xxx = x;
        }


        for(int  xx = 0;  xx < g_font_width;  xx += 1)
        {
          bool  c = true;

            if constexpr(PointCheck)
            {
              c = ((xxx >= 0) &&
                   (yyy >= 0) &&
                   (xxx < cv.get_width()) &&
                   (yyy < cv.get_height()));

              ++xxx;
            }


            if(c)
            {
                   if(t&(0x80    )){dst->color =      col;}
              else if(t&(0x80<<16)){dst->color = nega_col;}
            }


          dst  += 1;
            t <<= 1;
        }


        if constexpr(PointCheck)
        {
          ++yyy;
        }
    }
}


void
canvas::
draw_glyph(color  col, const glyph&  gl, int  x, int  y) const noexcept
{
  draw_glyph_tmpl<false>(*this,col,gl,x,y);
}


void
canvas::
draw_glyph_safely(color  col, const glyph&  gl, int  x, int  y) const noexcept
{
  draw_glyph_tmpl<true>(*this,col,gl,x,y);
}


void
canvas::
draw_string(color  col, std::string_view  sv, int  x, int  y) const noexcept
{
  utf8_decoder  dec(sv.data());

    while(dec)
    {
      auto  c = static_cast<char16_t>(dec());

        if(!c)
        {
          break;
        }


      draw_glyph(col,get_glyph(c),x,y);

      x += g_font_width;
    }
}




void
canvas::
draw_string(color  col, std::u16string_view  sv, int  x, int  y) const noexcept
{
    for(auto  u16: sv)
    {
      auto  c = u16;

      draw_glyph(col,get_glyph(c),x,y);

      x += g_font_width;
    }
}




void
canvas::
draw_string_safely(color  col, std::string_view  sv, int  x, int  y) const noexcept
{
  utf8_decoder  dec(sv.data());

    while(dec)
    {
      auto  c = static_cast<char16_t>(dec());

        if(!c)
        {
          break;
        }


      draw_glyph_safely(col,get_glyph(c),x,y);

      x += g_font_width;
    }
}




void
canvas::
draw_string_safely(color  col, std::u16string_view  sv, int  x, int  y) const noexcept
{
    for(auto  u16: sv)
    {
      auto  c = u16;

      draw_glyph_safely(col,get_glyph(c),x,y);

      x += g_font_width;
    }
}




void
canvas::
draw_string_as_right_align(color  col, std::u16string_view  sv, int  x, int  y) const noexcept
{
  auto  it     = sv.rbegin();
  auto  it_end = sv.rend();

  x -= g_font_width;

    while(it != it_end)
    {
      auto  c = *it++;

      draw_glyph(col,get_glyph(c),x,y);

      x -= g_font_width;
    }
}


void
canvas::
draw_string_safely_as_right_align(color  col, std::u16string_view  sv, int  x, int  y) const noexcept
{
  auto  it     = sv.rbegin();
  auto  it_end = sv.rend();

  x -= g_font_width;

    while(it != it_end)
    {
      auto  c = *it++;

      draw_glyph_safely(col,get_glyph(c),x,y);

      x -= g_font_width;
    }
}




void
canvas::
draw_string_in_area(color  col, std::string_view     sv, int  x, int  y, int  w, int  h) const noexcept
{
  const int  x_base  = x  ;
  const int  x_limit = x+w;
  const int  y_limit = y+h;

  utf8_decoder  dec(sv.data());

    while(dec)
    {
      auto  c = static_cast<char16_t>(dec());

        if(!c)
        {
          break;
        }


        if(c == '\n')
        {
          x  =        x_base;
          y += g_font_height;

            if(y >= y_limit)
            {
              break;
            }
        }

      else
        {
          draw_glyph(col,get_glyph(c),x,y);

          x += g_font_width;

            if(x >= x_limit)
            {
              x  =        x_base;
              y += g_font_height;

                if(y >= y_limit)
                {
                  break;
                }
            }
        }
    }
}


void
canvas::
draw_string_in_area(color  col, std::u16string_view  sv, int  x, int  y, int  w, int  h) const noexcept
{
  const int  x_base  = x  ;
  const int  x_limit = x+w;
  const int  y_limit = y+h;

    for(auto  u16: sv)
    {
      auto  c = u16;

        if(c == '\n')
        {
          x  =        x_base;
          y += g_font_height;

            if(y >= y_limit)
            {
              break;
            }
        }

      else
        {
          draw_glyph(col,get_glyph(c),x,y);

          x += g_font_width;

            if(x >= x_limit)
            {
              x  =        x_base;
              y += g_font_height;

                if(y >= y_limit)
                {
                  break;
                }
            }
        }
    }
}




}





