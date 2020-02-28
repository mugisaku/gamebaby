#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




type_info&
type_info::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::function): m_data.fnsig.~function_signature();break;
  case(kind::array   ): m_data.arr_ti.~array_type_info();break;
  case(kind::struct_ ): m_data.str_ti.~struct_type_info();break;
  case(kind::union_  ): m_data.uni_ti.~union_type_info();break;
  case(kind::enum_   ): m_data.enu_ti.~enum_type_info();break;
    }


  m_kind = kind::null;

  m_id.clear();

  m_derivation->clear();

  return *this;
}


int
type_info::
get_size() const noexcept
{
    switch(m_kind)
    {
  case(kind::boolean):
      return m_data.bool_ti.get_size();
      break;
  case(kind::generic_pointer):
      return m_data.gptr_ti.get_size();
      break;
  case(kind::pointer):
      return m_data.ptr_ti.get_size();
      break;
  case(kind::reference):
      return m_data.ref_ti.get_size();
      break;
  case(kind::integer):
      return m_data.int_ti.get_size();
      break;
  case(kind::unsigned_integer):
      return m_data.uint_ti.get_size();
      break;
  case(kind::fpn):
      return m_data.fpn_ti.get_size();
      break;
  case(kind::array):
      return m_data.arr_ti.get_size();
      break;
  case(kind::enum_):
      return m_data.enu_ti.get_size();
      break;
  case(kind::struct_):
      return m_data.str_ti.get_size();
      break;
  case(kind::union_):
      return m_data.uni_ti.get_size();
      break;
  default:
      return 0;
    }


  return 0;
}


int
type_info::
get_align() const noexcept
{
    switch(m_kind)
    {
  case(kind::array):
      return m_data.arr_ti.get_align();
      break;
  case(kind::struct_):
      return m_data.str_ti.get_align();
      break;
  case(kind::union_):
      return m_data.uni_ti.get_align();
      break;
  default:
      return get_size();
    }


  return 0;
}




void
type_info::
print() const noexcept
{
  print_detail();
}


void
type_info::
print_detail() const noexcept
{
  printf(" id: \"%s\", size: %d, align: %d, ",get_id().data(),get_size(),get_align());
}


namespace type_infos{
const type_info          null{};
const type_info  undefined{undefined_type_info()};
const type_info  null_pointer{null_pointer_type_info()};
const type_info         void_{void_type_info()};
const type_info            s8{         integer_type_info(1)};
const type_info            u8{unsigned_integer_type_info(1)};
const type_info           s16{         integer_type_info(2)};
const type_info           u16{unsigned_integer_type_info(2)};
const type_info           s32{         integer_type_info(4)};
const type_info           u32{unsigned_integer_type_info(4)};
const type_info           s64{         integer_type_info(8)};
const type_info           u64{unsigned_integer_type_info(8)};
const type_info           f32{fpn_type_info(4)};
const type_info           f64{fpn_type_info(8)};
}




}}




