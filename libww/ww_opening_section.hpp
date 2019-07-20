#ifndef ww_opening_section_hpp_is_included
#define ww_opening_section_hpp_is_included


#include"libww/ww_character.hpp"
#include"libww/ww_bar.hpp"
#include"libww/ww_spilling_text.hpp"
#include"libww/ww_party.hpp"
#include"libgbstd/menu.hpp"
#include<list>
#include<vector>




namespace ww{



class system;


struct
scene
{
  gbstd::point  m_text_position_base;

  const char*  m_text_id;

};


class
opening_section: public gbstd::section
{
  friend class system;

  system&  m_system;

  gbstd::clock  m_clock;

  const scene*  m_scene    ;
  const scene*  m_scene_end;

  gbstd::text  m_text;

  int  m_text_base_pos_y;

  uint32_t  m_delay;

  bool  advance() noexcept;

  void  run(gbstd::process&  proc) noexcept;

  void    work(                        ) noexcept override;
  void  render(const gbstd::canvas&  cv) noexcept override;

  void  startup(gbstd::process&  proc) noexcept override;
  void  cleanup(gbstd::process&  proc) noexcept override;

public:
  opening_section(system&  sys) noexcept;

};


}




#endif




