#include"libgbstd/vm.hpp"




namespace gbstd{


using namespace typesystem;


namespace{


type_info
compile_operand(const operand&  o, compile_context&  ctx)
{
    if(o.is_null_pointer())
    {
      ctx.m_asm_context.add_line(asm_opcode::pshi8,0);

      return type_info(null_pointer_type_info());
    }

  else
    if(o.is_boolean())
    {
      ctx.m_asm_context.add_line(asm_opcode::pshi8,static_cast<int64_t>(o.get_boolean()));

      return type_info(boolean_type_info(g_boolean_size));
    }

  else
    if(o.is_integer())
    {
      ctx.m_asm_context.add_line(asm_opcode::pshi64,o.get_integer());

      return type_info(integer_type_info(sizeof(int64_t)));
    }

  else
    if(o.is_fpn())
    {
    }

  else
    if(o.is_string())
    {
      ctx.m_asm_context.add_line(asm_opcode::pshi64,o.get_integer());

      type_info  base(integer_type_info(sizeof(int8_t)));

      return base.form_pointer_type(g_pointer_size);
    }

  else
    if(o.is_identifier())
    {
      auto  fi = ctx.get_node().find_all(o.get_string());

        if(fi.is_type_info())
        {
        }

      else
        if(fi.is_variable_info())
        {
        }

      else
        if(fi.is_function())
        {
        }
    }

  else
    if(o.is_expression())
    {
      return compile(o.get_expression(),ctx);
    }


  return type_info();
}


type_info
compile_selector_in_struct(const struct_type_info&  sti, std::string_view  id, asm_context&  ctx)
{
  auto  m = sti.find(id);

    if(!m)
    {
      throw expression_error(form_string("%s not found",id.data()));
    }


  ctx.add_line(asm_opcode::pshi64,m->get_offset());
  ctx.add_line(asm_opcode::addi,0);

  return m->get_type_info().form_reference_type(g_pointer_size);
}


type_info
compile_selector_in_union(const union_type_info&  uti, std::string_view  id, asm_context&  ctx)
{
  auto  m = uti.find(id);

    if(!m)
    {
      throw expression_error(form_string("%s not found",id.data()));
    }


  return m.form_reference_type(g_pointer_size);
}


void
compile_assignment(const assignment&  ass, type_info  lti, asm_context&  ctx)
{
  auto  op = ass.get_operator_code();

    if(op == operator_code("++"))
    {
    }

  else
    if(op == operator_code(""))
    {
    }
}


type_info
compile_expression(const primary_expression&  e, compile_context&  ctx)
{
  auto  ti = compile_operand(e.get_operand(),ctx);

    for(auto&  e: e.get_elements())
    {
        if(e.is_selector())
        {
            if(ti.is_reference())
            {
              ti = ti.strip_reference_type();

                if(ti.is_struct())
                {
                  ti = compile_selector_in_struct(ti.get_struct_type_info(),e.get_string(),ctx.m_asm_context);
                }

              else
                if(ti.is_union())
                {
                  ti = compile_selector_in_union(ti.get_union_type_info(),e.get_string(),ctx.m_asm_context);
                }
            }
        }

      else
        if(e.is_index())
        {
        }

      else
        if(e.is_call())
        {
        }

      else
        if(e.is_assignment())
        {
          auto&  ass = e.get_assignment();

            if(ti.is_reference())
            {
              compile_assignment(ass,ti.strip_reference_type(),ctx.m_asm_context);
            }
        }
    }


  return std::move(ti);
}


type_info
compile_expression(const unary_expression&  e, compile_context&  ctx)
{
  auto  it     = e.get_operator_code_list().rbegin();
  auto  it_end = e.get_operator_code_list().rend();

  auto  ti = compile_expression(e.get_primary_expression(),ctx);

    while(it != it_end)
    {
      auto&  op = *it++;

           if(op == operator_code("-")){;}
      else if(op == operator_code("!")){;}
      else if(op == operator_code("~")){;}
      else if(op == operator_code("*")){;}
      else if(op == operator_code("&")){;}
    }


  return std::move(ti);
}
}


type_info
compile(const expression&  e, compile_context&  ctx)
{
  auto&  ls = e.get_elements();

  auto    p = ls.data();
  auto  end = ls.data()+ls.size();

  type_info  lti;

    if(p != end)
    {
      lti = compile_expression(p++->get_expression(),ctx);

        while(p != end)
        {
          auto  op = p->get_operator_code();

          auto  rti = compile_expression(p++->get_expression(),ctx);

               if(op == operator_code("+")  ){;}
          else if(op == operator_code("-")  ){;}
          else if(op == operator_code("*")  ){;}
          else if(op == operator_code("/")  ){;}
          else if(op == operator_code("%")  ){;}
          else if(op == operator_code("<<") ){;}
          else if(op == operator_code(">>") ){;}
          else if(op == operator_code("|")  ){;}
          else if(op == operator_code("&")  ){;}
          else if(op == operator_code("^")  ){;}
          else if(op == operator_code("==") ){;}
          else if(op == operator_code("!=") ){;}
          else if(op == operator_code("<")  ){;}
          else if(op == operator_code("<=") ){;}
          else if(op == operator_code(">")  ){;}
          else if(op == operator_code(">=") ){;}
          else if(op == operator_code("||") ){;}
          else if(op == operator_code("&&") ){;}
          else if(op == operator_code("=")  ){;}
          else if(op == operator_code("+=") ){;}
          else if(op == operator_code("-=") ){;}
          else if(op == operator_code("*=") ){;}
          else if(op == operator_code("/=") ){;}
          else if(op == operator_code("%=") ){;}
          else if(op == operator_code("|=") ){;}
          else if(op == operator_code("&=") ){;}
          else if(op == operator_code("^=") ){;}
          else if(op == operator_code("<<=")){;}
          else if(op == operator_code(">>=")){;}
          else if(op == operator_code(",")  ){;}
        }
    }


  return std::move(lti);
}




}




