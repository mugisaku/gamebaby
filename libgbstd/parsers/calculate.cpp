#include"parser.hpp"
#include<memory>



namespace gbstd{




namespace{


struct
context
{
  token_block_view  m_view;

  context() noexcept{}
  context(const token_block&  blk) noexcept: m_view(blk){}

};


constexpr int
get_precedence(operator_code  o) noexcept
{
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

report;
  return 0;
}


template<typename  T>
T
operate(T  l, T  r, operator_code  o) noexcept
{
  return (o == "+" )? l+r
        :(o == "-" )? l-r
        :(o == "*" )? l*r
        :(o == "/" )? l/r
        :(o == "%" )? l%r
        :(o == "<<")? l<<r
        :(o == ">>")? l>>r
        :(o == "==")? (l == r? 1:0)
        :(o == "!=")? (l != r? 1:0)
        :(o == "<" )? (l <  r? 1:0)
        :(o == "<=")? (l <= r? 1:0)
        :(o == ">=")? (l >= r? 1:0)
        :(o == "|" )? l|r
        :(o == "&" )? l&r
        :(o == "^" )? l^r
        :(o == "||")? (l || r? 1:0)
        :(o == "&&")? (l && r? 1:0)
        :0
        ;
}


double
operate(double  l, double  r, operator_code  o) noexcept
{
  return (o == "+" )? l+r
        :(o == "-" )? l-r
        :(o == "*" )? l*r
        :(o == "/" )? l/r
        :0
        ;
}


template<typename  T>
int64_t
boperate(T  l, T  r, operator_code  o) noexcept
{
  return (o == "==")? (l == r? 1:0)
        :(o == "!=")? (l != r? 1:0)
        :(o == "<" )? (l <  r? 1:0)
        :(o == "<=")? (l <= r? 1:0)
        :(o == ">=")? (l >= r? 1:0)
        :(o == "||")? (l || r? 1:0)
        :(o == "&&")? (l && r? 1:0)
        :0
        ;
}


int64_t
boperate(double  l, double  r, operator_code  o) noexcept
{
  return (o == "==")? (l == r? 1:0)
        :(o == "!=")? (l != r? 1:0)
        :(o == "<" )? (l <  r? 1:0)
        :(o == "<=")? (l <= r? 1:0)
        :(o == ">=")? (l >= r? 1:0)
        :0
        ;
}


}




exprelem
calculate(std::string_view  sv) noexcept
{
  token_block  blk(sv);

  std::vector<context>  stack({blk});

  std::vector<exprelem>    output_stack;
  std::vector<exprelem>  operator_stack;

  auto  ctx = &stack.back();

  enum class kind{
    null,operand,unary,binary,
  } last_kind=kind::null;


    for(;;)
    {
        while(ctx->m_view)
        {
          auto&  bv = ctx->m_view;

            if(bv->is_integer())
            {
              output_stack.emplace_back(bv++->get_integer());

              last_kind = kind::operand;
            }

          else
            if(bv->is_floating_point_number())
            {
              output_stack.emplace_back(bv++->get_floating_point_number());

              last_kind = kind::operand;
            }

          else
            if(bv->is_operator_code())
            {
              auto  op = bv++->get_operator_code();

                if(last_kind == kind::null)
                {
                  operator_stack.emplace_back('U',op);

                  last_kind = kind::unary;
                }

              else
                if(last_kind == kind::unary)
                {
                  output_stack.emplace_back(operator_stack.back());

                  operator_stack.pop_back();

                  operator_stack.emplace_back('U',op);
                }

              else
                if(last_kind == kind::operand)
                {
                    while(operator_stack.size())
                    {
                      auto  prev_op = operator_stack.back().get_operator_code();

                        if(get_precedence(op) > get_precedence(prev_op))
                        {
                          break;
                        }


                      output_stack.emplace_back(operator_stack.back());

                      operator_stack.pop_back();
                    }

                  
                  operator_stack.emplace_back('B',op);
                }
            }

          else
            if(bv->is_block("(",")"))
            {
              stack.emplace_back(bv++->get_block());

              ctx = &stack.back();
            }

          else
            {
              report;

              return {};
            }
        }


        if(stack.size() > 1)
        {
          stack.pop_back();

          ctx = &stack.back();
        }

      else
        {
          break;
        }
    }


    while(operator_stack.size())
    {
      output_stack.emplace_back(operator_stack.back());

      operator_stack.pop_back();
    }


  std::vector<exprelem>  buf;

  int  num_operands = 0;

    for(auto&  e: output_stack)
    {
        if(e.is_integer() || e.is_unsigned_integer() || e.is_real_number())
        {
          buf.emplace_back(e);

          ++num_operands;
        }

      else
        if(e.is_unary_operator())
        {
            if(num_operands < 1)
            {
              report;

              return {};
            }
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




}




