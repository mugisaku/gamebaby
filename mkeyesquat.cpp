#include"libgbpng/png.hpp"




using namespace gbpng;


namespace{


constexpr int  g_screen_w = 16*26;
constexpr int  g_screen_h = 16*26;

constexpr uint8_t  fg_l = 0x00;
constexpr uint8_t  bg_l = 0xFF;




void
put(uint8_t  l, uint8_t*  p) noexcept
{
  *p++ =   l;
  *p++ =   l;
  *p++ =   l;
  *p++ = 255;
}


void
fill(direct_color_image&  img, uint8_t  l) noexcept
{
  auto  it     = img.get_pixel_pointer(0,0);
  auto  it_end = img.get_pixel_pointer(img.get_width(),img.get_height());

    while(it < it_end)
    {
      put(l,it);

      it += 4;
    }
}


void
draw_cross(direct_color_image&  img, uint8_t  l) noexcept
{
  int  w = img.get_width() ;
  int  h = img.get_height();

  auto  a = img.get_pixel_pointer(0  ,h/2);
  auto  b = img.get_pixel_pointer(w/2,0  );

    for(int  x = 0;  x < w;  ++x)
    {
      put(l,a+(4*x));
    }


    for(int  y = 0;  y < h;  ++y)
    {
      put(l,b+(4*w*y));
    }
}


void
draw_rect(direct_color_image&  img, uint8_t  l, int  x, int  y, int  w, int  h) noexcept
{
  auto  base = img.get_pixel_pointer(x,y);

  int  pitch = 4*img.get_width();

    if(w)
    {
      auto  a = base              ;
      auto  b = base+(pitch*(h-1));

        for(int  xx = 0;  xx < w;  ++xx)
        {
          put(l,a+(4*xx));
          put(l,b+(4*xx));
        }
    }


    if(h)
    {
      auto  a = base          ;
      auto  b = base+(4*(w-1));

        for(int  yy = 0;  yy < h;  ++yy)
        {
          put(l,a+(pitch*yy));
          put(l,b+(pitch*yy));
        }
    }
}


void
draw_rect(direct_color_image&  img, uint8_t  l, int  n, double  time, double  t) noexcept
{
  auto  diff = (time-t)*(1-(t/time));

  int  w = (img.get_width() /time*diff);
  int  h = (img.get_height()/time*diff);

  int  x = (img.get_width() /2)-(w/2);
  int  y = (img.get_height()/2)-(h/2);

    while(n--)
    {
      draw_rect(img,l,x,y,w,h);

      x += 1;
      y += 1;
      w -= 2;
      h -= 2;

        if((w <= 0) || (h <= 0))
        {
          break;
        }
    }
}


}




int
main(int  argc, char**  argv)
{
  int  sec = 10;

    if(argc == 2)
    {
      sscanf(argv[1],"%d",&sec);
    }


  int  time_ms = sec*1000;

  constexpr int  delay_ms = 60;

  image_header  ihdr(g_screen_w,g_screen_h);

  ihdr.set_bit_depth(2);
  ihdr.set_pixel_format(pixel_format::grayscale);


  animation_builder  ani(ihdr,delay_ms);

  direct_color_image  img;

  img.allocate(g_screen_w,g_screen_h);

    for(double  t = 0;  t < time_ms;  t += delay_ms)
    {
      fill(img,bg_l);

      constexpr int  div_amount = 5;

        for(int  n = 0;  n < div_amount;  ++n)
        {
          draw_rect(img,fg_l,1,time_ms,time_ms/div_amount*(1+n));
        }


      draw_cross(img,fg_l);

      draw_rect(img,fg_l,3,time_ms,t);

      ani.append(img);
    }


  auto  ls = ani.build(delay_ms);

  ls.write_png_to_file("eyesquat.apng");


  return 0;
}




