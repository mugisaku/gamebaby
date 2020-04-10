#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




std::string
function_signature::
make_id() const noexcept
{
  auto      it = m_container.cbegin();
  auto  end_it = m_container.cend();

  std::string  s;

    if(it != end_it)
    {
      s += (*it++).get_id();
    }


  s += "(";

    while(it != end_it)
    {
      s += (*it++).get_id();

      s += ",";
    }


  s += ")";

  return std::move(s);
}


void
function_signature::
print() const noexcept
{
  printf("(");

    for(auto&  ti: m_container)
    {
      ti.print();

      printf(",");
    }


  printf(")");
}




}}




