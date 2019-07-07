#include"libgbpng/png.hpp"


using namespace gbpng;


void
process(animation_builder&  ani, const char*  path)
{
  printf("append %s.\n",path);

  direct_color_image  img(path);

  auto  tmp = img.make_indexed_color_image();

    if(!ani.get_number_of_frames())
    {
      image_header  ihdr(img.get_width(),img.get_height(),pixel_format::indexed);

      ihdr.set_bit_depth(2);

      ihdr.check_error();

      ani.reset(ihdr,40);
    }


  ani.append(img);
}


int
main(int  argc, char**  argv)
{
    if(argc < 2)
    {
      return 0;
    }


  ++argv;

    try
    {
      animation_builder  ani;

      process(ani,*argv++);

        while(*argv)
        {
          process(ani,*argv++);
        }


      auto  anils = ani.build();

      anils.write_png_to_file("__output.png");

      printf("done.\n");
    }


    catch(const std::exception&  e)
    {
      printf("error: %s\n",e.what());

      return -1;
    }


  return 0;
}




