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


void
type_collection::
push_c_like_types() noexcept
{
  push("null_t",type_infos::null);
  push("void",type_infos::void_);
  push("undefined",type_infos::undefined);
  push("nullptr_t",type_infos::null_pointer);

  push("geneptr_t",std::make_unique<type_info>(generic_pointer_type_info(m_pointer_size)));
  push(     "bool",std::make_unique<type_info>(boolean_type_info(m_boolean_size)));

  push( "int8_t",std::make_unique<type_info>(integer_type_info(1)));
  push("int16_t",std::make_unique<type_info>(integer_type_info(2)));
  push("int32_t",std::make_unique<type_info>(integer_type_info(4)));
  push("int64_t",std::make_unique<type_info>(integer_type_info(8)));

  push( "uint8_t",std::make_unique<type_info>(unsigned_integer_type_info(1)));
  push("uint16_t",std::make_unique<type_info>(unsigned_integer_type_info(2)));
  push("uint32_t",std::make_unique<type_info>(unsigned_integer_type_info(4)));
  push("uint64_t",std::make_unique<type_info>(unsigned_integer_type_info(8)));

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




