#include"libgbstd/vm.hpp"




namespace gbstd{




prefix_unary_operation&
prefix_unary_operation::
assign(const prefix_unary_operation&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_opcode = rhs.m_opcode;

      m_operand = std::make_unique<expression>(*rhs.m_operand);
    }


  return *this;
}


type_info
prefix_unary_operation::
get_type_info(const space_node&  nd) const
{
  auto  ti = m_operand->get_type_info(nd);

    if((m_opcode == "++") ||
       (m_opcode == "--"))
    {
      return ti;
    }

  else
    if(m_opcode == "-")
    {
      auto  t = ti.strip_reference_type();

      return t.is_fpn()? type_infos::f64
            :t.is_integer()? type_infos::i64
            :type_infos::undefined;
    }

  else
    if(m_opcode == "~")
    {
      return type_infos::i64;
    }

  else
    if(m_opcode == "*")
    {
      return ti.form_reference_type(type_infos::pointer_size);
    }

  else
    if(m_opcode == "!")
    {
      return type_infos::i64;
    }

  else
    if(m_opcode == "&")
    {
      return ti.strip_reference_type().form_pointer_type(type_infos::pointer_size);
    }

  else
    if(m_opcode == "sz")
    {
      return type_infos::i64;
    }


  return type_infos::undefined;
}


type_info
postfix_unary_operation::
get_type_info(const space_node&  nd) const
{
  auto  ti = m_operand->get_type_info(nd);

    if((m_opcode == "++") ||
       (m_opcode == "--"))
    {
      return ti;
    }


  return type_infos::undefined;
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


type_info
binary_operation::
get_type_info(const space_node&  nd) const
{
  auto  lti = m_left->get_type_info(nd);
  auto  rti = m_right->get_type_info(nd);

    if(m_opcode == "+")
    {
      auto  l = lti.strip_reference_type();
      auto  r = rti.strip_reference_type();

      return (l.is_fpn() || r.is_fpn())? type_infos::f64
            :(l.is_integer() && r.is_integer())? type_infos::i64
            :(l.is_integer() || r.is_pointer())? r
            :(l.is_pointer() || r.is_integer())? l
            :type_infos::undefined;
    }

  else
    if(m_opcode == "-")
    {
      auto  l = lti.strip_reference_type();
      auto  r = rti.strip_reference_type();

      return (l.is_fpn() || r.is_fpn())? type_infos::f64
            :(l.is_integer() && r.is_integer())? type_infos::i64
            :(l.is_integer() || r.is_pointer())? r
            :(l.is_pointer() || r.is_integer())? l
            :(l.is_pointer() && r.is_pointer())? type_infos::i64
            :type_infos::undefined;
    }

  else
    if((m_opcode == "*") ||
       (m_opcode == "/"))
    {
      auto  l = lti.strip_reference_type();
      auto  r = rti.strip_reference_type();

      return (l.is_fpn() || r.is_fpn())? type_infos::f64
            :                            type_infos::i64;
    }

  else
    if((m_opcode ==  "%") ||
       (m_opcode == "<<") ||
       (m_opcode == ">>") ||
       (m_opcode ==  "|") ||
       (m_opcode ==  "&") ||
       (m_opcode ==  "^"))
    {
      return type_infos::i64;
    }

  else
    if((m_opcode == "==") ||
       (m_opcode == "!=") ||
       (m_opcode ==  "<") ||
       (m_opcode == "<=") ||
       (m_opcode ==  ">") ||
       (m_opcode == ">=") ||
       (m_opcode == "&&") ||
       (m_opcode == "||"))
    {
      return type_infos::boolean;
    }

  else
    if((m_opcode ==   ".") ||
       (m_opcode ==  "->") ||
       (m_opcode ==  ".*") ||
       (m_opcode == "->*"))
    {
    }

  else
    if((m_opcode == "(?)") ||
       (m_opcode == "[?]"))
    {
    }

  else
    if((m_opcode ==   "=") ||
       (m_opcode ==  "+=") ||
       (m_opcode ==  "-=") ||
       (m_opcode ==  "*=") ||
       (m_opcode ==  "/=") ||
       (m_opcode ==  "%=") ||
       (m_opcode == "<<=") ||
       (m_opcode == ">>=") ||
       (m_opcode ==  "|=") ||
       (m_opcode ==  "&=") ||
       (m_opcode ==  "^="))
    {
      return lti;
    }


  return type_infos::undefined;
}


expression&
expression::
assign(const expression&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind          = rhs.m_kind;
      m_begin_address = rhs.m_begin_address;
      m_end_address   = rhs.m_end_address;

        switch(m_kind)
        {
      case(kind::boolean_literal): new(&m_data) bool(rhs.m_data.b);break;
      case(kind::identifier     ): new(&m_data) std::string(rhs.m_data.s);break;
      case(kind::string_literal ): new(&m_data) std::string(rhs.m_data.s);break;
      case(kind::integer_literal): new(&m_data) int64_t(rhs.m_data.i);break;
      case(kind::fpn_literal    ): new(&m_data) double(rhs.m_data.f);break;

      case(kind::prefix_unary ): new(&m_data)  prefix_unary_operation(rhs.m_data.pre_un );break;
      case(kind::postfix_unary): new(&m_data)  postfix_unary_operation(rhs.m_data.pos_un );break;
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

      std::swap(m_kind         ,rhs.m_kind   );
      std::swap(m_begin_address,rhs.m_begin_address);
      std::swap(m_end_address  ,rhs.m_end_address);

        switch(m_kind)
        {
      case(kind::boolean_literal): new(&m_data) bool(rhs.m_data.b);break;
      case(kind::identifier     ): new(&m_data) std::string(std::move(rhs.m_data.s));break;
      case(kind::string_literal ): new(&m_data) std::string(std::move(rhs.m_data.s));break;
      case(kind::integer_literal): new(&m_data) int64_t(std::move(rhs.m_data.i));break;
      case(kind::fpn_literal    ): new(&m_data) double(std::move(rhs.m_data.f));break;

      case(kind::prefix_unary ): new(&m_data)  prefix_unary_operation(std::move(rhs.m_data.pre_un));break;
      case(kind::postfix_unary): new(&m_data)  postfix_unary_operation(std::move(rhs.m_data.pos_un));break;
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
assign(int64_t  i) noexcept
{
  clear();

  new(&m_data) int64_t(i);

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
assign(prefix_unary_operation&&  op) noexcept
{
  clear();

  new(&m_data) prefix_unary_operation(std::move(op));

  m_kind = kind::prefix_unary;

  return *this;
}


expression&
expression::
assign(postfix_unary_operation&&  op) noexcept
{
  clear();

  new(&m_data) postfix_unary_operation(std::move(op));

  m_kind = kind::postfix_unary;

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
  case(kind::identifier     ): std::destroy_at(&m_data.s);break;
  case(kind::string_literal ): std::destroy_at(&m_data.s);break;
  case(kind::integer_literal): std::destroy_at(&m_data.i);break;
  case(kind::fpn_literal    ): std::destroy_at(&m_data.i);break;

  case(kind::prefix_unary ): std::destroy_at(&m_data.pre_un);break;
  case(kind::postfix_unary): std::destroy_at(&m_data.pos_un);break;
  case(kind::binary       ): std::destroy_at(&m_data.bin);break;
    }


  m_kind = kind::null;

  m_begin_address = 0;
  m_end_address   = 0;

  return *this;
}


type_info
expression::
get_type_info(const space_node&  nd) const
{
    switch(m_kind)
    {
  case(kind::null                ): return type_infos::null;break;
  case(kind::null_pointer_literal): return type_infos::null_pointer;break;
  case(kind::undefined_literal   ): return type_infos::undefined;break;
  case(kind::boolean_literal     ): return type_infos::boolean;break;
  case(kind::string_literal      ): return type_infos::i8.form_pointer_type(type_infos::pointer_size);break;
  case(kind::integer_literal     ): return type_infos::i64;break;
  case(kind::identifier          ):
    {
      auto  fn = nd.find_function(m_data.s);

        if(fn)
        {
          return fn->get_signature().get_return_type_info();
        }


      auto  mi = nd.find_memo_info(m_data.s);

        if(mi)
        {
          return mi->get_type_info();
        }


      throw expression_error(form_string("expression::get_type_info error: identifier %s is not found.\n",m_data.s.data()));
    }
    break;

  case(kind::prefix_unary ): return m_data.pre_un.get_type_info(nd);break;
  case(kind::postfix_unary): return m_data.pos_un.get_type_info(nd);break;
  case(kind::binary       ): return m_data.bin.get_type_info(nd);break;
    }


  return type_infos::undefined;
}


address_t
expression::
allocate_address(const space_node&  nd, address_t  end)
{
  auto  ti = get_type_info(nd);

  m_begin_address = get_aligned_address(end              );
  m_end_address   = get_aligned_address(end+ti.get_size());

  end = m_end_address;

    if(is_prefix_unary())
    {
      end = m_data.pre_un.get_operand().allocate_address(nd,end);
    }

  else
    if(is_postfix_unary())
    {
      end = m_data.pos_un.get_operand().allocate_address(nd,end);
    }

  else
    if(is_binary())
    {
      end =  m_data.bin.get_left().allocate_address(nd,end);
      end = m_data.bin.get_right().allocate_address(nd,end);
    }


  return end;
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
  case(kind::integer_literal): printf("%" PRIi64,m_data.i);break;

  case(kind::prefix_unary):
      printf("(");
      printf("%s",m_data.pre_un.get_opcode().get_string());
      m_data.pre_un.get_operand().print();
      printf(")");
      break;
  case(kind::postfix_unary):
      printf("(");
      m_data.pos_un.get_operand().print();
      printf("%s",m_data.pos_un.get_opcode().get_string());
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


  printf("[%d]",m_begin_address);
}




tepid_object
expression::
evaluate(context&  ctx) const noexcept
{
  auto&  mem = ctx.get_memory();

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
/*
      auto  sym = ctx.get_local_symbol_table().find(m_data.s);

        if(sym)
        {
          hot_object  ho(mem,sym->get_type_info().form_reference_type(type_infos::pointer_size),ctx.get_bp()+sym->get_address());

          return ho;
        }


      sym = ctx.get_global_symbol_table().find(m_data.s);

        if(sym)
        {
          hot_object  ho(mem,sym->get_type_info().form_reference_type(type_infos::pointer_size),sym->get_address());

          return ho;
        }
*/


      printf("symbol %s is not found.",m_data.s.data());
    }

  else
    if(is_prefix_unary())
    {
      auto   op = m_data.pre_un.get_opcode();
      auto  obj = m_data.pre_un.get_operand().evaluate(ctx);

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
      auto   op = m_data.pos_un.get_opcode();
      auto  obj = m_data.pos_un.get_operand().evaluate(ctx);

      using namespace operations;

           if(op == "++"){return {postfix_increment(obj),mem};}
      else if(op == "--"){return {postfix_decrement(obj),mem};}
    }

  else
    if(is_binary())
    {
      auto  o = m_data.bin.get_opcode();

      auto&  l = m_data.bin.get_left() ;
      auto&  r = m_data.bin.get_right();

      using namespace operations;

        if(o == "::")
        {
          //not yet implemented.
        }

      else
        if(o == ".")
        {
          return dot(l.evaluate(ctx),r);
        }

      else
        if(o == ".*")
        {
          //not yet implemented.
        }

      else
        if(o == "->")
        {
          return arrow(l.evaluate(ctx),r,mem);
        }

      else
        if(o == "->*")
        {
          //not yet implemented.
        }

      else
        if(o == "(?)")
        {
          return invoke(l.evaluate(ctx),r);
        }
        

      auto  lo = l.evaluate(ctx);
      auto  ro = r.evaluate(ctx);

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




