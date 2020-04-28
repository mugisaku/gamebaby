#include"libgbstd/vm.hpp"
#include"libgbstd/parser.hpp"




namespace gbstd{


using namespace typesystem;


parameter_list
space_node::
read_parameter_list(token_iterator&  it) noexcept
{
  parameter_list  ls;

    while(it)
    {
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
space_node::
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
space_node::
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
space_node::
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
space_node::
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
space_node::
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

          push_type_info(std::move(ti));

          return std::move(ti);
        }
    }


  return type_info();
}


type_info
space_node::
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

          push_type_info(std::move(ti));

          return std::move(ti);
        }
    }


  return type_info();
}


type_info
space_node::
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

          push_type_info(std::move(ti));

          return std::move(ti);
        }
    }


  return type_info();
}


const function*
space_node::
read_function(token_iterator&  it)
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

             function_signature  fnsig(ti,read_parameter_list(parals_it));

             auto  fn = create_function(name,std::move(fnsig));

               if(fn)
               {
                 auto  blk = fn->get_node().create_block();

                   if(blk)
                   {
                     blk->get_node().read(body_it);

                     fn->set_main_block(*blk);

                     return fn;
                   }
               }
           }
       }
   }


  return nullptr;
}


type_info
space_node::
read_derived_type_info(token_iterator&  it)
{
    if(!it->is_identifier())
    {
      return type_info();
    }


  auto  ti = find_type_info_by_name(it->get_string());

    if(!ti)
    {
      throw compile_error(it->get_line_number(),form_string("\"%s\" as typename is not found.\n",it->get_string().data()));
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
              throw compile_error(it->get_line_number(),form_string("配列数指定が不正\n"));
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




}




