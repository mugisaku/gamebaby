#include"libgbstd/vm.hpp"




namespace gbstd{


using namespace typesystem;


namespace{


type_info
compile_operand(const operand&  o, const space_node&  nd, compile_context&  ctx)
{
    if(o.is_null_pointer())
    {
      return type_info(null_pointer_type_info());
    }

  else
    if(o.is_boolean())
    {
      return type_info(boolean_type_info(g_boolean_size));
    }

  else
    if(o.is_integer())
    {
      return type_info(integer_type_info(sizeof(int64_t)));
    }

  else
    if(o.is_fpn())
    {
    }

  else
    if(o.is_string())
    {
      type_info  base(integer_type_info(sizeof(int8_t)));

      return base.form_pointer_type(g_pointer_size);
    }

  else
    if(o.is_identifier())
    {
      auto  fi = nd.find_all(o.get_string());

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
      return compile_expression(o.get_expression(),nd,ctx);
    }


  return type_info();
}


type_info
compile_selector_in_struct(const struct_type_info&  sti, std::string_view  id, compile_context&  ctx)
{
  auto  m = sti.find(id);

    if(!m)
    {
      throw expression_error(form_string("%s not found",id.data()));
    }



  return m->get_type_info().form_reference_type(g_pointer_size);
}


type_info
compile_selector_in_union(const union_type_info&  uti, std::string_view  id, compile_context&  ctx)
{
  auto  m = uti.find(id);

    if(!m)
    {
      throw expression_error(form_string("%s not found",id.data()));
    }


  return m.form_reference_type(g_pointer_size);
}


void
compile_assignment(const assignment&  ass, type_info  lti, compile_context&  ctx)
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
compile_expression(const primary_expression&  e, const space_node&  nd, compile_context&  ctx)
{
  auto  ti = compile_operand(e.get_operand(),nd,ctx);

    for(auto&  e: e.get_elements())
    {
        if(e.is_selector())
        {
            if(ti.is_reference())
            {
              ti = ti.strip_reference_type();

                if(ti.is_struct())
                {
                  ti = compile_selector_in_struct(ti.get_struct_type_info(),e.get_string(),ctx);
                }

              else
                if(ti.is_union())
                {
                  ti = compile_selector_in_union(ti.get_union_type_info(),e.get_string(),ctx);
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
              compile_assignment(ass,ti.strip_reference_type(),ctx);
            }
        }
    }


  return std::move(ti);
}


type_info
compile_expression(const unary_expression&  e, const space_node&  nd, compile_context&  ctx)
{
  auto  it     = e.get_operator_code_list().rbegin();
  auto  it_end = e.get_operator_code_list().rend();

  auto  ti = compile_expression(e.get_primary_expression(),nd,ctx);

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
compile_expression(const expression&  e, const space_node&  nd, compile_context&  ctx)
{
  auto&  ls = e.get_elements();

  auto    p = ls.data();
  auto  end = ls.data()+ls.size();

  type_info  lti;

    if(p != end)
    {
      lti = compile_expression(p++->get_expression(),nd,ctx);

        while(p != end)
        {
          auto  op = p->get_operator_code();

          auto  rti = compile_expression(p++->get_expression(),nd,ctx);

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




