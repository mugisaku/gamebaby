#include"libgbstd/vm.hpp"
#include"libgbstd/parser.hpp"




namespace gbstd{


using namespace typesystem;


parameter_list
context::
read_parameter_list(token_iterator&  it) noexcept
{
  parameter_list  ls;

    while(it)
    {
      auto  ti = read_type(it);

        if(ti)
        {
            if(it->is_identifier())
            {
              ls.emplace_back(*ti,it++->get_string());
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


const type_info*
context::
read_struct_type(token_iterator&  it) noexcept
{
  struct_type_info  sti;

    while(it)
    {
      auto  ti = read_type(it);

        if(ti)
        {
            if(it->is_identifier())
            {
              sti.push(*ti,it++->get_string());

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


  auto  ti = std::make_unique<type_info>(std::move(sti));

  return &append_type_info(std::move(ti));
}


const type_info*
context::
read_union_type(token_iterator&  it) noexcept
{
  union_type_info  uti;

    while(it)
    {
      auto  ti = read_type(it);

        if(ti)
        {
            if(it->is_identifier())
            {
              uti.push(*ti,it++->get_string());

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


  auto  ti = std::make_unique<type_info>(std::move(uti));

  return &append_type_info(std::move(ti));
}


const type_info*
context::
read_enum_type(token_iterator&  it) noexcept
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


  auto  ti = std::make_unique<type_info>(std::move(eti));

  return &append_type_info(std::move(ti));
}




bool
context::
read_declaration(token_iterator&  it) noexcept
{
  const type_info*  ti = nullptr;

    if(it->is_identifier())
    {
      auto&  first = it->get_string();

        if(first == std::string_view("alias"))
        {
          ++it;

            if(it->is_identifier())
            {
              auto&  name = it++->get_string();

              ti = read_type(it);

                if(ti)
                {
                  m_type_collection.push(name,*ti);

                  return true;
                }
            }
        }

      else
        if(first == std::string_view("struct"))
        {
          ++it;

          auto&  it0 = it[0];
          auto&  it1 = it[1];

            if(it0.is_identifier() && it1.is_block("{","}"))
            {
              auto&  name = it0.get_string();

              token_iterator  coit(it1.get_block());

              ti = read_struct_type(coit);

                if(ti)
                {
                  it += 2;

                  m_type_collection.push(name,*ti);

                  return true;
                }
            }
        }

      else
        if(first == std::string_view("union"))
        {
          ++it;

          auto&  it0 = it[0];
          auto&  it1 = it[1];

            if(it0.is_identifier() && it1.is_block("{","}"))
            {
              auto&  name = it0.get_string();

              token_iterator  coit(it++->get_block());

              ti = read_union_type(coit);

                if(ti)
                {
                  it += 2;

                  m_type_collection.push(name,*ti);

                  return true;
                }
            }
        }

      else
        if(first == std::string_view("enum"))
        {
          ++it;

          auto&  it0 = it[0];
          auto&  it1 = it[1];

            if(it0.is_identifier() && it1.is_block("{","}"))
            {
              auto&  name = it0.get_string();

              token_iterator  coit(it++->get_block());

              ti = read_enum_type(coit);

                if(ti)
                {
                  it += 2;

                  m_type_collection.push(name,*ti);

                  return true;
                }
            }
        }

      else
        if(first == std::string_view("function_pointer"))
        {
          ++it;

/*
                if(it->is_block("(",")"))
                {
                  token_iterator  coit(it++->get_block());

                  function_signature  fnsig(*ti,read_parameter_list(coit));

                  function_pointer_type_info  fnptr_ti(std::move(fnsig),type_infos::pointer_size);

                  append_type_info(std::make_unique<type_info>(std::move(fnptr_ti)));
                }

              else
                {
                  return;
                }
            }

          else
            {
              return;
            }
*/
        }

      else
        if(first == std::string_view("function"))
        {
          ++it;

            if(it->is_identifier())
            {
              auto&  name = it++->get_string();

              ti = read_type(it);

                if(ti)
                {
                  auto&  it0 = it[0];
                  auto&  it1 = it[1];

                    if(it0.is_block("(",")") && it1.is_block("{","}"))
                    {
                      token_iterator  parals_it(it0.get_block());
                      token_iterator     blk_it(it1.get_block());

                      it += 2;

                      auto  parals = read_parameter_list(parals_it);
                      auto     blk = read_block(            blk_it);

                      auto  fn = std::make_unique<function>(*ti,std::move(parals),std::move(blk));

                      append_function(std::move(fn));
                    }
                }
            }
        }

      else
        {
          ti = read_type(it);

            if(ti)
            {
                if(it->is_identifier())
                {
                  auto&  name = it++->get_string();

                  m_global_symbol_table.push(symbol(*ti,name,0,nullptr));

                  return true;
                }
            }
        }
    }


  return false;
}


const type_info*
context::
read_type(token_iterator&  it) noexcept
{
    if(!it->is_identifier())
    {
      return nullptr;
    }


  auto  ti = m_type_collection.find_by_name(it->get_string());

    if(!ti)
    {
      printf("\"%s\" as typename is not found.\n",it->get_string().data());

      return nullptr;
    }


  ++it;

    while(it)
    {
        if(it->is_block("[","]"))
        {
          auto&  blk = it++->get_block();

            if((blk->size() == 1) && blk[0].is_integer())
            {
              ti = &ti->form_array_type(blk[0].get_integer());
            }

          else
            {
              report;

              return nullptr;
            }
        }

      else
        if(it->is_operator_code("*"))
        {
          ti = &ti->form_pointer_type(gbstd::type_infos::pointer_size);

          ++it;
        }

      else
        if(it->is_operator_code("&"))
        {
          ti = &ti->form_reference_type(gbstd::type_infos::pointer_size);

          ++it;
        }

      else
        {
          break;
        }
    }


  return ti;
}




const type_info*
context::
create_type_from_string(std::string_view  sv) noexcept
{
  token_block  blk(sv);

  token_iterator  it(blk);

  return read_type(it);
}




}




