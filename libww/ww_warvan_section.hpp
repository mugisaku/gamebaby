#ifndef ww_warvan_section_hpp_is_included
#define ww_warvan_section_hpp_is_included


#include"libgbstd/menu.hpp"
#include"libww/ww_character.hpp"
#include"libww/ww_bar.hpp"
#include"libww/ww_spilling_text.hpp"
#include<list>
#include<vector>




namespace ww{



class system;


class
warvan_section
{
  using this_type = warvan_section;

  friend class system;

  system&  m_system;

  gbstd::clock  m_clock;

  gbstd::task  m_task;

  gbstd::windows::stack  m_stack;
  gbstd::menus::table  m_table;
  gbstd::menus::view   m_view;

  static void  run(gbstd::execution&  exec, this_type&  fr) noexcept;

  static void  tick(                          this_type&  fr) noexcept;
  static void  draw(const gbstd::canvas&  cv, this_type&  fr) noexcept;

public:
  warvan_section(system&  sys, gbstd::execution&  exec) noexcept;
 ~warvan_section();

};


}




#endif




