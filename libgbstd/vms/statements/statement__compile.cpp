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
      auto  ti = nd.find_type_info("nullptr");

      ctx.write_local("  int  %s = add 0 0",name.data());

      return compile_result(*ti,std::move(name));
    }

  else
    if(o.is_boolean())
    {
      auto  ti = nd.find_type_info("bool");

      ctx.write_local("  int  %s = add 0 %d",name.data(),o.get_boolean()? 1:0);

      return compile_result(*ti,std::move(name));
    }

  else
    if(o.is_integer())
    {
      auto  ti = nd.find_type_info("int");

      ctx.write_local("  int  %s = add 0 %d",name.data(),o.get_integer());

      return compile_result(*ti,std::move(name));
    }

  else
    if(o.is_fpn())
    {
      auto  ti = nd.find_type_info("float");

      ctx.write_local("  float  %s = add 0 %f",name.data(),o.get_fpn());

      return compile_result(*ti,std::move(name));
    }

  else
    if(o.is_string())
    {
      auto  ti = nd.find_type_info("int8");

//      ctx.write_local("  int  %s = add 0 %d",name.data(),o.get_integer());

      return compile_result(*ti,std::move(name));
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



  return compile_result(m->get_type_info().form_reference_type(g_pointer_size),std::move(name));
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


  return compile_result(m.form_reference_type(g_pointer_size),std::move(name));
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


compile_result
neg(std::string&&  name, const compile_result&  res, const space_node&  nd, compile_context&  ctx)
{
  const char*  tis;

  auto&  ti = res.get_type_info();

    if(ti.is_pointer())
    {
      tis = "int";
    }

  else
    if(ti.is_fpn())
    {
      tis = "float";
    }

  else
    {
      throw compile_error(0,"type is mismatched");
    }


  ctx.write_local(" %s   %s = add %s",tis,name.data(),res.get_ir_variable_name().data());

  return compile_result(*nd.find_type_info_by_name("int"),std::move(name));
}


compile_result
log_not(std::string&&  name, const compile_result&  res, const space_node&  nd, compile_context&  ctx)
{
  return compile_result(*nd.find_type_info_by_name("int"),std::move(name));
}


compile_result
bit_not(std::string&&  name, const compile_result&  res, const space_node&  nd, compile_context&  ctx)
{
  return compile_result(*nd.find_type_info_by_name("int"),std::move(name));
}


compile_result
deref(std::string&&  name, const compile_result&  res, const space_node&  nd, compile_context&  ctx)
{
  return compile_result(*nd.find_type_info_by_name("int"),std::move(name));
}


compile_result
adr(std::string&&  name, const compile_result&  res, const space_node&  nd, compile_context&  ctx)
{
  return compile_result(*nd.find_type_info_by_name("int"),std::move(name));
}


compile_result
compile_expression(const unary_expression&  e, const space_node&  nd, compile_context&  ctx)
{
  auto  it     = e.get_operator_code_list().rbegin();
  auto  it_end = e.get_operator_code_list().rend();

  auto  res = compile_expression(e.get_primary_expression(),nd,ctx);

    while(it != it_end)
    {
      auto&  op = *it++;

      auto  name = ctx.create_variable_name();

           if(op == operator_code("-")){res =     neg(std::move(name),res,nd,ctx);}
      else if(op == operator_code("!")){res = log_not(std::move(name),res,nd,ctx);}
      else if(op == operator_code("~")){res = bit_not(std::move(name),res,nd,ctx);}
      else if(op == operator_code("*")){res =   deref(std::move(name),res,nd,ctx);}
      else if(op == operator_code("&")){res =     adr(std::move(name),res,nd,ctx);}
    }


  return std::move(res);
}
compile_result
add(std::string&&  name, const compile_result&  l, const compile_result&  r, const space_node&  nd, compile_context&  ctx)
{
  const char*  tis;

  auto&  lti = l.get_type_info();
  auto&  rti = r.get_type_info();

    if((lti.is_pointer() && rti.is_integer()) ||
       (lti.is_integer() && rti.is_integer()))
    {
      tis = "int";
    }

  else
    if((lti.is_fpn()     && rti.is_integer()) ||
       (lti.is_integer() && rti.is_fpn()    ) ||
       (lti.is_fpn()     && rti.is_fpn()    ))
    {
      tis = "float";
    }

  else
    {
      throw compile_error(0,"type is mismatched");
    }


  ctx.write_local(" %s   %s = add %s  %s",tis,name.data(),l.get_ir_variable_name().data(),r.get_ir_variable_name().data());

  return compile_result(*nd.find_type_info_by_name("int"),std::move(name));
}
compile_result
cmp(std::string&&  name, const compile_result&  l, const compile_result&  r, const space_node&  nd, compile_context&  ctx)
{
  const char*  tis;

  auto&  lti = l.get_type_info();
  auto&  rti = r.get_type_info();

    if((lti.is_pointer() && rti.is_integer()) ||
       (lti.is_integer() && rti.is_integer()))
    {
      tis = "int";
    }

  else
    if((lti.is_fpn()     && rti.is_integer()) ||
       (lti.is_integer() && rti.is_fpn()    ) ||
       (lti.is_fpn()     && rti.is_fpn()    ))
    {
      tis = "float";
    }

  else
    {
      throw compile_error(0,"type is mismatched");
    }


  ctx.write_local(" %s   %s = add %s  %s",tis,name.data(),l.get_ir_variable_name().data(),r.get_ir_variable_name().data());

  return compile_result(*nd.find_type_info_by_name("int"),std::move(name));
}
}


compile_result
compile_expression(const expression&  e, const space_node&  nd, compile_context&  ctx)
{
  auto&  ls = e.get_elements();

  auto    p = ls.data();
  auto  end = ls.data()+ls.size();

    if(p != end)
    {
      auto  l = compile_expression(p++->get_expression(),nd,ctx);

        while(p != end)
        {
          auto  op = p->get_operator_code();

          auto  r = compile_expression(p++->get_expression(),nd,ctx);

          auto  name = ctx.create_variable_name();

               if(op == operator_code("+")  ){l = add(std::move(name),l,r,nd,ctx);}
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


      return std::move(l);
    }


  return compile_result();
}




}




