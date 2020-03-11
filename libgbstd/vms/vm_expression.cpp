#include"libgbstd/vm.hpp"




namespace gbstd{




unary_operation&
unary_operation::
assign(const unary_operation&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_opcode = rhs.m_opcode;

      m_operand = std::make_unique<expression>(*rhs.m_operand);
    }


  return *this;
}


binary_operation&
binary_operation::
assign(const binary_operation&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_opcode = rhs.m_opcode;

      m_left  = std::make_unique<expression>(*rhs.m_left );
      m_right = std::make_unique<expression>(*rhs.m_right);
    }


  return *this;
}




expression&
expression::
assign(const expression&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::boolean_literal): new(&m_data) bool(rhs.m_data.b);break;
      case(kind::identifier     ): new(&m_data) std::string(rhs.m_data.s);break;
      case(kind::string_literal ): new(&m_data) std::string(rhs.m_data.s);break;
      case(kind::integer_literal): new(&m_data) uint64_t(rhs.m_data.i);break;
      case(kind::fpn_literal    ): new(&m_data) double(rhs.m_data.f);break;

      case(kind::prefix_unary ):
      case(kind::postfix_unary): new(&m_data)  unary_operation(rhs.m_data.un );break;
      case(kind::binary       ): new(&m_data) binary_operation(rhs.m_data.bin);break;
        }
    }


  return *this;
}


expression&
expression::
assign(expression&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::boolean_literal): new(&m_data) bool(rhs.m_data.b);break;
      case(kind::identifier     ): new(&m_data) std::string(std::move(rhs.m_data.s));break;
      case(kind::string_literal ): new(&m_data) std::string(std::move(rhs.m_data.s));break;
      case(kind::integer_literal): new(&m_data) uint64_t(std::move(rhs.m_data.i));break;
      case(kind::fpn_literal    ): new(&m_data) double(std::move(rhs.m_data.f));break;

      case(kind::prefix_unary ):
      case(kind::postfix_unary): new(&m_data)  unary_operation(std::move(rhs.m_data.un ));break;
      case(kind::binary       ): new(&m_data) binary_operation(std::move(rhs.m_data.bin));break;
        }
    }


  return *this;
}


expression&
expression::
assign(undefined  u) noexcept
{
  clear();

  m_kind = kind::undefined_literal;

  return *this;
}


expression&
expression::
assign(nullptr_t  n) noexcept
{
  clear();

  m_kind = kind::null_pointer_literal;

  return *this;
}


expression&
expression::
assign(bool  b) noexcept
{
  clear();

  new(&m_data) bool(b);

  m_kind = kind::boolean_literal;

  return *this;
}


expression&
expression::
assign(identifier&&  id) noexcept
{
  clear();

  new(&m_data) std::string(id.release());

  m_kind = kind::identifier;

  return *this;
}


expression&
expression::
assign(std::string_view  sv) noexcept
{
  clear();

  new(&m_data) std::string(sv);

  m_kind = kind::string_literal;

  return *this;
}


expression&
expression::
assign(uint64_t  i) noexcept
{
  clear();

  new(&m_data) uint64_t(i);

  m_kind = kind::integer_literal;

  return *this;
}


expression&
expression::
assign(double  f) noexcept
{
  clear();

  new(&m_data) double(f);

  m_kind = kind::fpn_literal;

  return *this;
}


expression&
expression::
assign(int  k, unary_operation&&  op) noexcept
{
  clear();

  new(&m_data) unary_operation(std::move(op));

  m_kind = (k == 'p')? kind::prefix_unary
          :            kind::postfix_unary
          ;

  return *this;
}


expression&
expression::
assign(binary_operation&&  op) noexcept
{
  clear();

  new(&m_data) binary_operation(std::move(op));

  m_kind = kind::binary;

  return *this;
}




expression&
expression::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::undefined_literal): break;
  case(kind::null_pointer_literal): break;
  case(kind::boolean_literal): break;
  case(kind::identifier     ): m_data.s.~basic_string();break;
  case(kind::string_literal ): m_data.s.~basic_string();break;
  case(kind::integer_literal): /*m_data.i.~int()*/;break;
  case(kind::fpn_literal): /*m_data.i.~int()*/;break;

  case(kind::prefix_unary ):
  case(kind::postfix_unary): m_data.un.~unary_operation();break;
  case(kind::binary       ): m_data.bin.~binary_operation();break;
    }


  m_kind = kind::null;

  return *this;
}


void
expression::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::null): break;
  case(kind::null_pointer_literal): printf("nullptr");break;
  case(kind::undefined_literal): printf("undefined");break;
  case(kind::boolean_literal): printf("%s",m_data.b? "true":"false");break;
  case(kind::string_literal ): printf("\"%s\"",m_data.s.data());break;
  case(kind::identifier     ): printf("%s",m_data.s.data());break;
  case(kind::integer_literal): printf("%" PRIu64,m_data.i);break;

  case(kind::prefix_unary):
      printf("(");
      printf("%s",m_data.un.get_opcode().get_string());
      m_data.un.get_operand().print();
      printf(")");
      break;
  case(kind::postfix_unary):
      printf("(");
      m_data.un.get_operand().print();
      printf("%s",m_data.un.get_opcode().get_string());
      printf(")");
      break;
  case(kind::binary):
      printf("(");
      m_data.bin.get_left().print();
      printf("%s",m_data.bin.get_opcode().get_string());
      m_data.bin.get_right().print();
      printf(")");
      break;
    }
}




expression
make_expression(token_iterator&  it) noexcept
{
  exprrpn  rpn(it);

  std::vector<std::vector<expression>>  table(1);

  std::vector<operator_code>  close_stack;

    for(auto&  e: rpn.get_stack())
    {
      auto&  buf = table.back();

        if(e.is_unsigned_integer())
        {
          buf.emplace_back(e.get_unsigned_integer());
        }

      else
        if(e.is_real_number())
        {
          buf.emplace_back(e.get_real_number());
        }

      else
        if(e.is_identifier())
        {
          auto  s = e.get_string();

            if(s == std::string_view("undefined"))
            {
              buf.emplace_back(undefined());
            }

          else
            if(s == std::string_view("nullptr"))
            {
              buf.emplace_back(nullptr);
            }

          else
            if(s == std::string_view("true"))
            {
              buf.emplace_back(true);
            }

          else
            if(s == std::string_view("false"))
            {
              buf.emplace_back(false);
            }

          else
            {
              identifier  id(s);

              buf.emplace_back(std::move(id));
            }
        }

      else
        if(e.is_string())
        {
          buf.emplace_back(e.get_string());
        }

      else
        if(e.is_prefix_unary_operator())
        {
            if(buf.size() < 1)
            {
              report;

              return {};
            }


          auto  op = e.get_operator_code();
          auto  o  = std::make_unique<expression>(std::move(buf.back()));

          buf.back().assign('p',unary_operation(op,std::move(o)));
        }

      else
        if(e.is_postfix_unary_operator())
        {
            if(buf.size() < 1)
            {
              report;

              return {};
            }


          auto  op = e.get_operator_code();
          auto  o  = std::make_unique<expression>(std::move(buf.back()));

          buf.back().assign('P',unary_operation(op,std::move(o)));
        }

      else
        if(e.is_binary_operator())
        {
            if(buf.size() < 2)
            {
              report;

              return {};
            }


          auto  r = std::make_unique<expression>(std::move(buf.back()));

          buf.pop_back();

          auto  l = std::make_unique<expression>(std::move(buf.back()));

          auto  op = e.get_operator_code();

          buf.back() = binary_operation(op,std::move(l),std::move(r));
        }

      else
        if(e.is_operator())
        {
          auto  op = e.get_operator_code();

            if(op == "(")
            {
              close_stack.emplace_back(")");

              table.emplace_back();
            }

          else
            if(op == "[")
            {
              close_stack.emplace_back("]");

              table.emplace_back();
            }

          else
            if(op == "{")
            {
              close_stack.emplace_back("}");

              table.emplace_back();
            }

          else
            if(op == close_stack.back())
            {
              close_stack.pop_back();

              expression  t;

                if(buf.size())
                {
                    if(buf.size() > 1)
                    {
                      report;
                    }


                  t = std::move(buf.back());
                }


              table.pop_back();

                if(table.back().size() < 1)
                {
                  table.back().emplace_back(std::move(t));
                }

              else
                {
                  operator_code  opop( (op == ")")? "(?)"
                                      :(op == "]")? "[?]"
                                      :(op == "}")? "{?}"
                                      :"")
                                      ;

                  auto  l = std::make_unique<expression>(std::move(table.back().back()));
                  auto  r = std::make_unique<expression>(std::move(t                  ));
     
                  table.back().back().assign(binary_operation(opop,std::move(l),std::move(r)));
                }
            }

          else
            {
              report;
            }
        }

      else
        {
          report;
        }
    }


    if(table.size() == 1)
    {
      auto&  buf = table.back();

        if(buf.size() == 1)
        {
          return std::move(buf.back());
        }
    }


  report;

  return expression();
}


expression
make_expression(std::string_view  sv) noexcept
{
  token_block  blk(sv);

  token_iterator  it(blk);

  return make_expression(it);
}




tepid_object
expression::
evaluate(const symbol_table&  symtbl, memory&  mem) const noexcept
{
    if(is_integer_literal())
    {
      return {m_data.i,mem};
    }

  else
    if(is_boolean_literal())
    {
      return {m_data.b,mem};
    }

  else
    if(is_null_pointer_literal())
    {
      return {nullptr,mem};
    }

  else
    if(is_fpn_literal())
    {
      return {m_data.f,mem};
    }

  else
    if(is_string_literal())
    {
      return {cold_object(m_data.s),mem};
    }

  else
    if(is_identifier())
    {
      return symtbl.make_object(m_data.s,mem);
    }

  else
    if(is_prefix_unary())
    {
      auto   op = m_data.un.get_opcode();
      auto  obj = m_data.un.get_operand().evaluate(symtbl,mem);

      using namespace operations;

           if(op == "++"){return operations::assign(obj,add(obj,1));}
      else if(op == "--"){return operations::assign(obj,sub(obj,1));}
      else if(op == "*" ){return dereference(obj,mem);}
      else if(op == "!" ){return {logical_not(obj),mem};}
      else if(op == "~" ){return {    bit_not(obj),mem};}
      else if(op == "-" ){return {        neg(obj),mem};}
      else if(op == "&" ){return {    address(obj),mem};}
      else if(op == "sz"){return {       size(obj),mem};}
    }

  else
    if(is_postfix_unary())
    {
      auto   op = m_data.un.get_opcode();
      auto  obj = m_data.un.get_operand().evaluate(symtbl,mem);

      using namespace operations;

           if(op == "++"){return {postfix_increment(obj),mem};}
      else if(op == "--"){return {postfix_decrement(obj),mem};}
    }

  else
    if(is_binary())
    {
      auto  o = m_data.bin.get_opcode();

      auto  l = m_data.bin.get_left() ;
      auto  r = m_data.bin.get_right();

      using namespace operations;

        if(o == "::")
        {
          //not yet implemented.
        }

      else
        if(o == ".")
        {
          return dot(l.evaluate(symtbl,mem),r);
        }

      else
        if(o == ".*")
        {
          //not yet implemented.
        }

      else
        if(o == "->")
        {
          return arrow(l.evaluate(symtbl,mem),r,mem);
        }

      else
        if(o == "->*")
        {
          //not yet implemented.
        }

      else
        if(o == "(?)")
        {
          return invoke(l.evaluate(symtbl,mem),r);
        }
        

      auto  lo = l.evaluate(symtbl,mem);
      auto  ro = r.evaluate(symtbl,mem);

           if(o == "+"  ){return {    add(lo,ro),mem};}
      else if(o == "-"  ){return {    sub(lo,ro),mem};}
      else if(o == "*"  ){return {    mul(lo,ro),mem};}
      else if(o == "/"  ){return {    div(lo,ro),mem};}
      else if(o == "%"  ){return {    rem(lo,ro),mem};}
      else if(o == "<<" ){return {    shl(lo,ro),mem};}
      else if(o == ">>" ){return {    shr(lo,ro),mem};}
      else if(o == "|"  ){return { bit_or(lo,ro),mem};}
      else if(o == "&"  ){return {bit_and(lo,ro),mem};}
      else if(o == "^"  ){return {bit_xor(lo,ro),mem};}
      else if(o == "+=" ){return operations::assign(lo,    add(lo,ro));}
      else if(o == "-=" ){return operations::assign(lo,    sub(lo,ro));}
      else if(o == "*=" ){return operations::assign(lo,    mul(lo,ro));}
      else if(o == "/=" ){return operations::assign(lo,    div(lo,ro));}
      else if(o == "%=" ){return operations::assign(lo,    rem(lo,ro));}
      else if(o == "<<="){return operations::assign(lo,    shl(lo,ro));}
      else if(o == ">>="){return operations::assign(lo,    shr(lo,ro));}
      else if(o == "|=" ){return operations::assign(lo, bit_or(lo,ro));}
      else if(o == "&=" ){return operations::assign(lo,bit_and(lo,ro));}
      else if(o == "^=" ){return operations::assign(lo,bit_xor(lo,ro));}
      else if(o == "="  ){return operations::assign(          lo,ro);}
      else if(o == "==" ){return {         eq(lo,ro),mem};}
      else if(o == "==="){return {         eq(lo,ro),mem};}
      else if(o == "!=" ){return {        neq(lo,ro),mem};}
      else if(o == "!=="){return {        neq(lo,ro),mem};}
      else if(o == "<"  ){return {         lt(lo,ro),mem};}
      else if(o == "<=" ){return {       lteq(lo,ro),mem};}
      else if(o == ">"  ){return {         gt(lo,ro),mem};}
      else if(o == ">=" ){return {       gteq(lo,ro),mem};}
      else if(o == "||" ){return { logical_or(lo,ro),mem};}
      else if(o == "&&" ){return {logical_and(lo,ro),mem};}
      else if(o == ","  ){return lo;}
      else if(o == "[?]"){return subscript(lo,ro,mem);}
    }


  return tepid_object();
}


std::vector<const expression*>
expression::
get_argument_source_list() const noexcept
{
  std::vector<const expression*>  buf;

  const expression*  e = this;

    while(*e)
    {
        if(e->is_binary())
        {
          auto&  bin = e->m_data.bin;

          auto  o = bin.get_opcode();

            if(o == ",")
            {
              auto&  l = bin.get_left() ;
              auto&  r = bin.get_right();

              buf.emplace_back(&r);

              e = &l;

              continue;
            }
        }


      buf.emplace_back(e);

      break;
    }


  return std::move(buf);
}




}




