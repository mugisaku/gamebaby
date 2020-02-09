#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




std::string
parameter_list::
make_id() const noexcept
{
  std::string  s = "(";

    for(auto&  para: m_container)
    {
      s += para->get_id();

      s += ",";
    }


  s += ")";

  return std::move(s);
}


void
parameter_list::
print() const noexcept
{
  printf("(");

    for(auto&  para: m_container)
    {
      para->print();

      printf(",");
    }


  printf(")");
}




}}




