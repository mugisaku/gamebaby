#ifndef Children_hpp
#define Children_hpp


#include"libgbstd/control.hpp"
#include"libgbstd/image.hpp"
#include"libgbstd/character.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/random.hpp"
#include"libgbstd/ilc.hpp"
#include"libgbstd/gbstd.hpp"




struct
game_information
{
  int  screen_width ;
  int  screen_height;

  std::string_view          title;
  std::string_view  category_name;
  std::string_view    description;

  void  (*boot)();

};




#endif




