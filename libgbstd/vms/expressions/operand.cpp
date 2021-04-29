#include"libgbstd/vms/expression.hpp"




namespace gbstd{




operand&
operand::
assign(const operand&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kinds::boolean   ): create_at(&m_data.b,rhs.m_data.b);break;
      case(kinds::string    ): create_at(&m_data.s,rhs.m_data.s);break;
      case(kinds::integer   ): create_at(&m_data.i,rhs.m_data.i);break;
      case(kinds::floating  ): create_at(&m_data.f,rhs.m_data.f);break;
      case(kinds::identifier): create_at(&m_data.s,rhs.m_data.s);break;
      case(kinds::expression): create_at(&m_data.expr,rhs.m_data.expr);break;
        }


      m_prefix_list  = rhs.m_prefix_list;
      m_postfix_list = rhs.m_postfix_list;
    }


  return *this;
}


operand&
operand::
assign(operand&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kinds::boolean   ): create_at(&m_data.b,std::move(rhs.m_data.b));break;
      case(kinds::string    ): create_at(&m_data.s,std::move(rhs.m_data.s));break;
      case(kinds::integer   ): create_at(&m_data.i,std::move(rhs.m_data.i));break;
      case(kinds::floating  ): create_at(&m_data.f,std::move(rhs.m_data.f));break;
      case(kinds::identifier): create_at(&m_data.s,std::move(rhs.m_data.s));break;
      case(kinds::expression): create_at(&m_data.expr,std::move(rhs.m_data.expr));break;
        }


      std::swap(m_prefix_list ,rhs.m_prefix_list );
      std::swap(m_postfix_list,rhs.m_postfix_list);
    }


  return *this;
}


operand&
operand::
assign(nullptr_t) noexcept
{
  clear();

  m_kind = kinds::null_pointer;

  return *this;
}


operand&
operand::
assign(bool  b) noexcept
{
  clear();

  create_at(&m_data.b,b);

  m_kind = kinds::boolean;

  return *this;
}


operand&
operand::
assign(std::u16string_view  sv) noexcept
{
  clear();

  create_at(&m_data.s,sv);

  m_kind = kinds::string;

  return *this;
}


operand&
operand::
assign(uint64_t  i) noexcept
{
  clear();

  create_at(&m_data.i,i);

  m_kind = kinds::integer;

  return *this;
}


operand&
operand::
assign(double  f) noexcept
{
  clear();

  create_at(&m_data.f,f);

  m_kind = kinds::floating;

  return *this;
}


operand&
operand::
assign(identifier&&  id) noexcept
{
  clear();

  create_at(&m_data.s,id.release());

  m_kind = kinds::identifier;

  return *this;
}


operand&
operand::
assign(expression&&  e) noexcept
{
  clear();

  create_at(&m_data.expr,std::move(e));

  m_kind = kinds::expression;

  return *this;
}


operand&
operand::
clear() noexcept
{
    switch(m_kind)
    {
  case(kinds::null        ): break;
  case(kinds::null_pointer): break;
  case(kinds::boolean     ): break;
  case(kinds::string      ): std::destroy_at(&m_data.s);break;
//  case(kinds::integer     ): std::destroy_at(&m_data.i);break;
//  case(kinds::floating    ): std::destroy_at(&m_data.f);break;
  case(kinds::identifier  ): std::destroy_at(&m_data.s);break;
  case(kinds::expression  ): std::destroy_at(&m_data.expr);break;
    }


  m_kind = kinds::null;

  m_prefix_list.clear();
  m_postfix_list.clear();

  return *this;
}


simple_object
operand::
evaluate() const noexcept
{
  simple_object  o;

    switch(m_kind)
    {
  case(kinds::null        ): break;
  case(kinds::null_pointer): break;
  case(kinds::boolean     ): o.assign(m_data.b);break;
  case(kinds::string      ): break;
  case(kinds::integer     ): o.assign((int64_t)m_data.i);break;
  case(kinds::floating    ): o.assign(m_data.f);break;
  case(kinds::identifier  ): gbstd::print(m_data.s);break;
  case(kinds::expression  ): o.assign(m_data.expr.evaluate());break;
    }


  auto&  v = o[0];

    for(auto  op: m_prefix_list)
    {
        if(op == "*")
        {
        }

      else
        if(op == "&")
        {
        }

      else
        if(op == "!")
        {
               if(v.is_boolean()){v.assign(!v.get_boolean());}
          else if(v.is_integer()){v.assign(!v.get_integer());}
        }

      else
        if(op == "+")
        {
          if(v.is_integer()){}
        }

      else
        if(op == "-")
        {
               if(v.is_integer() ){v.assign(-v.get_integer() );}
          else if(v.is_floating()){v.assign(-v.get_floating());}
        }

      else
        if(op == "~")
        {
               if(v.is_integer() ){v.assign(~v.get_integer() );}
        }
    }


  return std::move(o);
}


void
operand::
print() const noexcept
{
    for(auto&  unop: m_prefix_list)
    {
      printf("%s",unop.data());
    }


    switch(m_kind)
    {
  case(kinds::null        ): break;
  case(kinds::null_pointer): printf("nullptr");break;
  case(kinds::boolean     ): printf("%s",m_data.b? "true":"false");break;
  case(kinds::string      ): printf("\"");gbstd::print(m_data.s);printf("\"");break;
  case(kinds::integer     ): printf("%" PRIu64,m_data.i);break;
  case(kinds::floating    ): printf("%f",m_data.f);break;
  case(kinds::identifier  ): gbstd::print(m_data.s);break;
  case(kinds::expression  ): m_data.expr.print();break;
    }


    for(auto&  post: m_postfix_list)
    {
      post.print();
    }
}




}




