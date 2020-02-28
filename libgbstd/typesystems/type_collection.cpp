#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




const type_entry&
type_collection::
push(std::string_view  name, const type_info&  ti) noexcept
{
  m_entry_table.emplace_back(name,ti);

  return m_entry_table.back();
}


const type_info*
type_collection::
find_by_id(std::string_view  id) const noexcept
{
    for(auto&  ent: m_entry_table)
    {
        if(ent.get_info().get_id() == id)
        {
          return &ent.get_info();
        }
    }


  return nullptr;
}


const type_info*
type_collection::
find_by_name(std::string_view  name) const noexcept
{
ENTRY:
    for(auto&  ent: m_entry_table)
    {
        if(ent.get_name() == name)
        {
          return &ent.get_info();
        }
    }


    for(auto&  a: m_alias_table)
    {
        if(a.first == name)
        {
          name = a.second;

          goto ENTRY;
        }
    }


  return nullptr;
}


const type_entry*
type_collection::
find_entry(const type_info&  ti) const noexcept
{
    for(auto&  ent: m_entry_table)
    {
        if(&ent.get_info() == &ti)
        {
          return &ent;
        }
    }


  return nullptr;
}


bool
type_collection::
make_alias(std::string_view  target_name, std::string_view  new_name) noexcept
{
  m_alias_table.emplace_back(std::make_pair(new_name,target_name));

  return true;
}




void
type_collection::
print() const noexcept
{
    for(auto&  ent: m_entry_table)
    {
      ent.print();

      printf("\n");
    }


    for(auto&  a: m_alias_table)
    {
      printf("alias %s = %s\n",a.first.data(),a.second.data());
    }
}




}}




