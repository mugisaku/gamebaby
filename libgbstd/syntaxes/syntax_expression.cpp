#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_expression&
syntax_expression::
assign(const syntax_expression&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_code = rhs.m_code;

      m_left  = rhs.m_left ? std::make_unique<syntax_expression_element>(*m_left ):nullptr;
      m_right = rhs.m_right? std::make_unique<syntax_expression_element>(*m_right):nullptr;
    }


  return *this;
}


syntax_expression&
syntax_expression::
assign(syntax_expression&&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_code = rhs.m_code;

      m_left  = std::move(rhs.m_left );
      m_right = std::move(rhs.m_right);
    }


  return *this;
}


syntax_expression&
syntax_expression::
assign(small_string  c, pointer  l, pointer  r) noexcept
{
  m_code = c;

  m_left  = std::move(l);
  m_right = std::move(r);

  return *this;
}




std::unique_ptr<syntax_expression_element>
syntax_expression::
pack_self() noexcept
{
  return std::make_unique<syntax_expression_element>(std::move(*this));
}


void
syntax_expression::
push(syntax_expression_element&&  e)
{
    if(!m_left && !m_code)
    {
      m_left = std::make_unique<syntax_expression_element>(std::move(e));
    }

  else
    if(m_left && m_code && !m_right)
    {
      m_right = std::make_unique<syntax_expression_element>(std::move(e));
    }

  else
    {
      throw syntax_expression_error();
    }
}


void
syntax_expression::
push(small_string  c)
{
    if((c == "()") ||
       (c == "[]") ||
       (c == "{}"))
    {
        if(m_left && m_code && m_right)
        {
          m_left = pack_self();

          m_code = c;
        }

      else
        if(m_left && !m_right)
        {
            if(m_code)
            {
              m_left = pack_self();
            }


          m_code = c;
        }

      else
        {
          throw syntax_expression_error();
        }
    }

  else
    if((c == "|") ||
       (c == "&") ||
       (c == ":"))
    {
        if(m_left && m_code && m_right)
        {
          m_left = pack_self();

          m_code = c;
        }

      else
        if(m_left && m_code)
        {
          m_left = pack_self();

          m_code = c;
        }

      else
        if(m_left && !m_code && !m_right)
        {
          m_code = c;
        }

      else
        {
          throw syntax_expression_error();
        }
    }

  else
    {
      throw syntax_expression_error();
    }
}




void
syntax_expression::
print_for_test() const noexcept
{
    if(m_left ){printf("has left\n");}
    if(m_code ){printf("has code\n");}
    if(m_right){printf("has right\n");}
}


void
syntax_expression::
print() const noexcept
{
       if(m_code == "()"){printf("(");}
  else if(m_code == "[]"){printf("[");}
  else if(m_code == "{}"){printf("{");}

    if(m_left)
    {
      m_left->print();

        if(m_right)
        {
          printf(" ");

          m_code.print();

          printf(" ");

          m_right->print();
        }
    }

       if(m_code == "()"){printf(")");}
  else if(m_code == "[]"){printf("]");}
  else if(m_code == "{}"){printf("}");}
}


}




