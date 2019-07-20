#include"libww/ww_character.hpp"




namespace ww{
namespace battles{




void
character::
draw(const gbstd::canvas&  cv, character&  c) noexcept
{
  c.m_hp_bar.draw(cv,c.m_hp_bar);
  c.m_ap_bar.draw(cv,c.m_ap_bar);
}


void
character::
tick(character&  c) noexcept
{
  constexpr gbstd::fixed_t  length_max = 80;

  auto  hp     = c.m_product_body.m_hp    ;
  auto  hp_max = c.m_product_body.m_hp_max;

    if((c.m_last_hp     != hp    ) ||
       (c.m_last_hp_max != hp_max))
    {
      c.m_last_hp     = hp    ;
      c.m_last_hp_max = hp_max;

      auto  n = length_max/hp_max*hp;

      c.m_hp_bar.set_desired_length(n).update_increment();
    }


  auto  l = c.m_hp_bar.get_length();

  gbstd::color  color = (l >= length_max/2)? gbstd::colors::white
                       :(l >= length_max/4)? gbstd::colors::yellow
                       :                     gbstd::colors::red;

  c.m_hp_bar.set_color(color);

  c.m_hp_bar.tick(c.m_hp_bar);


  auto  ap = c.m_ap;

    if(c.m_last_ap != ap)
    {
      c.m_last_ap = ap;

      auto  n = length_max/character::m_ap_max*ap;

      c.m_ap_bar.set_desired_length(n).update_increment();
    }


  c.m_ap_bar.tick(c.m_ap_bar);
}




}}




