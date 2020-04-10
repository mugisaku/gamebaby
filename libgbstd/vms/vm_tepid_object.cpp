#include"libgbstd/vm.hpp"




namespace gbstd{




tepid_object::
tepid_object(const hot_object&  ho) noexcept:
//cold_object(ho.get_type_info().form_reference_type(type_infos::pointer_size),ho.get_address()),
m_base_memory(ho.get_memory())
{
}


tepid_object::
tepid_object(const cold_object&  co, const memory&  base_mem) noexcept:
cold_object(co),
m_base_memory(base_mem)
{
}




void
tepid_object::
print() const noexcept
{
    if(get_type_info().is_reference())
    {
      hot_object  ho(*this);

      cold_object  co(ho);

      co.print();
    }

  else
    {
      cold_object::print();
    }
}




}




