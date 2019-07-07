#include"libge/color_handler.hpp"




namespace ge{




color_handler
make_color_handler(gbstd::widgets::operating_node&  root, gbstd::color&  color) noexcept
{
  namespace colors = gbstd::colors;

  auto  color_list = {
    colors::black,
    colors::dark_gray,
    colors::gray,
    colors::light_gray,
    colors::white,
    colors::red,
    colors::green,
    colors::blue,
    colors::yellow,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
  };


  auto  holder = new color_holder(color_list,color);
  auto  maker  = new color_maker(root,color);

  maker->set_userdata(*holder);
  holder->set_userdata(*maker);

  auto&   maker_frame = root.create_frame("color").set_content(*maker);
  auto&  holder_frame = root.create_frame("palette").set_content(*holder);

  return color_handler{maker_frame,holder_frame};
}




}




