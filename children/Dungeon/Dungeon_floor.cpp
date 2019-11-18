#include"children/Dungeon/Dungeon.hpp"




namespace Dungeon{


void
floor::
reset(structure&  st, int  n) noexcept
{
  m_structure = &st;
  m_number    =   n;

    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      auto&  sp = m_table[y][x];

      sp.m_floor = this;

      sp.m_point = {x,y};

      sp.m_event_code = "";
    }}
}


}




