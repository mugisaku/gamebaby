#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{


void
enum_type_info::
push(std::string_view  name) noexcept
{
  m_enumerator_list.emplace_back(name,m_last_value++);
}


void
enum_type_info::
push(std::string_view  name, int  value) noexcept
{
  m_last_value = value;

  push(name);
}


std::string
enum_type_info::
get_id() const noexcept
{
  char  buf[48];

  snprintf(buf,sizeof(buf),"en%d",m_last_value);

  return buf;
}


void
enum_type_info::
print() const noexcept
{
    for(auto&  en: m_enumerator_list)
    {
      en.print();

      printf("\n");
    }
}


}}




