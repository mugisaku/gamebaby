#include"libgbstd/dn.hpp"
#include"libgbstd/character.hpp"
#include<cinttypes>




namespace gbstd{




dn_object&
dn_object::
assign(syntax_token_iterator&  it) noexcept
{
  clear();

  it.skip();

  const std::u16string*  name = nullptr;

    if((it->is_identifier() || it->is_string()))
    {
      auto&  tok = *it++;

      it.skip();

        if(it->is_others(u':'))
        {
          name = &tok.string();

          ++it;

          it.skip();
        }

      else
        {
               if(tok.is_identifier(u"null") ){}
          else if(tok.is_identifier(u"true") ){assign(true);}
          else if(tok.is_identifier(u"false")){assign(false);}
          else
            {assign(tok.string());}

          return *this;
        }
    }


  auto&  tok = *it++;

       if(tok.is_identifier(u"null")){}
  else if(tok.is_identifier(u"true")){assign(true);}
  else if(tok.is_identifier(u"false")){assign(false);}
  else if(tok.is_string() || tok.is_identifier()){assign(tok.string());}
  else if(tok.is_integer()){assign(tok.integer());}
  else if(tok.is_floating()){assign(tok.floating());}
  else if(tok.is_others(u'{')){assign(dn_list(it));}
  else if(tok.is_others(u'[')){assign(dn_array(it));}
  else if(tok.is_others(u'-'))
    {
           if(it->is_integer() ){assign(-static_cast<int64_t>(it++->integer()));}
      else if(it->is_floating()){assign(-it++->floating());}
      else
        {
          report;

          printf("[dn_object assign error]");

          tok.print();
        }
    }

  else
    {
      printf("[dn_object assign error]");

      tok.print();
    }


    if(name)
    {
      m_name = *name;
    }


  return *this;
}


dn_object&
dn_object::
assign(bool   b) noexcept
{
  clear();

  new(&m_data) bool(b);

  m_kind = kind::boolean;

  return *this;
}


dn_object&
dn_object::
assign(int64_t  i) noexcept
{
  clear();

  new(&m_data) int64_t(i);

  m_kind = kind::integer;

  return *this;
}


dn_object&
dn_object::
assign(uint64_t  u) noexcept
{
  clear();

  new(&m_data) uint64_t(u);

  m_kind = kind::unsigned_integer;

  return *this;
}


dn_object&
dn_object::
assign(double  f) noexcept
{
  clear();

  new(&m_data) double(f);

  m_kind = kind::floating;

  return *this;
}


dn_object&
dn_object::
assign(std::u16string_view  sv) noexcept
{
  clear();

  new(&m_data) std::u16string(sv);

  m_kind = kind::string;

  return *this;
}


dn_object&
dn_object::
assign(dn_array&&  arr) noexcept
{
  clear();

  m_data.arr = new dn_array(std::move(arr));

  m_kind = kind::array;

  return *this;
}


dn_object&
dn_object::
assign(dn_list&&  ls) noexcept
{
  clear();

  m_data.ls = new dn_list(std::move(ls));

  m_kind = kind::list;

  return *this;
}


dn_object&
dn_object::
assign(const dn_object&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;
      m_name = rhs.m_name;

        switch(m_kind)
        {
      case(kind::null): break;
      case(kind::undefined): break;
      case(kind::boolean): m_data.b = rhs.m_data.b;break;
      case(kind::integer): m_data.i = rhs.m_data.i;break;
      case(kind::unsigned_integer): m_data.u = rhs.m_data.u;break;
      case(kind::floating): m_data.f = rhs.m_data.f;break;
      case(kind::string ): new(&m_data) std::u16string(rhs.m_data.s);break;
      case(kind::array): m_data.arr = new dn_array(*rhs.m_data.arr);break;
      case(kind::list): m_data.ls = new dn_list(*rhs.m_data.ls);break;
        }
    }


  return *this;
}


dn_object&
dn_object::
assign(dn_object&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);
      std::swap(m_name,rhs.m_name);

        switch(m_kind)
        {
      case(kind::null): break;
      case(kind::undefined): break;
      case(kind::boolean  ): m_data.b = rhs.m_data.b;break;
      case(kind::integer  ): m_data.i = rhs.m_data.i;break;
      case(kind::unsigned_integer  ): m_data.u = rhs.m_data.u;break;
      case(kind::floating ): m_data.f = rhs.m_data.f;break;
      case(kind::string): new(&m_data) std::u16string(std::move(rhs.m_data.s));break;
      case(kind::array    ): m_data.arr = rhs.m_data.arr;break;
      case(kind::list     ): m_data.ls = rhs.m_data.ls;break;
        }
    }


  return *this;
}




void
dn_object::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::null): break;
  case(kind::undefined): break;
  case(kind::boolean): break;
  case(kind::integer): break;
  case(kind::unsigned_integer): break;
  case(kind::floating): break;
  case(kind::string ): std::destroy_at(&m_data.s);break;
  case(kind::array): delete m_data.arr;break;
  case(kind::list): delete m_data.ls;break;
    }


  m_kind = kind::null;

  m_name.clear();
}


dn_object&
dn_object::
set_name(std::u16string_view  name) noexcept
{
  m_name = name;

  return *this;
}


void
dn_object::
print() const noexcept
{
    if(has_name())
    {
      printf("\"");

      gbstd::print(m_name);

      printf("\":");
    }


    switch(m_kind)
    {
  case(kind::undefined): printf("undefined");break;
  case(kind::null): printf("null");break;
  case(kind::boolean): printf("%s",m_data.b? "true":"false");break;
  case(kind::integer): printf("%" PRIi64,m_data.i);break;
  case(kind::unsigned_integer): printf("%" PRIu64,m_data.u);break;
  case(kind::floating): printf("%f",m_data.f);break;
  case(kind::string ):
    printf("\"");
    gbstd::print(m_data.s);
    printf("\"");
    break;
  case(kind::list): m_data.ls->print();break;
  case(kind::array): m_data.arr->print();break;
  default: printf("unknown");break;
    }
}




dn_object
make_object_from_string(std::u16string_view  sv) noexcept
{
  auto  toks = make_token_string(sv);

  syntax_token_iterator  it(toks);

  return dn_object(it);
}




}




