#ifndef ww_party_hpp_is_included
#define ww_party_hpp_is_included


#include"libww/ww_character.hpp"




namespace ww{




struct
party
{
  struct member{
    battles::character_head  m_head;
    battles::character_body  m_body;

    member() noexcept{}
    member(std::u16string_view  name, int  hp_max, battles::position  pos=battles::positions::front) noexcept;

  };


  static constexpr int  m_table_length = 5;

  int  m_number_of_members=0;

  member  m_member_table[m_table_length];

  void  push(std::initializer_list<member>  ls) noexcept;

  void  clear() noexcept{m_number_of_members = 0;}

};


}




#endif




