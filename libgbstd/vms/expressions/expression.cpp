#include"libgbstd/vms/expression.hpp"




namespace gbstd{




struct
expression::
data
{
  std::vector<expression_element>  m_elements;

};




expression&
expression::
assign(const expression&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

        if(rhs.m_data)
        {
          m_data = new data(*rhs.m_data);
        }
    }


  return *this;
}


expression&
expression::
assign(expression&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}


expression&
expression::
assign(std::vector<expression_element>&&  els) noexcept
{
    if(!m_data)
    {
      m_data = new data;
    }


  m_data->m_elements = std::move(els);

  return *this;
}




expression&
expression::
append(expression_element&&  els) noexcept
{
    if(!m_data)
    {
      m_data = new data;
    }


  m_data->m_elements.emplace_back(std::move(els));

  return *this;
}




expression&
expression::
clear() noexcept
{
  delete m_data          ;
         m_data = nullptr;

  return *this;
}


std::vector<expression_element>&
expression::
get_elements() noexcept
{
  return m_data->m_elements;
}


simple_value
expression::
evaluate() const noexcept
{
  simple_value  v;

  auto&  els = m_data->m_elements;

  auto    p = els.data();
  auto  end = els.data()+els.size();

    if(p != end)
    {
      v = p++->get_expression().evaluate();

        while(p != end)
        {
          auto  opco = p->get_operator_code();
          auto  rv   = p++->get_expression().evaluate();

               if(opco == operator_code("+") ){v = v.get_integer()+rv.get_integer();}
          else if(opco == operator_code("-") ){v = v.get_integer()-rv.get_integer();}
          else if(opco == operator_code("*") ){v = v.get_integer()*rv.get_integer();}
          else if(opco == operator_code("/") ){v = v.get_integer()/rv.get_integer();}
          else if(opco == operator_code("%") ){v = v.get_integer()%rv.get_integer();}
          else if(opco == operator_code("<<")){v = v.get_integer()<<rv.get_integer();}
          else if(opco == operator_code(">>")){v = v.get_integer()>>rv.get_integer();}
          else if(opco == operator_code("|") ){v = v.get_integer()|rv.get_integer();}
          else if(opco == operator_code("&") ){v = v.get_integer()&rv.get_integer();}
          else if(opco == operator_code("^") ){v = v.get_integer()^rv.get_integer();}
          else if(opco == operator_code("==")){v = v.get_integer()==rv.get_integer();}
          else if(opco == operator_code("!=")){v = v.get_integer()!=rv.get_integer();}
          else if(opco == operator_code("<") ){v = v.get_integer()<rv.get_integer();}
          else if(opco == operator_code("<=")){v = v.get_integer()<=rv.get_integer();}
          else if(opco == operator_code(">") ){v = v.get_integer()>rv.get_integer();}
          else if(opco == operator_code(">=")){v = v.get_integer()>=rv.get_integer();}
          else if(opco == operator_code("||")){v = v.get_integer()||rv.get_integer();}
          else if(opco == operator_code("&&")){v = v.get_integer()&&rv.get_integer();}
        }
    }


  return v;
}


void
expression::
print() const noexcept
{
    if(m_data)
    {
        for(auto&  e: m_data->m_elements)
        {
          e.print();
        }
    }
}




operand
read_operand(token_iterator&  it)
{
    if(it->is_integer())
    {
      return static_cast<int64_t>(it++->get_integer());
    }

  else
    if(it->is_floating_point_number())
    {
      return it++->get_floating_point_number();
    }

  else
    if(it->is_single_quoted() ||
       it->is_double_quoted())
    {
      return std::string_view(it++->get_string());
    }

  else
    if(it->is_identifier())
    {
      auto&  s = it++->get_string();

        if(s == std::string_view("nullptr"))
        {
          return nullptr;
        }

      else
        if(s == std::string_view("false"))
        {
          return false;
        }

      else
        if(s == std::string_view("true"))
        {
          return true;
        }

      else
        {
          return identifier(s);
        }
    }

  else
    if(it->is_operator_code())
    {
        if(it->is_operator_code("("))
        {
          return read_expression(++it,")");
        }
    }


  throw expression_error();
}


std::vector<expression>
convert_to_expression_list(expression&&  src)
{
  std::vector<expression>  ls;

    if(src)
    {
      auto&  srcls = src.get_elements();

      auto    p = srcls.data();
      auto  end = srcls.data()+srcls.size();

      std::vector<expression_element>  tmpls;

        while(p != end)
        {
          auto&  el = *p++;

            if(el.get_operator_code() == operator_code(","))
            {
              ls.emplace_back(std::move(tmpls));

              tmpls = std::vector<expression_element>();

              el.set_operator_code("");
            }


          tmpls.emplace_back(std::move(el));
        }


        if(tmpls.size())
        {
          ls.emplace_back(std::move(tmpls));
        }
    }


  return std::move(ls);
}


primary_expression
read_primary_expression(token_iterator&  it)
{
  operand  o = read_operand(it);

  std::vector<primary_expression_element>  els;

    for(;;)
    {
        if(it->is_operator_code("("))
        {
          auto  e = read_expression(++it,")");

          auto  ls = convert_to_expression_list(std::move(e));

          els.emplace_back(std::move(ls));
        }

      else
        if(it->is_operator_code("["))
        {
          els.emplace_back(read_expression(++it,"]"));
        }

      else
        if(it->is_operator_code(".") ||
           it->is_operator_code("->"))
        {
          ++it;

            if(it->is_identifier())
            {
              els.emplace_back(std::string_view(it++->get_string()));
            }
        }

      else
        {
          break;
        }
    }


  return primary_expression(std::move(o),std::move(els));
}


operator_code
read_unary_operator(token_iterator&  it)
{
    if(it->is_operator_code())
    {
      auto  opco = it->get_operator_code();

        if((opco == "+") ||
           (opco == "-") ||
           (opco == "*") ||
           (opco == "!") ||
           (opco == "~") ||
           (opco == "&"))
        {
          ++it;

          return opco;
        }
    }


  return "";
}


unary_expression
read_unary_expression(token_iterator&  it)
{
  std::vector<operator_code>  stack;

    for(;;)
    {
      auto  op = read_unary_operator(it);

        if(!op)
        {
          break;
        }


      stack.emplace_back(op);
    }


  return unary_expression(std::move(stack),read_primary_expression(it));
}


operator_code
read_binary_operator(token_iterator&  it)
{
    if(it->is_operator_code())
    {
      auto  opco = it->get_operator_code();

        if((opco == "+") ||
           (opco == "-") ||
           (opco == "*") ||
           (opco == "/") ||
           (opco == "%") ||
           (opco == "<<") ||
           (opco == ">>") ||
           (opco == "==") ||
           (opco == "!=") ||
           (opco == "<") ||
           (opco == "<=") ||
           (opco == ">") ||
           (opco == ">=") ||
           (opco == "|") ||
           (opco == "&") ||
           (opco == "^") ||
           (opco == "=") ||
           (opco == "+=") ||
           (opco == "-=") ||
           (opco == "*=") ||
           (opco == "/=") ||
           (opco == "%=") ||
           (opco == "<<=") ||
           (opco == ">>=") ||
           (opco == "|=") ||
           (opco == "&=") ||
           (opco == "^=") ||
           (opco == "||") ||
           (opco == "&&") ||
           (opco == ",") ||
           (opco == "?") ||
           (opco == ":"))
        {
          ++it;

          return opco;
        }
    }


  return "";
}



expression
read_expression(token_iterator&  it, operator_code  close_code)
{
    if(!close_code)
    {
      throw expression_error("close code is none.");
    }


  std::vector<expression_element>  els;

    if(it->is_operator_code(close_code))
    {
      ++it;

      goto QUIT;
    }


  els.emplace_back(read_unary_expression(it));

    for(;;)
    {
        if(it->is_operator_code(close_code))
        {
          ++it;

          goto QUIT;
        }


      auto  op = read_binary_operator(it);

        if(!op)
        {
          it->print();

          break;
        }


      els.emplace_back(op,read_unary_expression(it));
    }


    if(close_code)
    {
      throw expression_error("閉じコードが出現しないまま終端に達した");
    }

QUIT:
  return expression(std::move(els));
}


expression
read_expression(std::string_view  sv, operator_code  close_code)
{
  auto  ts = make_token_string(sv);

  token_iterator  it(ts);

  return read_expression(it,close_code);
}




}




