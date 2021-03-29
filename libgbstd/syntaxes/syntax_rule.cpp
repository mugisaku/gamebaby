#include"libgbstd/syntax.hpp"




namespace gbstd{




void
syntax_rule::
start_read(bool  key, code_text::iterator&  it)
{
  auto  name = read_identifier(it);

  auto  def = find(name);

    if(!def)
    {
      def = &m_definition_list.emplace_back(false,name);
    }


  it.skip_spaces();

  auto  c = *it;

    if(c == '=')
    {
      ++it;

        if(key)
        {
          def->set_key();
        }


      read_expression(*def,';',it);
    }

  else
    if(c == ';')
    {
      ++it;
    }

  else
    {
      ++it;

      report;
    }
}


std::u16string
syntax_rule::
read_identifier(code_text::iterator&  it)
{
  std::u16string  s;

    while(*it)
    {
      auto  c = *it;

        if(is_alphabet(c) || (c == '_'))
        {
          s += c;

          ++it;
        }

      else
        {
          break;
        }
    }


  return std::move(s);
}


std::u16string
syntax_rule::
read_string(code_text::iterator&  it)
{
  std::u16string  s({*it});

  ++it;

    while(*it)
    {
      auto  c = *it;

        if(c == '\"')
        {
          ++it;

          break;
        }

      else
        if(c == '\\')
        {
          ++it;

          c = *it;

               if(c ==  'n'){s += '\n';}
          else if(c ==  'r'){s += '\r';}
          else if(c ==  't'){s += '\t';}
          else if(c == '\\'){s += '\\';}
          else if(c ==  '0'){s += '\0';}
          else if(c ==  '\"'){s += '\"';}
          else if(c ==  '\''){s += '\'';}
          else{report;}

          ++it;
        }

      else
        {
          ++it;

          s += c;
        }
    }


  return std::move(s);
}


void
syntax_rule::
read_expression_internal(syntax_expression&  expr, code_text::iterator&  it)
{
  auto  c = *it;

    if((c == '|') ||
       (c == '&') ||
       (c == ':'))
    {
      char  s[2] = {static_cast<char>(c),0};

      expr.push(small_string(s));

      ++it;

      it.skip_spaces();

      c = *it;
    }


    if(c == '\"')
    {
      expr.push(read_string(++it));
    }

  else
    if(c == '(')
    {
      small_string  ss("()");

      syntax_expression  coexpr;

      read_expression(coexpr,')',++it);

      syntax_expression_element  e(std::move(coexpr));

      expr.push(std::move(e));

      expr.push(ss);
    }

  else
    if(c == '[')
    {
      small_string  ss("[]");

      syntax_expression  coexpr;

      read_expression(coexpr,']',++it);

      syntax_expression_element  e(std::move(coexpr));

      expr.push(std::move(e));

      expr.push(ss);
    }

  else
    if(c == '{')
    {
      small_string  ss("{}");

      syntax_expression  coexpr;

      read_expression(coexpr,'}',++it);

      syntax_expression_element  e(std::move(coexpr));

      expr.push(std::move(e));

      expr.push(ss);
    }

  else
    {
      auto  name = read_identifier(it);

      auto  def = find(name);

        if(!def)
        {
          def = &m_definition_list.emplace_back(false,name);
        }


      expr.push(syntax_expression_element(*def));
    }
}


void
syntax_rule::
read_expression(syntax_expression&  expr, int  close, code_text::iterator&  it)
{
    for(;;)
    {
      it.skip_spaces();

      auto  c = *it;

        if(c == close)
        {
          ++it;

          break;
        }

      else
        if(!c)
        {
          report;

          break;
        }

      else
        {
            try
            {
              read_expression_internal(expr,it);
            }


            catch(syntax_expression_error&  e)
            {
              it.print();

              report;
              break;
            }
        }
    }
}


syntax_rule&
syntax_rule::
assign(std::u16string_view  sv) noexcept
{
  m_definition_list.clear();

  return append(sv);
}


syntax_rule&
syntax_rule::
append(std::u16string_view  sv) noexcept
{
  code_text::iterator  it(sv.data());

  it.skip_spaces();

    while(*it)
    {
      auto  c = *it;

      bool  key = false;

        if(c == '*')
        {
          key = true;

          c = *++it;
        }


        if(is_alphabet(c) || (c == '_'))
        {
          start_read(key,it);

          it.skip_spaces();
        }

      else
        {
          printf("<%c>\n",c);

          report;
          break;
        }
    }


  return *this;
}


syntax_definition*
syntax_rule::
find(std::u16string_view  name) noexcept
{
    for(auto&  def: m_definition_list)
    {
        if(def.get_name() == name)
        {
          return &def;
        }
    }


  return nullptr;
}


const syntax_definition*
syntax_rule::
find(std::u16string_view  name) const noexcept
{
    for(auto&  def: m_definition_list)
    {
        if(def.get_name() == name)
        {
          return &def;
        }
    }


  return nullptr;
}


void
syntax_rule::
print() const noexcept
{
    for(auto&  def: m_definition_list)
    {
      def.print();

      printf("\n");
    }
}




}




