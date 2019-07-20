#include"libww/ww_party.hpp"




namespace ww{




party::member::
member(std::u16string_view  name, int  hp_max, battles::position  pos) noexcept
{
  m_head.m_name     =   name;
  m_body.m_hp       = hp_max;
  m_body.m_hp_max   = hp_max;
  m_body.m_position =    pos;

}




void
party::
push(std::initializer_list<member>  ls) noexcept
{
    for(auto&  m: ls)
    {
      m_member_table[m_number_of_members++] = m;
    }
}




}




