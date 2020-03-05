#include"libgbstd/vm.hpp"




namespace gbstd{




tepid_object::
tepid_object(hot_object&&  ho) noexcept:
cold_object(ho.get_type_info(),ho.get_address()),
m_base_memory(ho.get_memory())
{
}


tepid_object::
tepid_object(cold_object&&  co, const memory&  base_mem) noexcept:
cold_object(std::move(co)),
m_base_memory(base_mem)
{
}




}




