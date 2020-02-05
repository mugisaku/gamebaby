#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{




uint32_t
enum_type_info::
m_id_source;


void
enum_type_info::
push(std::string_view  name, int  value) noexcept
{
  m_enumerator_list.emplace_back(name,value);
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




