#include"children/Dungeon/Dungeon.hpp"




namespace Dungeon{


void
structure::
reset() noexcept
{
    for(int  i = 0;  i < m_number_of_floors;  ++i)
    {
      m_floors[i].reset(*this,i);
    }
}


}




