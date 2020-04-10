#include"libgbstd/vm.hpp"
#include"libgbstd/parser.hpp"




namespace gbstd{


using namespace typesystem;


parameter_list
block_statement::
read_parameter_list(token_iterator&  it) noexcept
{
  parameter_list  ls;

    while(it)
    {
      auto  ti = read_derived_type_info(it);

        if(ti)
        {
            if(it->is_identifier())
            {
              ls.emplace_back(ti,it++->get_string());
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


  return std::move(ls);
}


type_info
block_statement::
read_struct_type_info(token_iterator&  it) noexcept
{
  struct_type_info  sti;

    while(it)
    {
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
block_statement::
read_union_type_info(token_iterator&  it) noexcept
{
  union_type_info  uti;

    while(it)
    {
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
block_statement::
read_enum_type_info(token_iterator&  it) noexcept
{
  enum_type_info  eti(gbstd::type_infos::enum_size);

  int  next = 0;

    while(it)
    {
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
block_statement::
read_alias(token_iterator&  it) noexcept
{
    if(it->is_identifier())
    {
      auto&  name = it++->get_string();

      auto  ti = read_derived_type_info(it);

        if(ti)
        {
          ti.set_name(name);

          m_type_info_table.emplace_back(ti);

          return std::move(ti);
        }
    }


  return type_info();
}


type_info
block_statement::
read_named_struct_type_info(token_iterator&  it) noexcept
{
  auto&  it0 = it[0];
  auto&  it1 = it[1];

    if(it0.is_identifier() && it1.is_block("{","}"))
    {
      auto&  name = it0.get_string();

      token_iterator  coit(it1.get_block());

      auto  ti = read_struct_type_info(coit);

        if(ti)
        {
          it += 2;

          ti.set_name(name);

          m_type_info_table.emplace_back(ti);

          return std::move(ti);
        }
    }


  return type_info();
}


type_info
block_statement::
read_named_union_type_info(token_iterator&  it) noexcept
{
  auto&  it0 = it[0];
  auto&  it1 = it[1];

    if(it0.is_identifier() && it1.is_block("{","}"))
    {
      auto&  name = it0.get_string();

      token_iterator  coit(it++->get_block());

      auto  ti = read_union_type_info(coit);

        if(ti)
        {
          it += 2;

          ti.set_name(name);

          m_type_info_table.emplace_back(ti);

          return std::move(ti);
        }
    }


  return type_info();
}


type_info
block_statement::
read_named_enum_type_info(token_iterator&  it) noexcept
{
  auto&  it0 = it[0];
  auto&  it1 = it[1];

    if(it0.is_identifier() && it1.is_block("{","}"))
    {
      auto&  name = it0.get_string();

      token_iterator  coit(it++->get_block());

      auto  ti = read_enum_type_info(coit);

        if(ti)
        {
          it += 2;

          ti.set_name(name);

          m_type_info_table.emplace_back(ti);

          return std::move(ti);
        }
    }


  return type_info();
}




type_info
block_statement::
read_user_defined_type_info(token_iterator&  it) noexcept
{
    if(it->is_identifier())
    {
      auto&  first = it->get_string();

        if(first == std::string_view("alias"))
        {
          return read_alias(++it);
        }

      else
        if(first == std::string_view("struct"))
        {
          return read_named_struct_type_info(++it);
        }

      else
        if(first == std::string_view("union"))
        {
          return read_named_union_type_info(++it);
        }

      else
        if(first == std::string_view("enum"))
        {
          return read_named_enum_type_info(++it);
        }
    }


  return type_info();
}


const function*
block_statement::
read_function(token_iterator&  it) noexcept
{
   if(it->is_identifier())
   {
     auto&  name = it++->get_string();

     auto  ti = read_derived_type_info(it);

       if(ti)
       {
         auto&  it0 = it[0];
         auto&  it1 = it[1];

           if(it0.is_block("(",")") && it1.is_block("{","}"))
           {
             token_iterator  parals_it(it0.get_block());
             token_iterator    body_it(it1.get_block());

             it += 2;

             auto  parals = read_parameter_list(parals_it);

             auto&  fn = m_global_space->create_function(name,ti,std::move(parals));
           }
       }
   }


  return nullptr;
}


type_info
block_statement::
read_derived_type_info(token_iterator&  it) noexcept
{
    if(!it->is_identifier())
    {
      return type_info();
    }


  auto  ti = find_type_info_by_name(it->get_string());

    if(!ti)
    {
      printf("\"%s\" as typename is not found.\n",it->get_string().data());

      return type_info();
    }


  ++it;

    while(it)
    {
        if(it->is_block("[","]"))
        {
          auto&  blk = it++->get_block();

            if((blk->size() == 1) && blk[0].is_integer())
            {
              ti = ti.form_array_type(blk[0].get_integer());
            }

          else
            {
              report;

              return type_info();
            }
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




type_info
block_statement::
create_type_from_string(std::string_view  sv) noexcept
{
  token_block  blk(sv);

  token_iterator  it(blk);

  return read_derived_type_info(it);
}




}




