#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{




int
enum_type_decl::
get_size() const noexcept
{
  return m_def? m_def->get_size():0;
}


enum_type_def&
enum_type_decl::
set_def(enum_type_def&&  def) noexcept
{
  m_def = std::make_unique<enum_type_def>(std::move(def));

  return *m_def;
}


void
enum_type_decl::
print() const noexcept
{
    if(m_def)
    {
      m_def->print();
    }
}




enumerator&
enum_type_def::
push(std::string_view  name, int  value) noexcept
{
  return m_enumerator_list.emplace_back(name,value);
}


const int64_t*
enum_type_def::
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
enum_type_def::
print() const noexcept
{
  printf("{\n");

    for(auto&  en: m_enumerator_list)
    {
      en.print();

      printf("\n");
    }


  printf("}\n");
}


}}




