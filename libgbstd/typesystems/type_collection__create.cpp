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


class
character_pointer
{
  const char*  m_current=nullptr;
  const char*  m_end=nullptr;

public:
  constexpr character_pointer(const char*  cur, const char*  end) noexcept:
  m_current(cur), m_end(end){}

  constexpr character_pointer(std::string_view  sv) noexcept:
  m_current(sv.data()), m_end(sv.data()+sv.size()){}

  const char&  operator*() noexcept{return *m_current;}
  operator bool() const noexcept{return m_current < m_end;}

  character_pointer&  operator++(   ) noexcept{  ++m_current;  return *this;}
  character_pointer   operator++(int) noexcept{  character_pointer  t(m_current++,m_end);  return t;}

  int  operator-(const char*  p) const noexcept{return m_current-p;}

  void  skip_spaces() noexcept;

  std::string  read_name() noexcept;

  int  read_number_of_elements() noexcept;

  parameter_list  read_parameter_list(const type_collection&  tc) noexcept;

  const type_info*  read(const type_collection&  tc, const type_info*  ti) noexcept;

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
read_number_of_elements() noexcept
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
read_parameter_list(const type_collection&  tc) noexcept
{
  parameter_list  ls;

  skip_spaces();

    for(;;)
    {
      int  n;

      auto  ti = tc.create_from_string(std::string_view(m_current,m_end-m_current),&n);

        if(!ti)
        {
          break;
        }


      ls.push(*ti);

      m_current += n;
    }


  return std::move(ls);
}


const type_info*
character_pointer::
read(const type_collection&  tc, const type_info*  ti) noexcept
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

          ti = &ti->get_derivation().get_pointer_type();
        }

      else
        if(c == '&')
        {
          ++m_current;

          return &ti->get_derivation().get_reference_type();
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
create_from_string(std::string_view  sv, int*  n) const noexcept
{
  const char*  start = sv.data();

  character_pointer  p(sv);

  p.skip_spaces();

  auto  base_type_name = p.read_name();

  auto  ti = find_by_name(base_type_name);

    if(!ti)
    {
      return nullptr;
    }


  ti = p.read(*this,ti);

    if(n)
    {
      *n = p-start;
    }


  return ti;
}




}}




