#include"libgbstd/vms/ir.hpp"




namespace gbstd{




namespace{
int64_t
read_integer(token_iterator&  it)
{
    if(it[0].is_operator_code("-") &&
       it[1].is_integer())
    {
      auto  i = -it[1].get_integer();

      it += 2;

      return i;
    }

  else
    if(it->is_integer())
    {
      auto  i = it++->get_integer();

      return i;
    }


  throw ir_error("");
}


double
read_fpn(token_iterator&  it)
{
    if(it[0].is_operator_code("-") &&
       it[1].is_floating_point_number())
    {
      auto  f = -it[1].get_floating_point_number();

      it += 2;

      return f;
    }

  else
    if(it->is_floating_point_number())
    {
      auto  f = it++->get_floating_point_number();

      return f;
    }


  throw ir_error("");
}


std::string
read_byte_string(token_iterator&  it)
{
    if(it->is_string())
    {
      return it++->get_string();
    }


  std::string  buf;

    for(;;)
    {
        if(it->is_operator_code(";"))
        {
          return std::move(buf);
        }

      else
        {
          buf.push_back(static_cast<char>(read_integer(it)));

            if(it->is_operator_code(","))
            {
              ++it;
            }
        }
    }


  throw ir_error("");
}


ir_value
read_value(ir_type_info  ti, token_iterator&  it)
{
    if(it->is_operator_code(";"))
    {
      ++it;

      return ir_value(ti);
    }

  else
    if(it->is_operator_code("="))
    {
      ++it;

        if(ti.is_integer())
        {
          auto  i = read_integer(it);

            if(it->is_operator_code(";"))
            {
              ++it;
            }


          return ir_value(i);
        }

      else
        if(ti.is_fpn())
        {
          auto  f = read_fpn(it);

            if(it->is_operator_code(";"))
            {
              ++it;
            }


          return ir_value(f);
        }

      else
        if(ti.is_object())
        {
          auto  s = read_byte_string(it);

            if(it->is_operator_code(";"))
            {
              ++it;
            }


            if(s.size() > ti.get_size())
            {
              throw ir_error(form_string("read value error: object initializer is too big"));
            }


          s.resize(ti.get_size());

          return ir_value(std::string_view(s));
        }
    }


  throw ir_error(form_string("read value error"));
}


std::vector<ir_parameter>
read_parameter_list(token_iterator&  it)
{
  std::vector<ir_parameter>  ls;

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
        if(it[0].is_identifier() &&
           it[1].is_identifier())
        {
          auto&  ti_s = it++->get_string();
          auto&  lb_s = it++->get_string();

          ls.emplace_back(ir_type_info(ti_s),lb_s);

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


  return std::move(ls);
}


}


void
ir_context::
read_variable(token_iterator&  it)
{
    if(it[0].is_identifier() &&
       it[1].is_identifier())
    {
      ir_type_info  ti(it[0].get_string());

      auto&  name = it[1].get_string();

        if(find_variable_info(name))
        {
          throw ir_error(form_string("read add_variable error: %s",name.data()));
        }


      it += 2;

      auto  v = read_value(ti,it);

      m_variable_info_list.emplace_back(std::move(v),std::string_view(name));
    }
}


void
ir_context::
read_function(token_iterator&  it)
{
    if(it[0].is_identifier() &&
       it[1].is_identifier() &&
       it[2].is_operator_code("("))
    {
      ir_type_info  ret_ti(it[0].get_string());

      auto&  name = it[1].get_string();

        if(find_function(name))
        {
          throw ir_error(form_string("read add_function error: %s",name.data()));
        }


      it += 3;

      auto  parals = read_parameter_list(it);

        if(it->is_operator_code("{"))
        {
          auto&  fn = create_function(ret_ti,name,std::move(parals));

          fn.read(++it);
        }
    }
}


void
ir_context::
read(token_iterator&  it)
{
    while(it)
    {
        if(it->is_keyword("variable"))
        {
          read_variable(++it);
        }

      else
        if(it->is_keyword("function"))
        {
          read_function(++it);
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
create_function(ir_type_info  ret_ti, std::string_view  fn_name, std::vector<ir_parameter>&&  parals) noexcept
{
  return *m_function_list.emplace_back(std::make_unique<ir_function>(ret_ti,fn_name,std::move(parals)));
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


const ir_variable_info*
ir_context::
find_variable_info(std::string_view  name) const noexcept
{
    for(auto&  vi: m_variable_info_list)
    {
        if(vi.get_name() == name)
        {
          return &vi;
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
    for(auto&  vi: m_variable_info_list)
    {
      printf("variable\n");

      vi.print();

      printf("\n");
    }


    for(auto&  fn: m_function_list)
    {
      fn->print();

      printf("\n");
    }
}




}




