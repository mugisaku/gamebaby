#ifndef gbstd_vm_space_HPP
#define gbstd_vm_space_HPP


#include"libgbstd/vms/statement.hpp"


namespace gbstd{


constexpr int  g_word_size = 8;


using typesystem::type_info;
using typesystem::enum_type_def;
using typesystem::struct_type_def;
using typesystem::union_type_def;
using typesystem::parameter_list;
using typesystem::function_signature;

class statement;
class function;
class space_node;




enum class
variable_scopes
{
  global,
  local,
  parameter,

};


class
variable_info
{
  type_info  m_type_info;

  std::string  m_name;

  std::vector<uint8_t>  m_content;

  variable_scopes  m_scope;

public:
  variable_info() noexcept{}
  variable_info(variable_scopes  scp, const type_info&  ti, std::string_view  name) noexcept:
  m_scope(scp), m_type_info(ti), m_name(name){}

  const type_info&  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

  void                         set_content(std::vector<uint8_t>&&  c)       noexcept{       m_content = std::move(c);}
  const std::vector<uint8_t>&  get_content(                         ) const noexcept{return m_content;}

  bool  is_global_scope()    const noexcept{return m_scope == variable_scopes::global;}
  bool  is_local_scope()     const noexcept{return m_scope == variable_scopes::local;}
  bool  is_parameter_scope() const noexcept{return m_scope == variable_scopes::parameter;}

  void  print() const noexcept{  m_type_info.print();  printf("  %s",m_name.data());}

};


class
found_info
{
  enum class kinds{
    null,
    type_info,
    variable_info,
    function,

  } m_kind=kinds::null;

  union data{
    const type_info*      ti;
    const variable_info*  vi;
    const function*       fn;

  } m_data;

public:
  found_info() noexcept{}
  found_info(const type_info&  ti) noexcept: m_kind(kinds::type_info){m_data.ti = &ti;}
  found_info(const variable_info&  vi) noexcept: m_kind(kinds::variable_info){m_data.vi = &vi;}
  found_info(const function&  fn) noexcept: m_kind(kinds::function){m_data.fn = &fn;}

  operator bool() const noexcept{return m_kind != kinds::null;}

  bool  is_type_info()     const noexcept{return m_kind == kinds::type_info;}
  bool  is_variable_info() const noexcept{return m_kind == kinds::variable_info;}
  bool  is_function()      const noexcept{return m_kind == kinds::function;}

  const type_info&          get_type_info() const noexcept{return *m_data.ti;}
  const variable_info&  get_variable_info() const noexcept{return *m_data.vi;}
  const function&            get_function() const noexcept{return *m_data.fn;}

};


class
basic_space
{
protected:
  space_node&  m_node;

  std::vector<type_info>                       m_type_info_table;
  std::vector<std::unique_ptr<variable_info>>  m_variable_info_table;

  std::vector<statement>  m_statement_list;

  parameter_list  read_parameter_list(token_iterator&  it) noexcept;

  struct_type_def   read_struct_type_def(token_iterator&  it) noexcept;
  union_type_def     read_union_type_def(token_iterator&  it) noexcept;
  enum_type_def       read_enum_type_def(token_iterator&  it) noexcept;
  type_info  read_derived_type_info(token_iterator&  it);

  type_info              read_alias(token_iterator&  it) noexcept;
  type_info   read_struct_type_decl(token_iterator&  it) noexcept;
  type_info    read_union_type_decl(token_iterator&  it) noexcept;
  type_info     read_enum_type_decl(token_iterator&  it) noexcept;

  const function*  read_function(token_iterator&  it);

  statement  read_return(token_iterator&  it);
  statement  read_jump(token_iterator&  it);
  statement  read_label(token_iterator&  it);
  statement  read_if_string(token_iterator&  it);
  if_statement  read_if(token_iterator&  it);
  statement  read_for(token_iterator&  it);
  statement  read_while(token_iterator&  it);
  statement  read_switch(token_iterator&  it);
  statement  read_case(token_iterator&  it);
  statement  read_let(token_iterator&  it);

  void  read_statement(std::string_view  keyword, token_iterator&  it);

  void  read_element_that_begins_with_identifier(token_iterator&  it);

public:
  basic_space(space_node&  nd) noexcept: m_node(nd){}
  basic_space(const space_node& ) noexcept=delete;
  basic_space(      space_node&&) noexcept=delete;

  void  clear() noexcept;

  space_node&  get_node() const noexcept{return m_node;}

  basic_space&  read(token_iterator&  it, operator_code  close_code);
  basic_space&  read(std::string_view  sv);

  const std::vector<type_info>&                   get_type_info_table()         const noexcept{return m_type_info_table;}
  const std::vector<std::unique_ptr<variable_info>>&  get_variable_info_table() const noexcept{return m_variable_info_table;}
  const std::vector<statement>&                    get_statement_list()         const noexcept{return m_statement_list;}

  template<class...  Args>
  void  push_statement(Args&&...  args) noexcept{m_statement_list.emplace_back(std::forward<Args>(args)...);}

  void  push_type_info(const type_info&   ti) noexcept{m_type_info_table.emplace_back(ti);}
  void  push_type_info(      type_info&&  ti) noexcept{m_type_info_table.emplace_back(std::move(ti));}

  void  push_variable_info(variable_scopes  scp, const type_info&  ti, std::string_view  name) noexcept{m_variable_info_table.emplace_back(std::make_unique<variable_info>(scp,ti,name));}

  type_info  find_type_info(std::string_view  name) const noexcept;

  const variable_info*  find_variable_info(std::string_view  name) const noexcept;

  void  print() const noexcept;

};


class global_space;
class     function;
class  block_space;
class   space_node;




class
function
{
  space_node&  m_node;

  std::vector<std::unique_ptr<variable_info>>  m_parameter_variable_info_table;

  std::string  m_name;

  function_signature  m_signature;

  pointer_wrapper<block_space>  m_main_block;

public:
  function(space_node&  nd, std::string_view  name, function_signature&&  sig) noexcept;

  space_node&  get_node() const noexcept{return m_node;}

  block_space&  get_main_block() const noexcept{return *m_main_block;}

  const std::vector<std::unique_ptr<variable_info>>&  get_parameter_variable_info_table() const noexcept {return m_parameter_variable_info_table;}

  const std::string&  get_name() const noexcept{return m_name;}

  const function_signature&  get_signature() const noexcept{return m_signature;}

  void  push_variable_info(const type_info&  ti, std::string_view  name) noexcept{m_parameter_variable_info_table.emplace_back(std::make_unique<variable_info>(variable_scopes::parameter,ti,name));}

  const variable_info*  find_parameter_variable_info(std::string_view  name) const noexcept;

  void  compile(compile_context&  ctx) const;

  void  print() const noexcept;

};


class
global_space: public basic_space
{
  void  initialize() noexcept;

public:
  global_space(space_node&  nd) noexcept: basic_space(nd){}

  global_space&  assign(std::string_view  sv);

  void  compile(compile_context&  ctx) const;

  void  print() const noexcept;

};


class
block_space: public basic_space
{
  std::vector<std::unique_ptr<block_space>>  m_children;

public:
  block_space(space_node&  nd) noexcept: basic_space(nd){}

  const statement&  operator[](int  i) const noexcept;

  void  clear() noexcept;

  block_space&  create_block_space() noexcept;

  const function*  get_parent_function() const noexcept;

  void  compile(compile_context&  ctx) const;

  void  print() const noexcept;

};




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
    global_space   gsp;
    function        fn;
    block_space    bsp;

    data() noexcept{}
   ~data(){}
  } m_data;

public:
  space_node() noexcept;
  space_node(const space_node& ) noexcept=delete;
  space_node(      space_node&&) noexcept=delete;
 ~space_node(){clear();}

  template<class...  Args>
  explicit space_node(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  space_node&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  space_node&  clear() noexcept;

  const space_node*  get_parent() const noexcept{return m_parent;}

  function&        create_function(std::string_view  name, typesystem::function_signature&&  sig) noexcept;
  block_space&  create_block_space() noexcept;

  const function*  find_function(std::string_view  name) const noexcept;

  type_info  find_type_info(std::string_view  name) const noexcept;

  const variable_info*  find_variable_info(std::string_view  name) const noexcept;

  found_info  find_all(std::string_view  name) const noexcept;

  bool  is_global_space() const noexcept{return m_kind == kinds::global_space;}
  bool      is_function() const noexcept{return m_kind == kinds::function;}
  bool   is_block_space() const noexcept{return m_kind == kinds::block_space;}

  const std::vector<std::unique_ptr<space_node>>&  get_children() const noexcept{return m_children;}

  void  collect_functions(std::vector<std::reference_wrapper<function>>&  buf) const noexcept;
  void  collect_global_variables(std::vector<std::reference_wrapper<variable_info>>&  buf) const noexcept;

  global_space&  get_global_space() noexcept{return m_data.gsp;}
  function&          get_function() noexcept{return m_data.fn;}
  block_space&    get_block_space() noexcept{return m_data.bsp;}

  const global_space&  get_global_space() const noexcept{return m_data.gsp;}
  const function&          get_function() const noexcept{return m_data.fn;}
  const block_space&    get_block_space() const noexcept{return m_data.bsp;}

  void  print() const noexcept;

};




class
compile_context
{
  std::string  m_source;

  space_node  m_root_node;

  std::string  m_ir_text;

  struct task{
    std::string  m_ir_text;

    const function*  m_function;

    int  m_variable_counter=0;
    int  m_entry_counter=0;
    int  m_if_string_counter=0;
    int  m_for_counter=0;
    int  m_while_counter=0;
    int  m_switch_counter=0;

    std::vector<std::string>  m_block_name_stack;

  };

  std::list<task>  m_stack;

  char  m_buffer[1024];

public:
  compile_context() noexcept{}

  compile_context&  assign(std::string_view  src_code);

  std::string  create_variable_name() noexcept;

  void  write_global(const char*  fmt, ...) noexcept;
  void  write_local(const char*  fmt, ...) noexcept;

  void  start_function_block(const function&  fn);
  void  start_if_string_block();
  void  start_for_block();
  void  start_while_block();
  void  start_switch_block();

  void  finish_block() noexcept;

  void  print() const noexcept;

};




}




#endif




