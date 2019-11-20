#include"children/Dungeon/Dungeon.hpp"




namespace Dungeon{




node
floor::
m_null_node;


void
floor::
reset(structure&  st, int  n) noexcept
{
  m_structure = &st;
  m_number    =   n;

    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      auto&  nd = m_table[y][x];

      nd.m_floor = this;

      nd.m_point = {x,y};

      nd.m_symbol = (!x || !y || (nd.is_x_way() && nd.is_y_way()) || (x == (m_width-1)) || (y == (m_height-1)))? '+'
                   :' ';

      nd.m_wayhole = false;

      nd.m_event_code = "";
    }}
}


void
floor::
print() const noexcept
{
    for(int  y = 0;  y < m_height;  ++y)
    {
        for(int  x = 0;  x <  m_width;  ++x)
        {
          auto&  nd = m_table[y][x];

          printf("%c",nd.m_symbol);
        }


      printf("\n");
    }
}


}




