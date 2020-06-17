#include"libgbstd/vm.hpp"




namespace gbstd{




void
label_statement::
compile(compile_context&  ctx) const
{
  ctx->add_label("LABEL__%s",m_string.data());
}


void
label_statement::
print() const noexcept
{
  printf("label:%s",m_string.data());
}




}




