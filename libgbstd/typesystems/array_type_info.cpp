#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{




int  array_type_info::get_size()  const noexcept{return m_base_type_info.get_size()*m_number_of_elements;}
int  array_type_info::get_align() const noexcept{return m_base_type_info.get_align();}


std::string
array_type_info::
make_id() const noexcept
{
  char  buf[48];

  snprintf(buf,sizeof(buf),"a%d",m_number_of_elements);

  return m_base_type_info.get_id()+buf;
}




}}



