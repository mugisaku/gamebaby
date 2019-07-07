#include"libgbstd/image.hpp"
#include<cstring>



namespace gbstd{




/*
void
canvas::
draw_glyph(color  col, const glyph&  gl, int  x, int  y) const noexcept
{
  auto  p = &gl.data[0];

    for(int  yy = 0;  yy < g_font_height;  yy += 1)
    {
      auto   dst = get_pixel_pointer(x,y+yy);
      auto  code = *p++;

        for(int  xx = 0;  xx < g_font_width;  xx += 1)
        {
            if(code&0x80)
            {
              dst->color = col;
            }


           dst  += 1;
          code <<= 1;
        }
    }
}
*/


void
canvas::
draw_glyph(color  col, const glyph&  gl, int  x, int  y) const noexcept
{
  color  nega_col(~col.get_r7(),~col.get_g7(),~col.get_b7());

  auto  p = &gl.data[0];

    for(int  yy = 0;  yy < g_font_height;  yy += 1)
    {
      auto       dst = get_pixel_pointer(x,y+yy);
      auto  code = *p++;

      auto  side_code = ((code<<1)|(code>>1))&~code;

      int  t = code|(side_code<<8);

        for(int  xx = 0;  xx < g_font_width;  xx += 1)
        {
            if(t&0x80)
            {
              dst->color = col;
            }


            if(t&(0x80<<8))
            {
              dst->color = nega_col;
            }


          dst  += 1;
            t <<= 1;
        }
    }
}


void
canvas::
draw_glyph_safely(color  col, const glyph&  gl, int  x, int  y) const noexcept
{
  auto  p = &gl.data[0];

    for(int  yy = 0;  yy < g_font_height;  yy += 1)
    {
      auto   dst = get_pixel_pointer(x,y+yy);
      auto  code = *p++;

      int  yyy = y+yy;

        if((yyy >= 0) && (yyy < get_height()))
        {
            for(int  xx = 0;  xx < g_font_width;  xx += 1)
            {
              int  xxx = x+xx;

                if((xxx >= 0) && (xxx < get_width()))
                {
                    if(code&0x80)
                    {
                      dst->color = col;
                    }
                }


               dst  += 1;
              code <<= 1;
            }
        }
    }
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




}





