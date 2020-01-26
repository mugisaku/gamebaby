#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{




uint32_t
enum_type_info::
m_id_source;


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


const int*
enum_type_info::
find(std::string_view  name) const noexcept
{
    for(auto&  en: m_enumerator_list)
    {
        if(en.get_name() == name)
        {
          return &en.get_value();
        }
    }


  return nullptr;
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
  printf("enum{\n");

    for(auto&  en: m_enumerator_list)
    {
      en.print();

      printf("\n");
    }


  printf("}\n");
}


}}



