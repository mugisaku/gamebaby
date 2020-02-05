#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




const type_info&
type_collection::
push(std::string_view  name, const type_info&  ti) noexcept
{
  m_entry_table.emplace_back(name,ti);

  return ti;
}


const type_info&
type_collection::
push(std::string_view  name, std::unique_ptr<type_info>&&  ti) noexcept
{
  return push(name,push(std::move(ti)));
}


const type_info&
type_collection::
push(std::unique_ptr<type_info>&&  ti) noexcept
{
  auto&  ref = *ti;

  m_info_table.emplace_back(std::move(ti));

  return ref;
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


void
type_collection::
push_c_like_types() noexcept
{
  push(     "void",std::make_unique<type_info>(*this,void_type_info{}));
  push("nullptr_t",std::make_unique<type_info>(*this,null_pointer_type_info{}));
  push("geneptr_t",std::make_unique<type_info>(*this,generic_pointer_type_info{}));
  push(     "bool",std::make_unique<type_info>(*this,boolean_type_info{}));

  push( "int8_t",std::make_unique<type_info>(*this,integer_type_info( 8)));
  push("int16_t",std::make_unique<type_info>(*this,integer_type_info(16)));
  push("int32_t",std::make_unique<type_info>(*this,integer_type_info(32)));
  push("int64_t",std::make_unique<type_info>(*this,integer_type_info(64)));

  push( "uint8_t",std::make_unique<type_info>(*this,unsigned_integer_type_info( 8)));
  push("uint16_t",std::make_unique<type_info>(*this,unsigned_integer_type_info(16)));
  push("uint32_t",std::make_unique<type_info>(*this,unsigned_integer_type_info(32)));
  push("uint64_t",std::make_unique<type_info>(*this,unsigned_integer_type_info(64)));

  make_alias("int32_t","int");
  make_alias("uint32_t","uint");
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




