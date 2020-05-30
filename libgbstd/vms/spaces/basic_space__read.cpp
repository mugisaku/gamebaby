#include"libgbstd/vm.hpp"




namespace gbstd{


using namespace typesystem;


parameter_list
basic_space::
read_parameter_list(token_iterator&  it) noexcept
{
  parameter_list  ls;

    while(it)
    {
        if(it->is_operator_code(")"))
        {
          ++it;

          break;
        }


      auto  ti = read_derived_type_info(it);

        if(ti)
        {
          std::string_view  sv;

            if(it->is_identifier())
            {
              sv = it++->get_string();
            }


            if(it->is_operator_code(","))
            {
              ++it;
            }


          ls.emplace_back(ti,sv);
        }

      else
        {
          break;
        }
    }


  return std::move(ls);
}


type_info
basic_space::
read_struct_type_info(token_iterator&  it) noexcept
{
  struct_type_info  sti;

    while(it)
    {
        if(it->is_operator_code("}"))
        {
          ++it;

          break;
        }


      auto  ti = read_derived_type_info(it);

        if(ti)
        {
            if(it->is_identifier())
            {
              sti.push(ti,it++->get_string());

                if(it->is_operator_code(";"))
                {
                  ++it;
                }
            }

          else
            {
              break;
            }
        }

      else
        {
          break;
        }
    }


  return type_info(std::move(sti));
}


type_info
basic_space::
read_union_type_info(token_iterator&  it) noexcept
{
  union_type_info  uti;

    while(it)
    {
        if(it->is_operator_code("}"))
        {
          ++it;

          break;
        }


      auto  ti = read_derived_type_info(it);

        if(ti)
        {
            if(it->is_identifier())
            {
              uti.push(ti,it++->get_string());

                if(it->is_operator_code(";"))
                {
                  ++it;
                }
            }

          else
            {
              break;
            }
        }

      else
        {
          break;
        }
    }


  return type_info(std::move(uti));
}


type_info
basic_space::
read_enum_type_info(token_iterator&  it) noexcept
{
  enum_type_info  eti(gbstd::type_infos::enum_size);

  int  next = 0;

    while(it)
    {
        if(it->is_operator_code("}"))
        {
          ++it;

          break;
        }


        if(it->is_identifier())
        {
          auto&  id = it++->get_string();

            if(it->is_operator_code("="))
            {
              ++it;

                if(it->is_integer())
                {
                  next = it++->get_integer();

                  eti.push(id,next++);
                }
            }

          else
            {
              eti.push(id,next++);
            }

            
            if(it->is_operator_code(","))
            {
              ++it;
            }
        }

      else
        {
          break;
        }
    }


  return type_info(std::move(eti));
}




type_info
basic_space::
read_alias(token_iterator&  it) noexcept
{
    if(it->is_identifier())
    {
      auto&  name = it++->get_string();

      auto  ti = read_derived_type_info(it);

        if(ti)
        {
          ti.set_name(name);

          push_type_info(std::move(ti));

          return std::move(ti);
        }
    }


  return type_info();
}


type_info
basic_space::
read_named_struct_type_info(token_iterator&  it) noexcept
{
  auto&  it0 = it[0];
  auto&  it1 = it[1];

    if(it0.is_identifier() && it1.is_operator_code("{"))
    {
      auto&  name = it0.get_string();

      it += 2;

      auto  ti = read_struct_type_info(it);

        if(ti)
        {
          ti.set_name(name);

          push_type_info(std::move(ti));

          return std::move(ti);
        }
    }


  return type_info();
}


type_info
basic_space::
read_named_union_type_info(token_iterator&  it) noexcept
{
  auto&  it0 = it[0];
  auto&  it1 = it[1];

    if(it0.is_identifier() && it1.is_operator_code("{"))
    {
      auto&  name = it0.get_string();

      it += 2;

      auto  ti = read_union_type_info(it);

        if(ti)
        {
          ti.set_name(name);

          push_type_info(std::move(ti));

          return std::move(ti);
        }
    }


  return type_info();
}


type_info
basic_space::
read_named_enum_type_info(token_iterator&  it) noexcept
{
  auto&  it0 = it[0];
  auto&  it1 = it[1];

    if(it0.is_identifier() && it1.is_operator_code("{"))
    {
      auto&  name = it0.get_string();

      it += 2;

      auto  ti = read_enum_type_info(it);

        if(ti)
        {
          ti.set_name(name);

          push_type_info(std::move(ti));

          return std::move(ti);
        }
    }


  return type_info();
}


const function*
basic_space::
read_function(token_iterator&  it)
{
    if(m_node.is_function())
    {
      return nullptr;
    }


   if(it->is_identifier())
   {
     auto&  name = it++->get_string();

     auto  ti = read_derived_type_info(it);

       if(ti)
       {
           if(it->is_operator_code("("))
           {
             function_signature  fnsig(ti,read_parameter_list(++it));

               if(it->is_operator_code("{"))
               {
                 auto&  sp = m_node.is_global_space()? static_cast<basic_space&>(m_node.get_global_space())
                            :                          static_cast<basic_space&>(m_node.get_block_space())
                            ;

                 auto&  fn = sp.create_function(name,std::move(fnsig));

                 auto&  blk = fn.get_main_block();

                 blk.read(++it,"}");

                 return &fn;
               }
           }
       }
   }


  return nullptr;
}


type_info
basic_space::
read_derived_type_info(token_iterator&  it)
{
    if(!it->is_identifier())
    {
      return type_info();
    }


  auto  ti_ptr = m_node.find_type_info_by_name(it->get_string());

    if(!ti_ptr)
    {
      throw compile_error(it->get_line_number(),form_string("\"%s\" as typename is not found.\n",it->get_string().data()));
    }


  ++it;

  auto  ti = *ti_ptr;

    while(it)
    {
        if(it->is_operator_code("["))
        {
/*
          auto&  blk = it++->get_block();

            if((blk->size() == 1) && blk[0].is_integer())
            {
              ti = ti.form_array_type(blk[0].get_integer());
            }

          else
            {
              throw compile_error(it->get_line_number(),form_string("配列数指定が不正\n"));
            }
*/
        }

      else
        if(it->is_operator_code("*"))
        {
          ti = ti.form_pointer_type(gbstd::type_infos::pointer_size);

          ++it;
        }

      else
        if(it->is_operator_code("&"))
        {
          ti = ti.form_reference_type(gbstd::type_infos::pointer_size);

          ++it;
        }

      else
        {
          break;
        }
    }


  return ti;
}




void
basic_space::
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
    if(m_node.is_block_space())
    {
      m_node.get_block_space().read_statement(first,it);
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
}


basic_space&
basic_space::
read(token_iterator&  it, operator_code  close_code)
{
    while(it)
    {
      auto&  tok = *it;

        if(tok.is_operator_code(";"))
        {
          ++it;
        }

      else
        if(tok.is_operator_code("("))
        {
            try{
//              push_statement(statement(read_expression(it,")")));
            }


            catch(const expression_error&  err)
            {
              throw compile_error(tok.get_line_number(),"[expression error]");
            }
        }

      else
        if(tok.is_operator_code("{"))
        {
  /*
        auto  blk = m_node.create_block();

          blk->read(++it,"}");

          push_statement(statement(*blk));
*/
        }

      else
        if(tok.is_identifier())
        {
            try{
              read_element_that_begins_with_identifier(it);
            }


            catch(const expression_error&  err)
            {
              throw compile_error(tok.get_line_number(),form_string("[expression_error] %s",err.m_comment.data()));
            }
        }

      else
        if(tok.is_operator_code(close_code))
        {
          ++it;

          break;
        }

      else
        if(tok.is_operator_code(")") ||
           tok.is_operator_code("]") ||
           tok.is_operator_code("}"))
        {
          throw compile_error(tok.get_line_number(),"closing error");
        }

      else
        {
          throw compile_error(tok.get_line_number(),"unknown element");
        }
    }


  return *this;
}


basic_space&
basic_space::
read(std::string_view  sv)
{
  auto  toks = make_token_string(sv);

  token_iterator  it(toks);

  read(it,"");

  return *this;
}




}




