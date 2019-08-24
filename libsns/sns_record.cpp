#include"libsns/sns_table.hpp"




namespace sns{




void
record::
print() const noexcept
{
  printf("date = %ld(",m_timestamp.get_value());

  time_object  to(m_timestamp);

  to.print();

  printf("), text = \"%s\"",m_content.data());
}



}




