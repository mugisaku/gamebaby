#include"libgbstd/syntax.hpp"




namespace gbstd{




void
syntax_rule::
start_read(text_iterator&  it)
{
  auto  name = read_identifier(it);

  auto  def = find(name);

    if(!def)
    {
      def = &m_definition_list.emplace_back(name);
    }


  it.skip_spaces();

  auto  c = *it.get().data();

    if(c == '=')
    {
      it.advance();

      read_group(*def,';',it);
    }

  else
    if(c == ';')
    {
      it.advance();
    }

  else
    {
      it.advance();

      report;
    }
}


std::string
syntax_rule::
read_identifier(text_iterator&  it)
{
  std::string  s;

    while(it)
    {
      auto  c = *it.get().data();

        if(is_alphabet(c) || (c == '_'))
        {
          s += c;

          it.advance();
        }

      else
        {
          break;
        }
    }


  return std::move(s);
}


std::string
syntax_rule::
read_string(text_iterator&  it)
{
  std::string  s(it.get());

  it.advance();

    while(it)
    {
      auto  c = *it.get().data();

        if(c == '\"')
        {
          it.advance();

          break;
        }

      else
        if(c == '\\')
        {
          it.advance();

          c = *it.get().data();

               if(c ==  'n'){s += '\n';}
          else if(c ==  'r'){s += '\r';}
          else if(c ==  't'){s += '\t';}
          else if(c == '\\'){s += '\\';}
          else if(c ==  '0'){s += '\0';}
          else if(c ==  '\"'){s += '\"';}
          else if(c ==  '\''){s += '\'';}
          else{report;}

          it.advance();
        }

      else
        {
          it.advance();

          s += c;
        }
    }


  return std::move(s);
}


void
syntax_rule::
read_group(syntax_group&  grp, int  close, text_iterator&  it)
{
  int  sep = 0;

  bool  is_first_time = true;

    for(;;)
    {
      it.skip_spaces();

      auto  c = *it.get().data();

        if(c == close)
        {
          it.advance();

          break;
        }

      else
        if(!c)
        {
          report;

          break;
        }

      else
        if(is_first_time)
        {
          is_first_time = false;
        }

      else
        if((c == '|') || (c == '&'))
        {
          it.advance();

          it.skip_spaces();

          sep = c                   ;
                c = *it.get().data();
        }


        if(c == '\"')
        {
          it.advance();

          grp.join(sep,read_string(it));
        }

      else
        if(c == '(')
        {
          syntax_group  cogrp(c);

          it.advance();

          read_group(cogrp,')',it);

          grp.join(sep,std::move(cogrp));
        }

      else
        if(c == '[')
        {
          syntax_group  cogrp(c);

          it.advance();

          read_group(cogrp,']',it);

          grp.join(sep,std::move(cogrp));
        }

      else
        if(c == '{')
        {
          syntax_group  cogrp(c);

          it.advance();

          read_group(cogrp,'}',it);

          grp.join(sep,std::move(cogrp));
        }

      else
        {
          auto  name = read_identifier(it);

          auto  def = find(name);

            if(!def)
            {
              def = &m_definition_list.emplace_back(name);
            }


          grp.join(sep,*def);
        }
    }
}


syntax_rule&
syntax_rule::
assign(std::string_view  sv) noexcept
{
  m_definition_list.clear();

  return append(sv);
}


syntax_rule&
syntax_rule::
append(std::string_view  sv) noexcept
{
  text_iterator  it(sv);

  it.skip_spaces();

    while(it)
    {
      auto  sv = *it;

      auto  c = *sv.data();

        if(is_alphabet(c) || (c == '_'))
        {
          start_read(it);

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
find(std::string_view  name) noexcept
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
find(std::string_view  name) const noexcept
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




