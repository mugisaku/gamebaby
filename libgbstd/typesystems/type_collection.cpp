#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




type_collection::
type_collection() noexcept:
m_s8_type_info(integer_type_info( true,8)),
m_u8_type_info(integer_type_info(false,8)),
m_s16_type_info(integer_type_info( true,16)),
m_u16_type_info(integer_type_info(false,16)),
m_s32_type_info(integer_type_info( true,32)),
m_u32_type_info(integer_type_info(false,32)),
m_s64_type_info(integer_type_info( true,64)),
m_u64_type_info(integer_type_info(false,64))
{
  push("int8_t",&m_s8_type_info);
  push("uint8_t",&m_u8_type_info);
  push("int16_t",&m_s16_type_info);
  push("uint16_t",&m_u16_type_info);
  push("int32_t",&m_s32_type_info);
  push("uint32_t",&m_u32_type_info);
  push("int64_t",&m_s64_type_info);
  push("uint64_t",&m_u64_type_info);

  push("int",&m_s32_type_info);
  push("uint",&m_u32_type_info);
}


void
type_collection::
push(std::string_view  name, type_info*  ti) noexcept
{
  m_entry_table.emplace_back(std::make_unique<type_entry>(name,ti));
}


type_entry*
type_collection::
find(std::string_view  name) const noexcept
{
    for(auto&  ent: m_entry_table)
    {
        if(ent->get_name() == name)
        {
          return ent.get();
        }
    }


  return nullptr;
}


bool
type_collection::
make_alias(std::string_view  target_name, std::string_view  new_name) noexcept
{
  auto  target = find(target_name);
  auto   exist = find(   new_name);

    if(target && !exist)
    {
      push(new_name,target->get_info());

      return true;
    }


  return false;
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




