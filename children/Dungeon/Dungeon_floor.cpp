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

      nd.m_symbol = (!x || !y || (x == (m_width-1)) || (y == (m_height-1)))? '*'
                   :( nd.is_x_way() &&  nd.is_y_way())? 'O'
                   :' ';

      nd.m_wayhole = false;

      nd.m_event_code = "";
    }}
}


void
floor::
unput_way(gbstd::point  pt) noexcept
{
  auto&  nd = get_node(pt);

    if(nd.is_up_way())
    {
        if(m_number < (structure::m_number_of_floors-1))
        {
          auto&  target_floor = m_structure->m_floors[m_number+1];

          target_floor.get_node(pt).be_space();
        }
    }

  else
    if(nd.is_down_way())
    {
        if(m_number >= 1)
        {
          auto&  target_floor = m_structure->m_floors[m_number-1];

          target_floor.get_node(pt).be_space();
        }
    }


  nd.be_space();
}


bool
floor::
put_up_way(gbstd::point  pt) noexcept
{
    if(m_number < (structure::m_number_of_floors-1))
    {
      auto&  target_floor = m_structure->m_floors[m_number+1];

                   get_node(pt).be_up_way();
      target_floor.get_node(pt).be_down_way();

      return true;
    }


  return false;
}


bool
floor::
put_down_way(gbstd::point  pt) noexcept
{
    if(m_number >= 1)
    {
      auto&  target_floor = m_structure->m_floors[m_number-1];

                   get_node(pt).be_down_way();
      target_floor.get_node(pt).be_up_way();

      return true;
    }


  return false;
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




