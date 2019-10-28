#include"libgbstd/gpfs.hpp"




namespace gbstd{
namespace gpfs{




namespace{
class
getter
{
  const char*  m_current;
  const char*  m_end;

public:
  getter(std::string_view  sv) noexcept: m_current(sv.data()), m_end(sv.data()+sv.size()){}

  operator bool() const noexcept{return m_current < m_end;}

  std::string  read_chars() noexcept
  {
    std::string  s;

    auto  c = get();

      if(!c)
      {
      }

    else
      if(c == '.')
      {
        advance();

        s += '.';

          while(get() == '.')
          {
            s += '.';

            advance();
          }
      }

    else
      {
        s += c;

        advance();

          for(;;)
          {
            c = get();

              if(!c || (c == '/'))
              {
                break;
              }


            s += c;

            advance();
          }
      }


    return std::move(s);
  }

  std::string  read0(bool&  abs) noexcept
  {
    auto  c = get();

      if(!c)
      {
        return std::string();
      }


      if(c == '/')
      {
        abs = true;

        advance();

        skip('/');
      }


    return read_chars();
  }

  std::string  read1() noexcept
  {
    auto  c = get();

      if(!c)
      {
        return std::string();
      }


      if(c == '/')
      {
        advance();

        skip('/');
      }


    return read_chars();
  }

  void  skip(char  c) noexcept
  {
      while(*m_current == c)
      {
        ++m_current;
      }
  }

  char  get() noexcept{return (*this)? *m_current:0;}

  void  advance() noexcept{++m_current;}

};
}




path&
path::
assign(std::string_view  sv) noexcept
{
  m_data.clear();

  append(sv);

  return *this;
}


path&
path::
append(std::string_view  sv) noexcept
{
  m_absolute = false;

  getter  g(sv);

  auto  s = g.read0(m_absolute);

    if(s.size())
    {
      m_data.emplace_back(std::move(s));

        for(;;)
        {
          s = g.read1();

            if(s.empty())
            {
              break;
            }


          m_data.emplace_back(std::move(s));
        }
    }


  return *this;
}


void
path::
print() const noexcept
{
  auto      it = begin();
  auto  end_it =   end();

    if(m_absolute)
    {
      printf("/");
    }


    if(it != end_it)
    {
      printf("%s",it++->data());

        while(it != end_it)
        {
          printf("/%s",it++->data());
        }
    }
}


bool
path::
test(std::string_view  sv) noexcept
{
  auto      it = sv.begin();
  auto  end_it = sv.end();

    if(it != end_it)
    {
      auto  c = *it++;

        if((c != '.') &&
           (c != '/'))
        {
            while(it != end_it)
            {
              c = *it++;

                if(c == '/')
                {
                  return false;
                }
            }


          return(*--it != '.');
        }
    }


  return false;
}




}}




