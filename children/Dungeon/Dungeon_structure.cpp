#include"children/Dungeon/Dungeon.hpp"




namespace Dungeon{


void
structure::
reset() noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      auto&  sp = m_table[y][x];

      sp.m_structure = this;

      sp.m_point = {x,y};

        for(auto&  w: sp.m_walls)
        {
          w.m_space = &sp;

          w.m_passage = true;

          w.m_event_index = 0;
        }
    }}
}


}




