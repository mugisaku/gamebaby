#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




namespace{
constexpr bool  isid0(int  c) noexcept
{
  return ((c >= 'a') && (c <= 'z')) ||
         ((c >= 'A') && (c <= 'Z')) ||
         ((c == '_')              );
}


constexpr bool  isdigit(int  c) noexcept
{return (c >= '0') && (c <= '9');}

constexpr bool  isidx(int  c) noexcept
{return isid0(c) || isdigit(c);}


const char
g_null = 0;


class
character_pointer
{
  const char*&  m_current;
  const char*   m_end;

public:
  constexpr character_pointer(const char*&  cur, const char*  end) noexcept:
  m_current(cur), m_end(end){}

  const char&  operator*() noexcept{return (*this)? *m_current:g_null;}
  operator bool() const noexcept{return m_current < m_end;}

  character_pointer&  operator++(   ) noexcept{  ++m_current;  return *this;}
  character_pointer   operator++(int) noexcept{  character_pointer  t(m_current,m_end);  ++m_current;  return t;}

  int  operator-(const char*  p) const noexcept{return m_current-p;}

  char  get() noexcept{return (*this)? *m_current:0;}

  void  skip_spaces() noexcept;

  std::string  read_name() noexcept;

  int  read_decimal_number() noexcept;

  int  read_number_of_elements() noexcept;

  parameter_list  read_parameter_list(type_collection&  tc) noexcept;

  struct_type_info  read_struct(type_collection&  tc) noexcept;
  union_type_info   read_union(type_collection&  tc) noexcept;
  enum_type_info    read_enum(type_collection&  tc) noexcept;

  const type_info*  read(type_collection&  tc, const type_info*  ti) noexcept;

};


void
character_pointer::
skip_spaces() noexcept
{
    while(*m_current == ' ')
    {
      ++m_current;
    }
}


std::string
character_pointer::
read_name() noexcept
{
  std::string  s;

  auto  c = *m_current;

    if(isid0(c))
    {
      s += c;

      ++m_current;

        for(;;)
        {
          c = *m_current;

            if(isidx(c))
            {
              s += c;

              ++m_current;
            }

          else
            {
              break;
            }
        }
    }


  return std::move(s);
}


int
character_pointer::
read_decimal_number() noexcept
{
  int  n = 0;

  skip_spaces();

  auto  c = *m_current;

    if(isdigit(c))
    {
        for(;;)
        {
          n *=    10;
          n += c-'0';

          c = *++m_current;

            if(!isdigit(c))
            {
              break;
            }
        }
    }

  else
    {
      printf("ERROR: number string is not found.");
    }


  return n;
}


int
character_pointer::
read_number_of_elements() noexcept
{
  auto  n = read_decimal_number();

  skip_spaces();

    if(*m_current == ']')
    {
      ++m_current;
    }

  else
    {
      printf("ERROR: number string is not closed by \']\'.");
    }


  return n;
}


parameter_list
character_pointer::
read_parameter_list(type_collection&  tc) noexcept
{
  parameter_list  ls;

  skip_spaces();

    for(;;)
    {
      auto  ti = tc.create_from_string(m_current,m_end);

        if(!ti)
        {
          break;
        }


      ls.push(*ti);
    }


  return std::move(ls);
}


struct_type_info
character_pointer::
read_struct(type_collection&  tc) noexcept
{
  struct_type_info  sti;

    for(;;)
    {
      skip_spaces();

      auto  c = get();

        if(c == '}')
        {
          break;
        }

      else
        if(isid0(c))
        {
          auto  ti = tc.create_from_string(m_current,m_end);

            if(ti)
            {
              skip_spaces();

                if(isid0(get()))
                {
                  auto  id = read_name();

                  sti.push(*ti,id);

                  skip_spaces();
                }


                if((get() == ',') ||
                   (get() == ';'))
                {
                  ++m_current;
                }
            }
        }

      else
        {
          printf("ERROR: struct");

          break;
        }
    }


  return std::move(sti);
}


union_type_info
character_pointer::
read_union(type_collection&  tc) noexcept
{
  union_type_info  uti;

    for(;;)
    {
      skip_spaces();

      auto  c = get();

        if(c == '}')
        {
          break;
        }

      else
        if(isid0(c))
        {
          auto  ti = tc.create_from_string(m_current,m_end);

            if(ti)
            {
              skip_spaces();

                if(isid0(get()))
                {
                  auto  id = read_name();

                  uti.push(*ti,id);

                  skip_spaces();
                }


                if((get() == ',') ||
                   (get() == ';'))
                {
                  ++m_current;
                }
            }
        }

      else
        {
          printf("ERROR: union");

          break;
        }
    }


  return std::move(uti);
}


enum_type_info
character_pointer::
read_enum(type_collection&  tc) noexcept
{
  enum_type_info  eti(8*tc.get_enum_size());

  int  next = 0;

    for(;;)
    {
      skip_spaces();

      auto  c = get();

        if(c == '}')
        {
          break;
        }

      else
        if(isid0(c))
        {
          auto  id = read_name();

          skip_spaces();

            if(get() == '=')
            {
              ++m_current;

              skip_spaces();

                if(isdigit(get()))
                {
                  next = read_decimal_number();
                }
            }


          eti.push(id,next++);

          skip_spaces();

            if((get() == ',') ||
               (get() == ';'))
            {
              ++m_current;
            }
        }

      else
        {
          printf("ERROR: enum");

          break;
        }
    }


  return std::move(eti);
}




const type_info*
character_pointer::
read(type_collection&  tc, const type_info*  ti) noexcept
{
    while(*this)
    {
      skip_spaces();

      auto  c = *m_current;

        if((c == ',') ||
           (c == ')'))
        {
          ++m_current;

          return ti;
        }

      else
        if(c == '*')
        {
          ++m_current;

          ti = &ti->get_derivation().get_pointer_type(8*tc.get_pointer_size());
        }

      else
        if(c == '&')
        {
          ++m_current;

          return &ti->get_derivation().get_reference_type(8*tc.get_pointer_size());
        }

      else
        if(c == '[')
        {
          ++m_current;

          ti = &ti->get_derivation().get_array_type(read_number_of_elements());
        }

      else
        if(c == '(')
        {
          ++m_current;

          ti = &ti->get_derivation().get_function_type(read_parameter_list(tc));
        }

      else
        {
          break;
        }
    }


  return ti;
}


}




const type_info*
type_collection::
create_from_string(const char*&  begin, const char*  end) noexcept
{
  character_pointer  p(begin,end);

  p.skip_spaces();

  auto  base_type_name = p.read_name();

    if(base_type_name == "struct")
    {
      p.skip_spaces();

        if(*p == '{')
        {
          ++p;

          return &push(std::make_unique<type_info>(p.read_struct(*this)));
        }
    }

  else
    if(base_type_name == "union")
    {
      p.skip_spaces();

        if(*p == '{')
        {
          ++p;

          return &push(std::make_unique<type_info>(p.read_union(*this)));
        }
    }

  else
    if(base_type_name == "enum")
    {
      p.skip_spaces();

        if(*p == '{')
        {
          ++p;

          return &push(std::make_unique<type_info>(p.read_enum(*this)));
        }
    }

  else
    {
      auto  ti = find_by_name(base_type_name);

        if(!ti)
        {
          return nullptr;
        }


      return p.read(*this,ti);
    }


  return nullptr;
}




}}




