#include"libgbstd/vm.hpp"




namespace gbstd{




expression
make_expression(token_iterator&  it)
{
    if(!it || it->is_null())
    {
      return expression();
    }


  auto  first = *it;

  exprrpn  rpn(it);

  std::vector<std::vector<expression>>  table(1);

  std::vector<operator_code>  close_stack;

    for(auto&  e: rpn.get_stack())
    {
      auto&  buf = table.back();

        if(e.is_unsigned_integer())
        {
          buf.emplace_back(static_cast<int64_t>(e.get_unsigned_integer()));
        }

      else
        if(e.is_real_number())
        {
          buf.emplace_back(e.get_real_number());
        }

      else
        if(e.is_identifier())
        {
          auto  s = e.get_string();

            if(s == std::string_view("undefined"))
            {
              buf.emplace_back(undefined());
            }

          else
            if(s == std::string_view("nullptr"))
            {
              buf.emplace_back(nullptr);
            }

          else
            if(s == std::string_view("true"))
            {
              buf.emplace_back(true);
            }

          else
            if(s == std::string_view("false"))
            {
              buf.emplace_back(false);
            }

          else
            {
              identifier  id(s);

              buf.emplace_back(std::move(id));
            }
        }

      else
        if(e.is_string())
        {
          buf.emplace_back(e.get_string());
        }

      else
        if(e.is_prefix_unary_operator())
        {
            if(buf.size() < 1)
            {
              report;

              return {};
            }


          auto  op = e.get_operator_code();
          auto  o  = std::make_unique<expression>(std::move(buf.back()));

          buf.back().assign(prefix_unary_operation(op,std::move(o)));
        }

      else
        if(e.is_postfix_unary_operator())
        {
            if(buf.size() < 1)
            {
              report;

              return {};
            }


          auto  op = e.get_operator_code();
          auto  o  = std::make_unique<expression>(std::move(buf.back()));

          buf.back().assign(postfix_unary_operation(op,std::move(o)));
        }

      else
        if(e.is_binary_operator())
        {
            if(buf.size() < 2)
            {
              report;

              return {};
            }


          auto  r = std::make_unique<expression>(std::move(buf.back()));

          buf.pop_back();

          auto  l = std::make_unique<expression>(std::move(buf.back()));

          auto  op = e.get_operator_code();

          buf.back() = binary_operation(op,std::move(l),std::move(r));
        }

      else
        if(e.is_operator())
        {
          auto  op = e.get_operator_code();

            if(op == "(")
            {
              close_stack.emplace_back(")");

              table.emplace_back();
            }

          else
            if(op == "[")
            {
              close_stack.emplace_back("]");

              table.emplace_back();
            }

          else
            if(op == "{")
            {
              close_stack.emplace_back("}");

              table.emplace_back();
            }

          else
            if(op == close_stack.back())
            {
              close_stack.pop_back();

              expression  t;

                if(buf.size())
                {
                    if(buf.size() > 1)
                    {
                      report;
                    }


                  t = std::move(buf.back());
                }


              table.pop_back();

                if(table.back().size() < 1)
                {
                  table.back().emplace_back(std::move(t));
                }

              else
                {
                  operator_code  opop( (op == ")")? "(?)"
                                      :(op == "]")? "[?]"
                                      :(op == "}")? "{?}"
                                      :"")
                                      ;

                  auto  l = std::make_unique<expression>(std::move(table.back().back()));
                  auto  r = std::make_unique<expression>(std::move(t                  ));
     
                  table.back().back().assign(binary_operation(opop,std::move(l),std::move(r)));
                }
            }

          else
            {
              report;
            }
        }

      else
        {
          report;
        }
    }


    if(table.size() == 1)
    {
      auto&  buf = table.back();

        if(buf.size() == 1)
        {
          return std::move(buf.back());
        }
    }


  report;

  rpn.print();

  printf("\n%s\n",first.get_begin());

  return expression();
}


expression
make_expression(std::string_view  sv)
{
  token_block  blk(sv);

  token_iterator  it(blk);

  return make_expression(it);
}




}




