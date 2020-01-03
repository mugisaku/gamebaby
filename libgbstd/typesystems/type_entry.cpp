#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




type_entry::
type_entry(std::string_view  name, type_info*  ti) noexcept:
m_name(name), m_type_info(ti)
{
}




void
type_entry::
print() const noexcept
{
    if(m_type_info)
    {
      m_type_info->print();

      printf(" id: \"%s\", ",m_type_info->get_id().data());
    }


  printf("%s",m_name.data());
}




}}




