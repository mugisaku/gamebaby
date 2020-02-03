#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




const type_info&
type_derivation::
get_array_type(int  n) noexcept
{
    for(auto&  arr: m_array_type_info_list)
    {
        if(arr->get_number_of_elements() == n)
        {
          return *arr;
        }
    }



  auto  ptr = std::make_unique<type_info>(m_base,n);

  m_array_type_info_list.emplace_back(std::move(ptr));

  return *m_array_type_info_list.back();
}


const type_info&
type_derivation::
get_reference_type() noexcept
{
    if(m_base.is_reference())
    {
      return m_base;
    }


    if(!m_reference_type_info)
    {
      m_reference_type_info = std::make_unique<type_info>(m_base,reference_type_info());
    }


   return *m_reference_type_info;
}


const type_info&
type_derivation::
get_pointer_type() noexcept
{
    if(!m_pointer_type_info)
    {
      m_pointer_type_info = std::make_unique<type_info>(m_base,pointer_type_info());
    }


   return *m_pointer_type_info;
}


const type_info&
type_derivation::
get_function_type(parameter_list&&  parals) noexcept
{
    for(auto&  fn: m_function_type_info_list)
    {
        if(fn->get_parameter_list().get_id() == parals.get_id())
        {
          return *fn;
        }
    }



  auto  ptr = std::make_unique<type_info>(m_base,std::move(parals));

  m_function_type_info_list.emplace_back(std::move(ptr));

  return *m_function_type_info_list.back();
}




}}



