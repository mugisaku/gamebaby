#include"libgbstd/vms/typesystem.hpp"



namespace gbstd{
namespace typesystem{




void
fn_signature::
print() const noexcept
{
  m_return_type_info.print();

  printf("(");

    for(auto&  p: m_parameter_list)
    {
      p.type().print();

      printf("  ");

      gbstd::print(p.name());

      printf(", ");
    }


  printf(")");
}




}}




