#include"libgbstd/vm.hpp"




namespace gbstd{


using namespace typesystem;


namespace{


compile_result
add(std::string&&  name, const compile_result&  l, const compile_result&  r, const space_node&  nd, compile_context&  ctx)
{
  const char*  tis;

  auto&  lti = l.get_type_info();
  auto&  rti = r.get_type_info();

    if(lti.is_pointer())
    {
      tis = "int";
    }

  else
    if(lti.is_fpn())
    {
      tis = "float";
    }

  else
    {
      throw compile_error(0,"type is mismatched");
    }


  ctx.write_local(" %s   %s = add %s",tis,name.data(),l.get_ir_variable_name().data(),
                                                      r.get_ir_variable_name().data());

  return compile_result(g_int_ti,std::move(name));
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


const type_info      g_int_ti("int64");
const type_info    g_float_ti("float64");
const type_info     g_bool_ti("bool8");
const type_info   g_intptr_ti("intptr8");
const type_info  g_nullptr_ti("nullptr");
const type_info     g_void_ti("void");




}




