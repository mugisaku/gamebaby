#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




std::string
function_signature::
make_id() const noexcept
{
  std::string  s(m_return_type_info.get_id());

  s += "(";

    for(auto&  p: m_parameter_list)
    {
      s += p.get_type_info().get_id();

      s += ",";
    }


  s += ")";


  return std::move(s);
}


void
function_signature::
print() const noexcept
{
  m_return_type_info.print();

  printf("(");

    for(auto&  p: m_parameter_list)
    {
      p.get_type_info().print();

      printf("  %s, ",p.get_name().data());
    }


  printf(")");
}




}}




