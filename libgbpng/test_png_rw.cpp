#include"libgbpng/png.hpp"


using namespace gbpng;


void
process(const char*  path)
{
  chunk_list  ls;

  ls.read_png_from_file(path);

  printf("%s\n",path);

  ls.print();

  printf("\n\n");

  
  chunk_set  set(ls);

  image_source  isrc;

  isrc.ihdr = set.get_image_header();

  auto  idat = set.get_image_data();

  isrc.data = idat.extract(isrc.ihdr);

  auto  trns_chunk = set.get_trns_chunk();
  auto  plte_chunk = set.get_plte_chunk();

    if(trns_chunk)
    {
      isrc.trns.assign(*trns_chunk,isrc.ihdr.get_pixel_format());
    }


    if(plte_chunk)
    {
      isrc.plte.assign(*plte_chunk);
    }


  direct_color_image  img(isrc);


  img.write_png_to_file("__save_g1.png",pixel_format::grayscale,1);
  img.write_png_to_file("__save_g2.png",pixel_format::grayscale,2);
  img.write_png_to_file("__save_g4.png",pixel_format::grayscale,4);
  img.write_png_to_file("__save_g8.png",pixel_format::grayscale,8);

  img.write_png_to_file("__save_rgb.png",pixel_format::rgb,8);
  img.write_png_to_file("__save_rgba.png",pixel_format::rgba,8);


  auto  tmp = img.make_indexed_color_image();

  tmp.write_png_to_file("__save_1.png",1);
  tmp.write_png_to_file("__save_2.png",2);
  tmp.write_png_to_file("__save_4.png",4);
  tmp.write_png_to_file("__save_8.png",8);


  image_header  ihdr(img.get_width(),img.get_height(),pixel_format::rgba);

  ihdr.check_error();


  animation_builder  ani(ihdr,200);

  img.read_png_from_file("__save_g1.png");

  ani.append(img);

  img.read_png_from_file("__save_g2.png");

  ani.append(img);

  img.read_png_from_file("__save_g4.png");

  ani.append(img);

  img.read_png_from_file("__save_g8.png");

  ani.append(img);

  img.read_png_from_file("__save_rgb.png");

  ani.append(img);

  img.read_png_from_file("__save_1.png");

  ani.append(img);

  img.read_png_from_file("__save_2.png");

  ani.append(img);

  img.read_png_from_file("__save_4.png");

  ani.append(img);

  img.read_png_from_file("__save_8.png");

  ani.append(img);

  img.read_png_from_file("__save_rgba.png");

  ani.append(img);


  auto  anils = ani.build();

  anils.write_png_to_file("__output.png");
}


int
main(int  argc, char**  argv)
{
    if(argc != 2)
    {
      return 0;
    }


  auto  path = argv[1];

    try
    {
      process(path);
    }


    catch(const std::exception&  e)
    {
      printf("error: %s\n",e.what());

      return -1;
    }


  return 0;
}




