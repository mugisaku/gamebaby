#ifndef gbstd_vm_space_HPP
#define gbstd_vm_space_HPP


#include"libgbstd/vms/expression.hpp"


namespace gbstd{


using typesystem::type_info;
using typesystem::parameter_list;
using typesystem::function_signature;

class statement;
class function;
class space_node;


class
found_info
{
  enum class kinds{
    null, type_info, memo_info, function, namespace_,
  } m_kind=kinds::null;

  union data{
    const type_info*  ti;
    const memo_info*  mi;
    const function*   fn;

  } m_data;

public:
  found_info() noexcept{}
  found_info(const type_info&  ti) noexcept: m_kind(kinds::type_info){m_data.ti = &ti;}
  found_info(const memo_info&  mi) noexcept: m_kind(kinds::memo_info){m_data.mi = &mi;}
  found_info(const function&  fn) noexcept: m_kind(kinds::function){m_data.fn = &fn;}

  operator bool() const noexcept{return m_kind != kinds::null;}

  bool  is_type_info() const noexcept{return m_kind == kinds::type_info;}
  bool  is_memo_info() const noexcept{return m_kind == kinds::memo_info;}
  bool  is_function() const noexcept{return m_kind == kinds::function;}

  const type_info&  get_type_info() const noexcept{return *m_data.ti;}
  const memo_info&  get_memo_info() const noexcept{return *m_data.mi;}
  const function&   get_function() const noexcept{return *m_data.fn;}

};


class
basic_space
{
protected:
  space_node&  m_node;

  std::vector<type_info>  m_type_info_table;

  std::vector<std::unique_ptr<memo_info>>  m_memo_info_table;
  std::vector<std::unique_ptr<function>>  m_function_table;

  parameter_list  read_parameter_list(token_iterator&  it) noexcept;

  type_info   read_struct_type_info(token_iterator&  it) noexcept;
  type_info    read_union_type_info(token_iterator&  it) noexcept;
  type_info     read_enum_type_info(token_iterator&  it) noexcept;
  type_info  read_derived_type_info(token_iterator&  it);

  type_info                    read_alias(token_iterator&  it) noexcept;
  type_info   read_named_struct_type_info(token_iterator&  it) noexcept;
  type_info    read_named_union_type_info(token_iterator&  it) noexcept;
  type_info     read_named_enum_type_info(token_iterator&  it) noexcept;

  const function*  read_function(token_iterator&  it);

  void  read_element_that_begins_with_identifier(token_iterator&  it);

public:
  basic_space(space_node&  nd) noexcept: m_node(nd){}

  void  clear() noexcept;

  space_node&  get_node() const noexcept{return m_node;}

  basic_space&  read(token_iterator&  it, operator_code  close_code);
  basic_space&  read(std::string_view  sv);

  const std::vector<type_info>&                             get_type_info_table() const noexcept{return m_type_info_table;}
  const std::vector<std::unique_ptr<memo_info>>&            get_memo_info_table() const noexcept{return m_memo_info_table;}
  const std::vector<std::unique_ptr<function>>&    get_function_reference_table() const noexcept{return m_function_table;}

  void  push_type_info(type_info&&  ti) noexcept{m_type_info_table.emplace_back(std::move(ti));}
  void  push_memo_info(const type_info&  ti, std::string_view  name) noexcept{m_memo_info_table.emplace_back(std::make_unique<memo_info>(ti,name));}

  function&  create_function(std::string_view  name, function_signature&&  sig) noexcept;

  const function*  find_function(std::string_view  name) const noexcept;

  const type_info*  find_type_info_by_name(std::string_view  name) const noexcept;
  const type_info*  find_type_info_by_id(  std::string_view    id) const noexcept;

  const memo_info*  find_memo_info(std::string_view  name) const noexcept;

  void  print() const noexcept;

};


class global_space;
class     function;
class  block_space;


class
space_node
{
  space_node*  m_parent=nullptr;
  space_node*  m_root=nullptr;

  std::vector<std::unique_ptr<space_node>>  m_children;
 

  enum class kinds{
    null, global_space, function, block_space
  } m_kind=kinds::null;

  union data{
    pointer_wrapper<global_space>   gsp;
    pointer_wrapper<function>        fn;
    pointer_wrapper<block_space>    bsp;

    data() noexcept{}
   ~data(){}
  } m_data;

public:
  space_node() noexcept;
  space_node(space_node&  parent) noexcept;

  template<class...  Args>
  space_node(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  space_node&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  const space_node*  get_parent() const noexcept{return m_parent;}

  space_node&  assign(global_space&  gsp) noexcept;
  space_node&  assign(function&       fn) noexcept;
  space_node&  assign(block_space&   bsp) noexcept;

  space_node&  create_child() noexcept;

  const function*  find_function(std::string_view  name) const noexcept;

  const type_info*  find_type_info_by_name(std::string_view  name) const noexcept;
  const type_info*  find_type_info_by_id(  std::string_view    id) const noexcept;

  const memo_info*  find_memo_info(std::string_view  name) const noexcept;

  found_info  find_all(std::string_view  name) const noexcept;

  bool  is_global_space() const noexcept{return m_kind == kinds::global_space;}
  bool      is_function() const noexcept{return m_kind == kinds::function;}
  bool   is_block_space() const noexcept{return m_kind == kinds::block_space;}

  const std::vector<std::unique_ptr<space_node>>&  get_children() const noexcept{return m_children;}

  global_space&  get_global_space() noexcept{return *m_data.gsp;}
  function&          get_function() noexcept{return *m_data.fn;}
  block_space&    get_block_space() noexcept{return *m_data.bsp;}

  const global_space&  get_global_space() const noexcept{return *m_data.gsp;}
  const function&          get_function() const noexcept{return *m_data.fn;}
  const block_space&    get_block_space() const noexcept{return *m_data.bsp;}

  void  print() const noexcept;

};




class
function
{
  space_node&  m_node;

  std::vector<std::unique_ptr<memo_info>>  m_parameter_memo_info_table;

  std::string  m_name;

  address_t  m_operation_stack_size=0;

  function_signature  m_signature;

  std::unique_ptr<block_space>  m_main_block;

public:
  function(space_node&  nd, std::string_view  name, function_signature&&  sig) noexcept;

  space_node&  get_node() const noexcept{return m_node;}

  block_space&  get_main_block() const noexcept{return *m_main_block;}

  const std::vector<std::unique_ptr<memo_info>>&  get_parameter_memo_info_table() const noexcept {return m_parameter_memo_info_table;}

  const std::string&  get_name() const noexcept{return m_name;}

  const function_signature&  get_signature() const noexcept{return m_signature;}

  void  push_memo_info(const type_info&  ti, std::string_view  name) noexcept{m_parameter_memo_info_table.emplace_back(std::make_unique<memo_info>(ti,name));}

  const memo_info*  find_parameter_memo_info(std::string_view  name) const noexcept;

  void  allocate_address(address_t&  global_end) noexcept;

  void  print() const noexcept;

};


class
global_space: public basic_space
{
  std::string  m_source;

  void  initialize() noexcept;

public:
  global_space(space_node&  nd) noexcept: basic_space(nd){}
  global_space(space_node&  nd, std::string_view  sv): basic_space(nd){assign(sv);}

  global_space&  assign(std::string_view  sv);

  void  clear() noexcept;

  void  allocate_address() noexcept;

};


class
block_space: public basic_space
{
  std::vector<statement>  m_statement_list;

  std::vector<std::unique_ptr<block_space>>  m_children;

  statement  read_return(token_iterator&  it);
  statement  read_jump(token_iterator&  it) noexcept;
  statement  read_label(token_iterator&  it) noexcept;
  statement  read_if(token_iterator&  it) noexcept;
  statement  read_for(token_iterator&  it) noexcept;
  statement  read_while(token_iterator&  it) noexcept;
  statement  read_switch(token_iterator&  it) noexcept;
  statement  read_let(token_iterator&  it);

public:
  block_space(space_node&  nd) noexcept: basic_space(nd){}

  const statement&  operator[](int  i) const noexcept;

  void  clear() noexcept;

  block_space&  create_block_space() noexcept;

  void  read_statement(std::string_view  keyword, token_iterator&  it);

  const function*  get_parent_function() const noexcept;

  const std::vector<statement>&  get_statement_list() const noexcept{return m_statement_list;}

  void  push_statement(statement&&  st) noexcept;

  void  allocate_address(address_t&  global_end, address_t&  local_end, address_t&  operation_stack_size) noexcept;

  void  print() const noexcept;

};




}




#endif




