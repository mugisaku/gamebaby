#include"libgbstd/vm.hpp"
#include"libgbstd/parser.hpp"




namespace gbstd{


using namespace typesystem;




statement
space_node::
read_return(token_iterator&  it) noexcept
{
  return statement(return_statement(make_expression(it)));
}


statement
space_node::
read_jump(token_iterator&  it) noexcept
{
   if(it->is_identifier())
   {
     return statement(jump_statement(it++->get_string()));
   }

 else
   {
     report;
   }


  return statement();
}


statement
space_node::
read_label(token_iterator&  it) noexcept
{
  return statement();
}


statement
space_node::
read_if(token_iterator&  it) noexcept
{
  auto&  tok0 = it[0];
  auto&  tok1 = it[1];

    if(tok0.is_block("(",")") && tok1.is_block("{","}"))
    {
      it += 2;
    }

  else
    {
      report;
    }


  return statement();
}


statement
space_node::
read_for(token_iterator&  it) noexcept
{
  auto&  tok0 = it[0];
  auto&  tok1 = it[1];

    if(tok0.is_block("(",")") && tok1.is_block("{","}"))
    {
      it += 2;
    }

  else
    {
      report;
    }


  return statement();
}


statement
space_node::
read_while(token_iterator&  it) noexcept
{
  auto&  tok0 = it[0];
  auto&  tok1 = it[1];

    if(tok0.is_block("(",")") && tok1.is_block("{","}"))
    {
      it += 2;
    }

  else
    {
      report;
    }


  return statement();
}


statement
space_node::
read_switch(token_iterator&  it) noexcept
{
  auto&  tok0 = it[0];
  auto&  tok1 = it[1];

    if(tok0.is_block("(",")") && tok1.is_block("{","}"))
    {
      it += 2;
    }

  else
    {
      report;
    }


  return statement();
}


statement
space_node::
read_let(token_iterator&  it)
{
  auto&  it0 = it[0];
  auto&  it1 = it[1];

    if(it0.is_identifier() && it1.is_operator_code("="))
    {
      auto&  name = it0.get_string();

      it += 2;

      auto  ti = read_derived_type_info(it);

        if(ti)
        {
          expression  e;

          push_memo_info(ti,name);

            if(it->is_block("(",")"))
            {
              token_iterator  coit(it++->get_block());

              e = make_expression(coit);
            }


          return statement(let_statement(name,std::move(e)));
        }
    }


  return statement();
}


void
space_node::
read_element_that_begins_with_identifier(token_iterator&  it)
{
  auto&  first = it->get_string();

    if(first == std::string_view("function"))
    {
      read_function(++it);
    }

  else
    if(first == std::string_view("function_pointer"))
    {
      ++it;
    }

  else
    if(first == std::string_view("return"))
    {
      push_statement(read_return(++it));
    }

  else
    if(first == std::string_view("jump"))
    {
      push_statement(read_jump(++it));
    }

  else
    if((first ==     "break") ||
       (first ==  "continue") ||
       (first == "interrupt"))
    {
      ++it;

      push_statement(statement(control_statement(first)));
    }

  else
    if(first == std::string_view("if"))
    {
      push_statement(read_if(++it));
    }

  else
    if(first == std::string_view("while"))
    {
      push_statement(read_while(++it));
    }

  else
    if(first == std::string_view("for"))
    {
      push_statement(read_for(++it));
    }

  else
    if(first == std::string_view("switch"))
    {
      push_statement(read_switch(++it));
    }

  else
    if(first == std::string_view("let"))
    {
      push_statement(read_let(++it));
    }

  else
    if(first == std::string_view("alias"))
    {
      read_alias(++it);
    }

  else
    if(first == std::string_view("struct"))
    {
      read_named_struct_type_info(++it);
    }

  else
    if(first == std::string_view("union"))
    {
      read_named_union_type_info(++it);
    }

  else
    if(first == std::string_view("enum"))
    {
      read_named_enum_type_info(++it);
    }

  else
    {
      push_statement(statement(make_expression(it)));
    }
}


space_node&
space_node::
read(token_iterator&  it)
{
    while(it)
    {
      auto&  tok = *it;

        if(tok.is_operator_code(";"))
        {
          ++it;
        }

      else
        if(tok.is_block("(",")"))
        {
          ++it;

          token_iterator  coit(tok.get_block());

            try{
              push_statement(statement(make_expression(coit)));
            }


            catch(const expression_error&  err)
            {
              throw compile_error(tok.get_line_number(),"[expression error]");
            }
        }

      else
        if(tok.is_block("{","}"))
        {
          ++it;

          token_iterator  coit(tok.get_block());

          auto  blk = create_block();

          blk->get_node().read(coit);

          push_statement(statement(*blk));
        }

      else
        if(tok.is_identifier())
        {
            try{
              read_element_that_begins_with_identifier(it);
            }


            catch(const expression_error&  err)
            {
              throw compile_error(tok.get_line_number(),"[expression_error]");
            }
        }

      else
        {
          throw compile_error(tok.get_line_number(),"unknown element");
        }
    }


  return *this;
}


space_node&
space_node::
read(std::string_view  sv)
{
  token_block  blk(sv);

  token_iterator  it(blk);

  read(it);

  return *this;
}




}




