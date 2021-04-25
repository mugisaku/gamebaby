#include"libgbstd/vms/typesystem.hpp"




namespace gbstd{
namespace typesystem{




int
enum_decl::
size() const noexcept
{
  return m_def? m_def->size():0;
}


enum_def&
enum_decl::
set_def(enum_def&&  def) noexcept
{
  m_def = std::make_unique<enum_def>(std::move(def));

  return *m_def;
}


void
enum_decl::
print() const noexcept
{
    if(m_def)
    {
      m_def->print();
    }
}




enumerator&
enum_def::
push(std::u16string_view  name, int  value) noexcept
{
  return m_enumerator_list.emplace_back(name,value);
}


const int64_t*
enum_def::
find(std::u16string_view  name) const noexcept
{
    for(auto&  en: m_enumerator_list)
    {
        if(en.name() == name)
        {
          return &en.value();
        }
    }


  return nullptr;
}


void
enum_def::
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




