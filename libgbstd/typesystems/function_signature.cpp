#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




std::string
function_signature::
make_id() const noexcept
{
  std::string  s = m_parameter_list.make_id();

  s += "->";

  s += m_return_type_info.get_id();

  return std::move(s);
}


void
function_signature::
print() const noexcept
{
}




}}




