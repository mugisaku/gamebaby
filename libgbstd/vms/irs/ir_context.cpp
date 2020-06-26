#include"libgbstd/vms/ir.hpp"




namespace gbstd{




void
ir_context::
read(token_iterator&  it)
{
    while(it)
    {
        if(it[0].is_keyword("function") &&
           it[1].is_identifier()        &&
           it[2].is_operator_code("("))
        {
          auto&  name = it[1].get_string();

            if(find_function(name))
            {
              throw ir_error(form_string("read add_function error: %s",name.data()));
            }


          it += 3;

          std::vector<ir_parameter>  parals;

            for(;;)
            {
                if(!it)
                {
                  throw ir_error("read error");
                }

              else
                if(it->is_operator_code(")"))
                {
                  ++it;

                  break;
                }

              else
                if(it->is_identifier())
                {
                  parals.emplace_back(it++->get_string());

                    if(it->is_operator_code(","))
                    {
                      ++it;
                    }
                }

              else
                {
                  throw ir_error("ir_context::read error: unknown parameter element");
                }
            }


            if(it->is_operator_code("{"))
            {
              auto&  fn = create_function(name,std::move(parals));

              fn.read(++it);
            }
/*
             for(;;)
             {
               ir_type_info  ti;

                 if(it->keyword("int"))
                 {
                 }

               else
                 if(it->keyword("float"))
                 {
                   ti = 
                 }
             }
*/
        }
    }
}


void
ir_context::
read(std::string_view  sv)
{
  auto  toks = make_token_string(sv);

  token_iterator  it(toks);

  read(it);
}




ir_function&
ir_context::
create_function(std::string_view  fn_name, std::vector<ir_parameter>&&  parals) noexcept
{
  return *m_function_list.emplace_back(std::make_unique<ir_function>(fn_name,std::move(parals)));
}


const ir_function*
ir_context::
find_function(std::string_view  label) const noexcept
{
    for(auto&  fn: m_function_list)
    {
        if(fn->get_name() == label)
        {
          return fn.get();
        }
    }


  return nullptr;
}


void
ir_context::
finalize() noexcept
{
    for(auto&  fn: m_function_list)
    {
      fn->finalize();
    }
}


void
ir_context::
print() const noexcept
{
    for(auto&  fn: m_function_list)
    {
      fn->print();

      printf("\n");
    }
}




}




