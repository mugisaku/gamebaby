#include"libgbstd/vms/ir.hpp"




namespace gbstd{




ir_parameter
make_parameter(const syntax_branch&  br) noexcept
{
  auto     ti = make_type_info(br[0].branch());
  auto&  name = br[1].token().get_string();

  return ir_parameter(ti,name);
}


ir_parameter_list
make_parameter_list(const syntax_branch&  br) noexcept
{
  ir_parameter_list  ls;

    for(auto&  e: br)
    {
        if(e.is_definition(u"parameter"))
        {
          ls.emplace_back(make_parameter(e.branch()));
        }
    }


  return std::move(ls);
}


ir_define_operation
make_define_operation(const syntax_branch&  br) noexcept
{
  auto&  tok = br[1].token();

  return tok.is_integer()? ir_define_operation(tok.get_integer())
        :                  ir_define_operation(tok.get_floating())
        ;
}


ir_unary_operation
make_unary_operation(const syntax_branch&  br) noexcept
{
  auto&  opco1 = br[0].branch()[0].token().get_string();
  auto&  opco2 = br[0].branch()[1].token().get_string();
  auto       o = make_identifier(br[1].branch());

  return ir_unary_operation(opco1,opco2,o);
}


ir_binary_operation
make_binary_operation(const syntax_branch&  br) noexcept
{
  auto&  opco1 = br[0].branch()[0].token().get_string();
  auto&  opco2 = br[0].branch()[1].branch()[0].token().get_string();
  auto      o1 = make_identifier(br[1].branch());
  auto      o2 = make_identifier(br[2].branch());

  return ir_binary_operation(opco1,opco2,o1,o2);
}


ir_load_operation
make_load_operation(const syntax_branch&  br) noexcept
{
  auto   ti = make_type_info(br[0].branch());
  auto&  id = make_identifier(br[1].branch());

  return ir_load_operation(ti,id);
}


ir_address_operation
make_address_operation(const syntax_branch&  br) noexcept
{
  return ir_address_operation(u"");
}


ir_call_operation
make_call_operation(const syntax_branch&  br) noexcept
{
  return ir_call_operation(u"",{});
}


ir_phi_operation
make_phi_operation(const syntax_branch&  br) noexcept
{
  return ir_phi_operation({});
}


ir_operation
make_operation(const syntax_branch&  br) noexcept
{
  ir_operation  op;

  auto&  e = br[0];

       if(e.is_definition(u"define_operation") ){op = make_define_operation(e.branch());}
  else if(e.is_definition(u"unary_operation")  ){op = make_unary_operation(e.branch());}
  else if(e.is_definition(u"binary_operation") ){op = make_binary_operation(e.branch());}
  else if(e.is_definition(u"load_operation")   ){op = make_load_operation(e.branch());}
  else if(e.is_definition(u"address_operation")){op = make_address_operation(e.branch());}
  else if(e.is_definition(u"call_operation")   ){op = make_call_operation(e.branch());}
  else if(e.is_definition(u"phi_operation")    ){op = make_phi_operation(e.branch());}
  else{}


  return std::move(op);
}


ir_register_statement
make_register_statement(const syntax_branch&  br) noexcept
{
  auto&  id = br[1].token().get_string();
  auto   op = make_operation(br[2].branch());

  return ir_register_statement(id,std::move(op));
}


ir_statement
make_statement(const syntax_branch&  br) noexcept
{
  ir_statement  stmt;

  auto&  e = br[0];

    if(e.is_definition(u"register_statement")){stmt = make_register_statement(e.branch());}


  return std::move(stmt);
}


ir_statement_list
make_statement_list(const syntax_branch&  br) noexcept
{
  ir_statement_list  ls;

    for(auto&  e: br)
    {
        if(e.is_definition(u"statement"))
        {
          ls.emplace_back(make_statement(e.branch()));
        }
    }


  return std::move(ls);
}


ir_type_info
make_type_info(const syntax_branch&  br) noexcept
{
  ir_type_info  ti;

  auto&  kw = br[0].token().get_string();

       if(kw == u"void"){}
  else if(kw == u"i8" ){ti = ir_type_info('i',1);}
  else if(kw == u"i16"){ti = ir_type_info('i',2);}
  else if(kw == u"i32"){ti = ir_type_info('i',4);}
  else if(kw == u"i64"){ti = ir_type_info('i',8);}
  else if(kw == u"f32"){ti = ir_type_info('f',4);}
  else if(kw == u"f64"){ti = ir_type_info('f',8);}
  else
    {
    }


  return ti;
}


std::u16string_view
make_identifier(const syntax_branch&  br) noexcept
{
  return br[0].token().get_string();
}


ir_function
make_function(const syntax_branch&  br) noexcept
{
  auto     ti = make_type_info(br[1].branch());
  auto  paras = make_parameter_list(br[2].branch());
  auto&    id = br[3].token().get_string();
  auto  stmts = make_statement_list(br[4].branch());

  return ir_function(ti,std::move(paras),id,std::move(stmts));
}


ir_global_space&
ir_global_space::
assign(const syntax_branch&  br) noexcept
{
  m_function_list.clear();

    for(auto&  e: br)
    {
        if(e.is_definition(u"element"))
        {
            for(auto&  ee: e.branch())
            {
                if(ee.is_definition(u"function_declaration"))
                {
                  auto  fn = make_function(ee.branch());

                  m_function_list.emplace_back(std::move(fn));
                }
            }
        }
    }


  return *this;
}




void
ir_global_space::
print() const noexcept
{
    for(auto&  fn: m_function_list)
    {
      fn.print();
    }
}




}




