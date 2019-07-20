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


template<typename  T>
void
reduce(T&  x, T&  y, T&  w, T&  h) noexcept
{
  x += 1;
  y += 1;
  w -= 2;
  h -= 2;
}


void
draw_rect(direct_color_image&  img, int  line_width, double  time, double  t) noexcept
{
  int  n = 0;

    while(t > time)
    {
      t -= time;

      ++n;
    }


  double  rate = (n&1)? (t/time):1.0-(t/time);

  line_width = 1+(line_width*rate);

  int  w = (img.get_width() *rate);
  int  h = (img.get_height()*rate);

  int  x = (img.get_width() /2)-(w/2);
  int  y = (img.get_height()/2)-(h/2);

  draw_rect(img,bg_l,x,y,w,h);

  reduce(x,y,w,h);

    while(line_width--)
    {
      draw_rect(img,fg_l,x,y,w,h);

      reduce(x,y,w,h);
    }


  draw_rect(img,bg_l,x,y,w,h);
}


}




int
main(int  argc, char**  argv)
{
  constexpr double    fast_sec =  6;
  constexpr double  middle_sec = 15;
  constexpr double    slow_sec = 30;

  int  time_ms = slow_sec*1000*2;

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

      draw_cross(img,fg_l);

      draw_rect(img,6,  slow_sec*1000,t);
      draw_rect(img,6,middle_sec*1000,t);
      draw_rect(img,6,  fast_sec*1000,t);

      ani.append(img);
    }


  auto  ls = ani.build(delay_ms);

  ls.write_png_to_file("eyesquat.apng");


  return 0;
}




