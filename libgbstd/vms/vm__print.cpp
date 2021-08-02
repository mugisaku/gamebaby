#include"libgbstd/vm.hpp"
#include<cinttypes>




namespace gbstd{




namespace{
void  print_type(const syntax_branch&  br) noexcept;


void
print_operand(const syntax_branch&  br) noexcept
{
  gbstd::print(br[0].string());
}


void
print_parameter(const syntax_branch&  br) noexcept
{
  print_type(br[0].branch());

  printf("  ");

  gbstd::print(br[1].string());
}


void
print_parameter_list(const syntax_branch&  br) noexcept
{
  printf("(");

    for(auto&  e: br)
    {
        if(e.is_branch(u"parameter"))
        {
          print_parameter(e.branch());

          printf(", ");
        }
    }


  printf(")");
}


void
print_define_operation(const syntax_branch&  br) noexcept
{
  printf("def ");

  auto&  tok = br[1].token();

    if(tok.is_integer())
    {
      printf("%" PRIu64,tok.integer());
    }

  else
    {
//      tok.floating();
    }
}


void
print_unary_operation(const syntax_branch&  br) noexcept
{
}


void
print_binary_instruction(const syntax_branch&  br) noexcept
{
  gbstd::print(br[0].string());
}


void
print_binary_operation(const syntax_branch&  br) noexcept
{
  print_binary_instruction(br[0].branch());

  printf(" ");

  print_operand(br[1].branch());

  printf(" ");

  print_operand(br[2].branch());
}


void
print_address_operation(const syntax_branch&  br) noexcept
{
}


void
print_call_operation(const syntax_branch&  br) noexcept
{
}


void
print_phi_operation(const syntax_branch&  br) noexcept
{
}


void
print_operation(const syntax_branch&  br) noexcept
{
  auto&  e = br[0];

       if(e.is_branch(u"define_operation") ){print_define_operation(e.branch());}
  else if(e.is_branch(u"unary_operation")  ){print_unary_operation(e.branch());}
  else if(e.is_branch(u"binary_operation") ){print_binary_operation(e.branch());}
  else if(e.is_branch(u"address_operation")){print_address_operation(e.branch());}
  else if(e.is_branch(u"call_operation")   ){print_call_operation(e.branch());}
  else if(e.is_branch(u"phi_operation")    ){print_phi_operation(e.branch());}
  else{}
}


void
print_register_statement(const syntax_branch&  br) noexcept
{
  printf("reg ");

  print_type(br[1].branch());

  printf(" ");

  gbstd::print(br[2].string());

  printf(" ");

  print_operation(br[3].branch());
}


void
print_return_statement(const syntax_branch&  br) noexcept
{
  printf("ret ");

    if(br.length() == 2)
    {
      print_operand(br[1].branch());
    }
}


void
print_branch_statement(const syntax_branch&  br) noexcept
{
  gbstd::print(br[1].string());
  gbstd::print(br[2].string());
}


void
print_label_statement(const syntax_branch&  br) noexcept
{
  gbstd::print(br[1].string());
}


void
print_store_statement(const syntax_branch&  br) noexcept
{
  print_type(br[1].branch());

  gbstd::print(br[2].string());
  gbstd::print(br[3].string());
}


void
print_statement(const syntax_branch&  br) noexcept
{
  auto&  e = br[0];

       if(e.is_branch(u"register_statement")){print_register_statement(e.branch());}
  else if(e.is_branch(u"return_statement")  ){print_return_statement(e.branch());}
  else if(e.is_branch(u"branch_statement")  ){print_branch_statement(e.branch());}
  else if(e.is_branch(u"label_statement")   ){print_label_statement(e.branch());}
  else if(e.is_branch(u"store_statement")   ){print_store_statement(e.branch());}
  else{;}
}


void
print_statement_list(const syntax_branch&  br) noexcept
{
    for(auto&  e: br)
    {
        if(e.is_branch(u"statement"))
        {
          printf("  ");
  
          print_statement(e.branch());

          printf("\n");
        }
    }
}


void
print_type(const syntax_branch&  br) noexcept
{
  gbstd::print(br[0].string());
}


void
print_function(const syntax_branch&  br) noexcept
{
  printf("function\n");

  print_type(br[1].branch());

  print_parameter_list(br[2].branch());

  printf("\n");

  gbstd::print(br[3].string());

  printf("\n{\n");

  print_statement_list(br[4].branch());

  printf("}\n");
}
}




void
print_branch_as_ir(const syntax_branch&  br)
{
    for(auto&  e: br)
    {
        if(e.is_branch(u"element"))
        {
            for(auto&  ee: e.branch())
            {
                if(ee.is_branch(u"function_declaration"))
                {
                  print_function(ee.branch());
                }
            }
        }
    }
}




}




