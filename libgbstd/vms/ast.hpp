#ifndef gbstd_vm_space_HPP
#define gbstd_vm_space_HPP


#include"libgbstd/typesystem.hpp"
#include"libgbstd/vms/expression.hpp"


namespace gbstd{




class
ast_error
{
  std::string  m_text;

  const token*  m_token=nullptr;

public:
  ast_error(std::string_view  sv) noexcept: m_text(sv){}
  ast_error(std::string_view  sv, const token&  tok) noexcept: m_text(sv), m_token(&tok){}

  void  print() const noexcept{  if(m_token){m_token->print();}  printf(": %s",m_text.data());}

};


class
ast_loopguard
{
  std::string_view  m_name;

  const token_iterator  m_iterator;

public:
  ast_loopguard(std::string_view  name, const token_iterator&  it) noexcept:
  m_name(name), m_iterator(it){}

  void  operator()(const token_iterator&  it) const{
      if(m_iterator == it)
      {
        auto  s = form_string("it is looping at %s",m_name.data());

        throw ast_error(s,*m_iterator);
      }
  }

};


class
ast_node
{
  pointer_wrapper<ast_node>  m_parent;

  const token*  m_token=nullptr;

  std::string_view  m_kind_id;

  std::vector<std::unique_ptr<ast_node>>  m_children;

  static std::unique_ptr<ast_node>  read_argument_list(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_index(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_operand(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_primary_expression_element(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_primary_expression(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_unary_operator(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_unary_expression(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_binary_operator(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_expression_element(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_expression(token_iterator&  it);

  static std::unique_ptr<ast_node>  read_let(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_return(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_goto(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_case(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_switch(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_for_instruction(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_for(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_while(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_if(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_if_string(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_statement(token_iterator&  it);

  static std::unique_ptr<ast_node>  read_parameter(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_parameter_list(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_local_space(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_function(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_struct_def(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_struct(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_union_def(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_union(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_enumerator(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_enum_def(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_enum(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_alias(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_type(token_iterator&  it);
  static std::unique_ptr<ast_node>  read_name(token_iterator&  it);

  void  print_internal(std::vector<std::string>&  route) const noexcept;

public:
  static std::unique_ptr<ast_node>  read_global_space(token_iterator&  it);

  ast_node(                   std::string_view  kid):                m_kind_id(kid){}
  ast_node(const token&  tok, std::string_view  kid): m_token(&tok), m_kind_id(kid){}
  ast_node(std::unique_ptr<ast_node>&&  nd, std::string_view  kid): m_kind_id(kid){m_children.emplace_back(std::move(nd));}

  ast_node(const ast_node& ) noexcept=delete;
  ast_node(      ast_node&&) noexcept=delete;

  ast_node&  operator=(const ast_node& ) noexcept=delete;
  ast_node&  operator=(      ast_node&&) noexcept=delete;

  const token&  operator*()  const noexcept{return *m_token;}
  const token*  operator->() const noexcept{return  m_token;}

  bool  operator==(std::string_view  kid) const noexcept{return m_kind_id == kid;}

  std::string_view  get_kind_id() const noexcept{return m_kind_id;}

  void          set_token(const token*  tok)       noexcept{       m_token = tok;}
  const token*  get_token(                 ) const noexcept{return m_token;}

  std::vector<const ast_node*>  find() const noexcept;

  pointer_wrapper<ast_node>  get_parent() const noexcept{return m_parent;}

  void  append_child(std::unique_ptr<ast_node>&&  e) noexcept;
  void  append_child(std::unique_ptr<ast_node>&&  nd, std::string_view  kid) noexcept
  {append_child(std::make_unique<ast_node>(std::move(nd),kid));}

  int  get_number_of_children() const noexcept{return m_children.size();}

  const std::unique_ptr<ast_node>*  begin() const noexcept{return m_children.data();}
  const std::unique_ptr<ast_node>*    end() const noexcept{return m_children.data()+m_children.size();}

  void  print() const noexcept;

};




}




#endif




