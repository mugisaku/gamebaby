#include"parser.hpp"
#include<memory>



namespace gbstd{




namespace{




constexpr int
get_precedence(exprelem  e) noexcept
{
  auto  o = e.get_operator_code();

    if(e.is_prefix_unary_operator())
    {
        if((o == "++") ||
           (o == "--") ||
           (o == "*") ||
           (o == "&") ||
           (o == "+") ||
           (o == "-") ||
           (o == "!") ||
           (o == "~"))
        {
          return 65;
        }
    }

  else
    if(e.is_postfix_unary_operator())
    {
        if((o == "++") ||
           (o == "--"))
        {
          return 70;
        }
    }

  else
    if(e.is_binary_operator())
    {
        if(o == "::" )
        {
          return 80;
        }

      else
        if((o == "." ) ||
           (o == "->"))
        {
          return 70;
        }

      else
        if((o == ".*") ||
           (o == "->*"))
        {
          return 60;
        }

      else
        if((o == "*") ||
           (o == "/") ||
           (o == "%"))
        {
          return 20;
        }

      else
        if((o == "+") ||
           (o == "-"))
        {
          return 19;
        }

      else
        if((o == "<<") ||
           (o == ">>"))
        {
          return 18;
        }

      else
        if((o == "<" ) ||
           (o == "<=") ||
           (o == ">" ) ||
           (o == ">="))
        {
          return 17;
        }

      else
        if((o == "==") ||
           (o == "!="))
        {
          return 16;
        }

      else if(o == "&" ){return 15;}
      else if(o == "^" ){return 14;}
      else if(o == "|" ){return 13;}
      else if(o == "&&"){return 12;}
      else if(o == "||"){return 11;}

      else
        if((o == "=") ||
           (o == "+=") ||
           (o == "-=") ||
           (o == "*=") ||
           (o == "/=") ||
           (o == "%=") ||
           (o == "&=") ||
           (o == "|=") ||
           (o == "^=") ||
           (o == "<<=") ||
           (o == ">>=") ||
           (o == "="))
        {
          return 3;
        }

      else if(o == ","){return 1;}
    }


  return 0;
}


template<typename  T>
T
operate(T  operand, operator_code  o) noexcept
{
       if(o == "-" ){return -operand;}
  else if(o == "!" ){return !operand;}
  else if(o == "~" ){return -operand;}


  report;
  return  0;
}


template<typename  T>
T
operate(T  l, T  r, operator_code  o) noexcept
{
       if(o == "+" ){return l+r;}
  else if(o == "-" ){return l-r;}
  else if(o == "*" ){return l*r;}
  else if(o == "/" ){return l/r;}
  else if(o == "%" ){return l%r;}
  else if(o == "<<"){return l<<r;}
  else if(o == ">>"){return l>>r;}
  else if(o == "=="){return (l == r? 1:0);}
  else if(o == "!="){return (l != r? 1:0);}
  else if(o == "<" ){return (l <  r? 1:0);}
  else if(o == "<="){return (l <= r? 1:0);}
  else if(o == ">="){return (l >= r? 1:0);}
  else if(o == "|" ){return l|r;}
  else if(o == "&" ){return l&r;}
  else if(o == "^" ){return l^r;}
  else if(o == "||"){return (l || r? 1:0);}
  else if(o == "&&"){return (l && r? 1:0);}

  report;
  return  0;
}


double
operate(double  l, double  r, operator_code  o) noexcept
{
       if(o == "+" ){return l+r;}
  else if(o == "-" ){return l-r;}
  else if(o == "*" ){return l*r;}
  else if(o == "/" ){return l/r;}

  report;
  return  0;
}


template<typename  T>
T
boperate(T  l, T  r, operator_code  o) noexcept
{
       if(o == "=="){return (l == r? 1:0);}
  else if(o == "!="){return (l != r? 1:0);}
  else if(o == "<" ){return (l <  r? 1:0);}
  else if(o == "<="){return (l <= r? 1:0);}
  else if(o == ">" ){return (l >  r? 1:0);}
  else if(o == ">="){return (l >= r? 1:0);}
  else if(o == "||"){return (l || r? 1:0);}
  else if(o == "&&"){return (l && r? 1:0);}

  report;
  return  0;
}


int64_t
boperate(double  l, double  r, operator_code  o) noexcept
{
       if(o == "=="){return (l == r? 1:0);}
  else if(o == "!="){return (l != r? 1:0);}
  else if(o == "<" ){return (l <  r? 1:0);}
  else if(o == "<="){return (l <= r? 1:0);}
  else if(o == ">" ){return (l >  r? 1:0);}
  else if(o == ">="){return (l >= r? 1:0);}

  report;
  return  0;
}


}




void
exprrpn::
finish(std::vector<exprelem>&&  src, std::vector<exprelem>&  dst) noexcept
{
    while(src.size())
    {
      dst.emplace_back(src.back());

      src.pop_back();
    }
}


void
exprrpn::
preprocess(token_iterator&  top_it) noexcept
{
  std::vector<token_iterator>  it_stack({top_it});

  std::vector<operator_code>  close_stack;

  enum class kind{
    null,operand,prefix_unary,postfix_unary,binary,
  } last_kind=kind::null;


    for(;;)
    {
        while(it_stack.size())
        {
          auto&  it = it_stack.back();

            if(!it)
            {
              break;
            }

          else
            if(it->is_integer())
            {
              m_stack.emplace_back(it++->get_integer());

              last_kind = kind::operand;
            }

          else
            if(it->is_floating_point_number())
            {
              m_stack.emplace_back(it++->get_floating_point_number());

              last_kind = kind::operand;
            }

          else
            if(it->is_string())
            {
              m_strings.emplace_back(it++->get_string());

              m_stack.emplace_back('s',m_strings.back());

              last_kind = kind::operand;
            }

          else
            if(it->is_identifier())
            {
              m_stack.emplace_back('I',it++->get_string());

              last_kind = kind::operand;
            }

          else
            if(it->is_operator_code())
            {
              auto  op = it++->get_operator_code();

                if(op == ";")
                {
                  break;
                }


                if((last_kind == kind::null        ) ||
                   (last_kind == kind::prefix_unary) ||
                   (last_kind == kind::binary      ))
                {
                  m_stack.emplace_back('p',op);

                  last_kind = kind::prefix_unary;
                }

              else
                if(last_kind == kind::postfix_unary)
                {
                  m_stack.emplace_back('B',op);

                  last_kind = kind::binary;
                }

              else
                if(last_kind == kind::operand)
                {
                    if((op == "++") ||
                       (op == "--"))
                    {
                      m_stack.emplace_back('P',op);

                      last_kind = kind::postfix_unary;
                    }

                  else
                    {
                      m_stack.emplace_back('B',op);

                      last_kind = kind::binary;
                    }
                }
            }

          else
            if(it->is_block("(",")"))
            {
              m_stack.emplace_back('o',operator_code("("));

              close_stack.emplace_back(")");

              it_stack.emplace_back(it++->get_block());

              last_kind = kind::null;
            }

          else
            if(it->is_block("[","]"))
            {
              m_stack.emplace_back('o',operator_code("["));

              close_stack.emplace_back("]");

              it_stack.emplace_back(it++->get_block());

              last_kind = kind::null;
            }

          else
            if(it->is_block("{","}"))
            {
              m_stack.emplace_back('o',operator_code("{"));

              close_stack.emplace_back("}");

              it_stack.emplace_back(it++->get_block());

              last_kind = kind::null;
            }

          else
            {
              report;
              return;
            }
        }


        if(it_stack.size() > 1)
        {
          m_stack.emplace_back('o',close_stack.back());

          close_stack.pop_back();

          it_stack.pop_back();

          last_kind = kind::operand;
        }

      else
        {
          break;
        }
    }


  top_it = it_stack.back();
//print();
}


void
exprrpn::
postprocess()
{
  std::vector<exprelem>  tmp;

  std::vector<std::vector<exprelem>>  operator_stack(1);
  std::vector<operator_code>             close_stack;

    for(auto&  e: m_stack)
    {
        if(e.is_operand())
        {
          tmp.emplace_back(e);
        }

      else
        if(e.is_prefix_unary_operator()  ||
           e.is_postfix_unary_operator() ||
           e.is_binary_operator())
        {
          auto&  st = operator_stack.back();

            while(st.size())
            {
              auto  prev_op = st.back();

                if(get_precedence(e) > get_precedence(prev_op))
                {
                  break;
                }


              tmp.emplace_back(st.back());

              st.pop_back();
            }


          st.emplace_back(e);
        }

      else
        if(e.is_operator())
        {
          auto  o = e.get_operator_code();

            if(o == "(")
            {
              close_stack.emplace_back(")");

              operator_stack.emplace_back();
            }

          else
            if(o == "[")
            {
              close_stack.emplace_back("]");

              operator_stack.emplace_back();
            }

          else
            if(o == "{")
            {
              close_stack.emplace_back("}");

              operator_stack.emplace_back();
            }

          else
            if(o == close_stack.back())
            {
              close_stack.pop_back();

              finish(std::move(operator_stack.back()),tmp);

              operator_stack.pop_back();
            }


          tmp.emplace_back(e);
        }

      else
        {
          report;
          return;
        }
    }


  finish(std::move(operator_stack.back()),tmp);

  m_stack = std::move(tmp);
/*
printf("\n");
print();
printf("\n");
*/
}


exprrpn&
exprrpn::
assign(token_iterator&  it)
{
  m_strings.clear();
  m_stack.clear();

   preprocess(it);
  postprocess();

  return *this;
}


exprrpn&
exprrpn::
assign(std::string_view  sv)
{
//  printf("%s\n",sv.data());

  token_block  blk(sv);

  token_iterator  it(blk);

  return assign(it);
};


exprelem
exprrpn::
evaluate(const exprrpn&  rpn) noexcept
{
  std::vector<exprelem>  buf;

  int  num_operands = 0;

    for(auto&  e: rpn.get_stack())
    {
        if(e.is_integer() || e.is_unsigned_integer() || e.is_real_number())
        {
          buf.emplace_back(e);

          ++num_operands;
        }

      else
        if(e.is_prefix_unary_operator())
        {
            if(num_operands < 1)
            {
              report;

              return {};
            }


          auto  op = e.get_operator_code();
          auto  o  = buf.back();

            if(o.is_real_number())
            {
                if(op == "-")
                {
                  o = exprelem(-o.get_real_number());
                }
            }

          else
            if(o.is_unsigned_integer())
            {
              o = exprelem(operate(o.get_unsigned_integer(),op));
            }

          else
            if(o.is_integer())
            {
              o = exprelem(operate(o.get_integer(),op));
            }


          buf.back() = o;
        }

      else
        if(e.is_postfix_unary_operator())
        {
            if(num_operands < 1)
            {
              report;

              return {};
            }


          auto  op = e.get_operator_code();
          auto  o  = buf.back();

            if(o.is_unsigned_integer())
            {
              o = exprelem(operate(o.get_unsigned_integer(),op));
            }

          else
            if(o.is_integer())
            {
              o = exprelem(operate(o.get_integer(),op));
            }


          buf.back() = o;
        }

      else
        if(e.is_binary_operator())
        {
            if(num_operands < 2)
            {
              report;

              return {};
            }


          auto  r = buf.back();

          buf.pop_back();

          auto  l = buf.back();

          buf.pop_back();

          auto  o = e.get_operator_code();

          exprelem  x;

            if((o == "==") ||
               (o == "!=") ||
               (o == "<" ) ||
               (o == "<=") ||
               (o == ">" ) ||
               (o == ">=") ||
               (o == "||") ||
               (o == "&&"))
            {
                if(l.is_real_number() || r.is_real_number())
                {
                  x = boperate(static_cast<double>(l),
                               static_cast<double>(r),o);
                }

              else
                if(l.is_unsigned_integer() || r.is_unsigned_integer())
                {
                  x = boperate(static_cast<uint64_t>(l),
                               static_cast<uint64_t>(r),o);
                }

              else
                {
                  x = boperate(static_cast<int64_t>(l),
                               static_cast<int64_t>(r),o);
                }
            }

          else
            {
                if(l.is_real_number() || r.is_real_number())
                {
                  x = operate(static_cast<double>(l),
                              static_cast<double>(r),o);
                }

              else
                if(l.is_unsigned_integer() || r.is_unsigned_integer())
                {
                  x = operate(static_cast<uint64_t>(l),
                              static_cast<uint64_t>(r),o);
                }

              else
                {
                  x = operate(static_cast<int64_t>(l),
                              static_cast<int64_t>(r),o);
                }
            }


          buf.emplace_back(x);

          --num_operands;
        }
    }


    if(buf.size() == 1)
    {
      return buf.back();
    }


  report;

  return exprelem();
}


void
exprrpn::
print() const noexcept
{
    for(auto&  e: m_stack)
    {
      e.print();

      printf(",");
    }
}




}




