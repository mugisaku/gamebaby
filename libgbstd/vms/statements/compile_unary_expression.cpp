#include"libgbstd/vm.hpp"




namespace gbstd{


using namespace typesystem;


namespace{
compile_result
neg(std::string&&  name, const compile_result&  res, const space_node&  nd, compile_context&  ctx)
{
  const char*  tis;

  auto&  ti = res.get_type_info();

    if(ti.is_integer())
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


  ctx.write_local(" %s   %s = sub 0 %s",tis,name.data(),res.get_ir_variable_name().data());

  return compile_result(g_int_ti,std::move(name));
}


compile_result
log_not(std::string&&  name, const compile_result&  res, const space_node&  nd, compile_context&  ctx)
{
  return compile_result(g_int_ti,std::move(name));
}


compile_result
bit_not(std::string&&  name, const compile_result&  res, const space_node&  nd, compile_context&  ctx)
{
  return compile_result(g_int_ti,std::move(name));
}


compile_result
deref(std::string&&  name, const compile_result&  res, const space_node&  nd, compile_context&  ctx)
{
  return compile_result(g_int_ti,std::move(name));
}


compile_result
adr(std::string&&  name, const compile_result&  res, const space_node&  nd, compile_context&  ctx)
{
  return compile_result(g_int_ti,std::move(name));
}
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




}




