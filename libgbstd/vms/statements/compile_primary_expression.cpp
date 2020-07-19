#include"libgbstd/vm.hpp"




namespace gbstd{


using namespace typesystem;


namespace{


compile_result
compile_operand(const operand&  o, const space_node&  nd, compile_context&  ctx)
{
  auto  name = ctx.create_variable_name();

    if(o.is_null_pointer())
    {
      ctx.write_local("  int  %s = add 0 0",name.data());

      return compile_result(g_nullptr_ti,std::move(name));
    }

  else
    if(o.is_boolean())
    {
      ctx.write_local("  int  %s = add 0 %d",name.data(),o.get_boolean()? 1:0);

      return compile_result(g_bool_ti,std::move(name));
    }

  else
    if(o.is_integer())
    {
      ctx.write_local("  int  %s = add 0 %d",name.data(),o.get_integer());

      return compile_result(g_int_ti,std::move(name));
    }

  else
    if(o.is_fpn())
    {
      ctx.write_local("  float  %s = add 0 %f",name.data(),o.get_fpn());

      return compile_result(g_float_ti,std::move(name));
    }

  else
    if(o.is_string())
    {
//      ctx.write_local("  int  %s = add 0 %d",name.data(),o.get_integer());

//      return compile_result(*ti,std::move(name));
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


  throw compile_error(0,"");
}


compile_result
compile_selector_in_struct(const struct_type_def&  def, std::string_view  id, compile_context&  ctx)
{
  auto  name = ctx.create_variable_name();

  auto  m = def.find(id);

    if(!m)
    {
      throw expression_error(form_string("%s not found",id.data()));
    }



  return compile_result(m->get_type_info().form_reference(g_word_size),std::move(name));
}


compile_result
compile_selector_in_union(const union_type_def&  def, std::string_view  id, compile_context&  ctx)
{
  auto  name = ctx.create_variable_name();

  auto  m = def.find(id);

    if(!m)
    {
      throw expression_error(form_string("%s not found",id.data()));
    }


  return compile_result(m.form_reference(g_word_size),std::move(name));
}


void
compile_assignment(const assignment&  ass, type_info&&  lti, compile_context&  ctx)
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
}




compile_result
compile_expression(const primary_expression&  e, const space_node&  nd, compile_context&  ctx)
{
  auto  res = compile_operand(e.get_operand(),nd,ctx);

/*
    for(auto&  e: e.get_elements())
    {
        if(e.is_selector())
        {
            if(ti.is_reference())
            {
              ti = ti.strip_reference_type();

                if(ti.is_struct())
                {
                  res = compile_selector_in_struct(ti.get_struct_type_info(),e.get_string(),ctx);
                }

              else
                if(ti.is_union())
                {
                  res = compile_selector_in_union(ti.get_union_type_info(),e.get_string(),ctx);
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
*/


  return std::move(res);
}




}




