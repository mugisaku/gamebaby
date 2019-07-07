#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




radio_button::
radio_button(operating_node&  root) noexcept:
checkbox(root)
{
  revise_to_radio();
}


radio_button::
radio_button(operating_node&  root, const checkbox&  member) noexcept:
checkbox(root,member)
{
  revise_to_radio();
}




}}




