#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




type_info&
type_collection::
operator[](std::string_view  name) noexcept
{
  auto  ti = find(name);

    if(ti)
    {
      push(name,new type_info(*this));
    }


  return *ti;
}




void
type_collection::
push(std::string_view  name, type_info*  ti) noexcept
{
  m_entry_table.emplace_back(std::make_unique<type_entry>(name,ti));
}


type_info*
type_collection::
find(std::string_view  name) const noexcept
{
    for(auto&  ent: m_entry_table)
    {
        if(ent->get_name() == name)
        {
          return ent->get_info();
        }
    }


  return nullptr;
}


void
type_collection::
push_c_like_types() noexcept
{
  push(     "void",new type_info(*this,void_type_info{}));
  push("nullptr_t",new type_info(*this,null_pointer_type_info{}));
  push("geneptr_t",new type_info(*this,generic_pointer_type_info{}));
  push(     "bool",new type_info(*this,boolean_type_info{}));

  push(  "int8_t",new type_info(*this,integer_type_info( true, 8)));
  push( "uint8_t",new type_info(*this,integer_type_info(false, 8)));
  push( "int16_t",new type_info(*this,integer_type_info( true,16)));
  push("uint16_t",new type_info(*this,integer_type_info(false,16)));
  push( "int32_t",new type_info(*this,integer_type_info( true,32)));
  push("uint32_t",new type_info(*this,integer_type_info(false,32)));
  push( "int64_t",new type_info(*this,integer_type_info( true,64)));
  push("uint64_t",new type_info(*this,integer_type_info(false,64)));

  make_alias("int32_t","int");
  make_alias("uint32_t","uint");
}


bool
type_collection::
make_alias(std::string_view  target_name, std::string_view  new_name) noexcept
{
  auto  target = find(target_name);
  auto   exist = find(   new_name);

    if(target && !exist)
    {
      push(new_name,target);

      return true;
    }


  return false;
}




type_info&
type_collection::
create_struct(std::string_view  name) noexcept
{
  auto  ti = new type_info(*this,struct_type_info());

  push(name,ti);

  return *ti;
}


type_info&
type_collection::
create_union(std::string_view  name) noexcept
{
  auto  ti = new type_info(*this,union_type_info());

  push(name,ti);

  return *ti;
}


type_info&
type_collection::
create_enum(std::string_view  name) noexcept
{
  auto  ti = new type_info(*this,enum_type_info());

  push(name,ti);

  return *ti;
}




void
type_collection::
print() const noexcept
{
    for(auto&  ent: m_entry_table)
    {
      ent->print();

      printf("\n");
    }
}




}}




