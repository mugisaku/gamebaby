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

  m_holder->m_reference_type_info.reset();
  m_holder->m_pointer_type_info.reset();
  m_holder->m_array_type_info_list.clear();
  m_holder->m_function_type_info_list.clear();

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




const type_info&
type_info::
form_function_type(parameter_list&&  parals) const noexcept
{
  auto  id = parals.make_id();

    for(auto&  fn: m_holder->m_function_type_info_list)
    {
        if(fn.get_function_signature().get_parameter_list().make_id() == id)
        {
          return fn;
        }
    }



  m_holder->m_function_type_info_list.emplace_back(function_signature(*this,std::move(parals)));

  return m_holder->m_function_type_info_list.back();
}


const type_info&
type_info::
form_array_type(int  n) const noexcept
{
    for(auto&  arr: m_holder->m_array_type_info_list)
    {
        if(arr.get_array_type_info().get_number_of_elements() == n)
        {
          return arr;
        }
    }


  m_holder->m_array_type_info_list.emplace_back(array_type_info(*this,n));

  return m_holder->m_array_type_info_list.back();
}


const type_info&
type_info::
form_reference_type(int  w) const noexcept
{
    if(is_reference())
    {
      return *this;
    }


    if(!m_holder->m_reference_type_info)
    {
      m_holder->m_reference_type_info = std::make_unique<type_info>(reference_type_info(*this,w));
    }


   return *m_holder->m_reference_type_info;
}


const type_info&
type_info::
form_pointer_type(int  w) const noexcept
{
    if(!m_holder->m_pointer_type_info)
    {
      m_holder->m_pointer_type_info = std::make_unique<type_info>(pointer_type_info(*this,w));
    }


   return *m_holder->m_pointer_type_info;
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




}}




