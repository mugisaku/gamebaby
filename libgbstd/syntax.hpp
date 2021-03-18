#ifndef gbstd_syntax_HPP
#define gbstd_syntax_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/character.hpp"
#include<list>


namespace gbstd{




class syntax_element;
class syntax_group;
class syntax_definition;


class
syntax_group
{
  enum class kind{
    once,
    optional,
    multiple,
  } m_kind=kind::once;

  std::vector<syntax_element>  m_elements;

public:
  syntax_group(int  open=0) noexcept:
  m_kind( (open == '[')? kind::optional
         :(open == '{')? kind::multiple
         :               kind::once){}

  template<class... Args>
  syntax_group&  join(Args&&...  args) noexcept{  m_elements.emplace_back(std::forward<Args>(args)...);  return *this;}

  bool  is_once()     const noexcept{return m_kind == kind::once    ;}
  bool  is_optional() const noexcept{return m_kind == kind::optional;}
  bool  is_multiple() const noexcept{return m_kind == kind::multiple;}

  void  print() const noexcept;

};


class
syntax_element
{
  int  m_separator=0;

  enum class kind{
    null, string, group, definition,
  } m_kind=kind::null;

  union data{
    std::string    str;
    syntax_group   grp;
    const syntax_definition*  def;

  data() noexcept{}
 ~data()         {}

  } m_data;

public:
  syntax_element() noexcept{}
  syntax_element(const syntax_element&   rhs) noexcept{assign(rhs);}
  syntax_element(      syntax_element&&  rhs) noexcept{assign(std::move(rhs));}
 ~syntax_element(){clear();}

  template<class... Args>
  syntax_element(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class... Args>
  syntax_element&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  syntax_element&  operator=(const syntax_element&   rhs) noexcept{return assign(rhs);}
  syntax_element&  operator=(      syntax_element&&  rhs) noexcept{return assign(std::move(rhs));}

  syntax_element&  assign(const syntax_element&   rhs) noexcept;
  syntax_element&  assign(      syntax_element&&  rhs) noexcept;
  syntax_element&  assign(int  sep, std::string_view  sv) noexcept;
  syntax_element&  assign(int  sep, std::string&&  s) noexcept;
  syntax_element&  assign(int  sep, syntax_group&&  grp) noexcept;
  syntax_element&  assign(int  sep, const syntax_definition&  def) noexcept;

  bool  is_string()     const noexcept{return m_kind == kind::string;}
  bool  is_group()      const noexcept{return m_kind == kind::group;}
  bool  is_definition() const noexcept{return m_kind == kind::definition;}

  int  get_separator() const noexcept{return m_separator;}

  void  clear() noexcept;

  const std::string&  get_string() const noexcept{return m_data.str;}

  const syntax_group&  get_group() const noexcept{return m_data.grp;}

  const syntax_definition&  get_definition() const noexcept{return *m_data.def;}

  void  print() const noexcept;

};


class
syntax_definition: public syntax_group
{
  std::string  m_name;

public:
  syntax_definition(std::string_view  name) noexcept: m_name(name){}

  const std::string&  get_name() const noexcept{return m_name;}

  void  print() const noexcept;

};


class
syntax_rule
{
  std::list<syntax_definition>  m_definition_list;

  void  start_read(text_iterator&  it);

  std::string   read_string(text_iterator&  it);
  std::string   read_identifier(text_iterator&  it);

  void  read_group(syntax_group&  grp, int  close, text_iterator&  it);

  syntax_definition*  find(std::string_view  name) noexcept;

public:
  syntax_rule(std::string_view  sv) noexcept{assign(sv);}

  syntax_rule&  operator =(std::string_view  sv) noexcept{return assign(sv);}
  syntax_rule&  operator+=(std::string_view  sv) noexcept{return append(sv);}

  syntax_rule&  assign(std::string_view  sv) noexcept;
  syntax_rule&  append(std::string_view  sv) noexcept;

  const syntax_definition*  find(std::string_view  name) const noexcept;

  void  print() const noexcept;

};




}




#endif




