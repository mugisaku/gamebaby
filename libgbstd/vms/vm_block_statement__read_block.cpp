#include"libgbstd/vm.hpp"
#include"libgbstd/parser.hpp"




namespace gbstd{


using namespace typesystem;


statement
block_statement::
read_return(token_iterator&  it) noexcept
{
  return statement(this,return_statement(make_expression(it)));
}


statement
block_statement::
read_jump(token_iterator&  it) noexcept
{
   if(it->is_identifier())
   {
     return statement(this,jump_statement(it++->get_string()));
   }

 else
   {
     report;
   }


  return statement();
}


statement
block_statement::
read_label(token_iterator&  it) noexcept
{
  return statement();
}


statement
block_statement::
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
block_statement::
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
block_statement::
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
block_statement::
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
block_statement::
read_let(token_iterator&  it) noexcept
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

//          m_symbol_table->push(*ti,name,0,nullptr);

            if(it->is_block("(",")"))
            {
              token_iterator  coit(it++->get_block());

              e = make_expression(coit);
            }


          return statement(this,let_statement(name,std::move(e)));
        }
    }


  return statement();
}


block_statement&
block_statement::
read(token_iterator&  it) noexcept
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

          push(statement(this,make_expression(coit)));
        }

      else
        if(tok.is_block("{","}"))
        {
          ++it;

          token_iterator  coit(tok.get_block());

          push(statement(this,std::make_unique<block_statement>(this,coit)));
        }

      else
        if(read_user_defined_type_info(it))
        {
          continue;
        }

      else
        if(tok.is_identifier())
        {
          auto&  first = tok.get_string();

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
              push(read_return(++it));
            }

          else
            if(first == std::string_view("jump"))
            {
              push(read_jump(++it));
            }

          else
            if((first ==     "break") ||
               (first ==  "continue") ||
               (first == "interrupt"))
            {
              ++it;

              push(statement(this,control_statement(first)));
            }

          else
            if(first == std::string_view("if"))
            {
              push(read_if(++it));
            }

          else
            if(first == std::string_view("while"))
            {
              push(read_while(++it));
            }

          else
            if(first == std::string_view("for"))
            {
              push(read_for(++it));
            }

          else
            if(first == std::string_view("switch"))
            {
              push(read_switch(++it));
            }

          else
            if(first == std::string_view("let"))
            {
              push(read_let(++it));
            }

          else
            {
              push(statement(this,make_expression(it)));
            }
        }

      else
        {
          report;
           break;
        }
    }


  return *this;
}


block_statement&
block_statement::
read(std::string_view  sv) noexcept
{
  token_block  blk(sv);

  token_iterator  it(blk);

  read(it);

  return *this;
}




}




