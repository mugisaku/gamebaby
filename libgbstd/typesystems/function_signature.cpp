#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




std::string
function_signature::
make_name() const noexcept
{
  std::string  s(m_return_type_info.get_name());

  s += "(";

    for(auto&  p: m_parameter_list)
    {
      s += p.get_type_info().get_name();

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




