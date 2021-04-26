#include"libgbstd/vms/expression.hpp"




namespace gbstd{




postfix_element&
postfix_element::
assign(const postfix_element&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kinds::selector  ): create_at(&m_data.s     ,rhs.m_data.s     );break;
      case(kinds::index     ): create_at(&m_data.expr  ,rhs.m_data.expr  );break;
      case(kinds::call      ): create_at(&m_data.exprls,rhs.m_data.exprls);break;
        }
    }


  return *this;
}


postfix_element&
postfix_element::
assign(postfix_element&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kinds::selector  ): create_at(&m_data.s     ,std::move(rhs.m_data.s     ));break;
      case(kinds::index     ): create_at(&m_data.expr  ,std::move(rhs.m_data.expr  ));break;
      case(kinds::call      ): create_at(&m_data.exprls,std::move(rhs.m_data.exprls));break;
        }
    }


  return *this;
}


postfix_element&
postfix_element::
assign(std::u16string_view  id) noexcept
{
  clear();

  create_at(&m_data.s,id);

  m_kind = kinds::selector;

  return *this;
}


postfix_element&
postfix_element::
assign(expression&&  e) noexcept
{
  clear();

  create_at(&m_data.expr,std::move(e));

  m_kind = kinds::index;

  return *this;
}


postfix_element&
postfix_element::
assign(std::vector<expression>&&  ls) noexcept
{
  clear();

  create_at(&m_data.exprls,std::move(ls));

  m_kind = kinds::call;

  return *this;
}


postfix_element&
postfix_element::
clear() noexcept
{
    switch(m_kind)
    {
  case(kinds::null    ): break;
  case(kinds::selector): std::destroy_at(&m_data.s     );break;
  case(kinds::index   ): std::destroy_at(&m_data.expr  );break;
  case(kinds::call    ): std::destroy_at(&m_data.exprls);break;
    }


  m_kind = kinds::null;

  return *this;
}


void
postfix_element::
print() const noexcept
{
    switch(m_kind)
    {
  case(kinds::null    ): break;
  case(kinds::selector): printf(".");  gbstd::print(m_data.s);break;
  case(kinds::index   ): printf("[");  m_data.expr.print();  printf("]");break;
  case(kinds::call    ):
      printf("(");

        for(auto&  e: m_data.exprls)
        {
          e.print();

          printf(",");
        }


      printf(")");
      break;
    }
}




}




