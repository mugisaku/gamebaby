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




class
ast_node
{
  pointer_wrapper<ast_node>  m_parent;

  std::vector<std::unique_ptr<ast_node>>  m_children;

  const token*  m_token=nullptr;

  std::string_view  m_kind_id;

  void  read_function(token_iterator&  it);
  void  read_variable(token_iterator&  it);
  void  read_struct(token_iterator&  it);
  void  read_union(token_iterator&  it);
  void  read_enum(token_iterator&  it);
  void  read_alias(token_iterator&  it);
  void  read_global_space(token_iterator&  it);

public:
  ast_node(                   std::string_view  kid):                m_kind_id(kid){}
  ast_node(const token&  tok, std::string_view  kid): m_token(&tok), m_kind_id(kid){}
  ast_node(const ast_node& ) noexcept=delete;
  ast_node(      ast_node&&) noexcept=delete;
 virtual ~ast_node(){}

  ast_node&  operator=(const ast_node& ) noexcept=delete;
  ast_node&  operator=(      ast_node&&) noexcept=delete;

  const token&  operator*()  const noexcept{return *m_token;}
  const token*  operator->() const noexcept{return  m_token;}

  bool  operator==(std::string_view  kid) const noexcept{return m_kind_id == kid;}

  std::string_view  get_kind_id() const noexcept{return m_kind_id;}

  void          set_token(const token*  tok)       noexcept{       m_token = tok;}
  const token*  get_token(                 ) const noexcept{return m_token;}

  pointer_wrapper<ast_node>  get_parent() const noexcept{return m_parent;}

  void  append_child(ast_node&  nd) noexcept;

  const std::unique_ptr<ast_node>*  begin() const noexcept{return m_children.data();}
  const std::unique_ptr<ast_node>*    end() const noexcept{return m_children.data()+m_children.size();}

  template<class  T>      T&  cast()       noexcept{return *static_cast<      T*>(this);}
  template<class  T>const T&  cast() const noexcept{return *static_cast<const T*>(this);}

  virtual void  print() const noexcept{}

};




class
ast_space: public ast_node
{
protected:
  std::vector<type_info>          m_type_info_table;
  std::vector<variable_info>  m_variable_info_table;

public:
  using ast_node::ast_node;

  const std::vector<type_info>&          get_type_info_table()         const noexcept{return m_type_info_table;}
  const std::vector<variable_info>&  get_variable_info_table() const noexcept{return m_variable_info_table;}

  void  push_type_info(const type_info&   ti) noexcept{m_type_info_table.emplace_back(ti);}
  void  push_type_info(      type_info&&  ti) noexcept{m_type_info_table.emplace_back(std::move(ti));}

  void  push_variable_info(variable_scopes  scp, const type_info&  ti, std::string_view  name) noexcept{m_variable_info_table.emplace_back(std::make_unique<variable_info>(scp,ti,name));}

  type_info  find_type_info(std::string_view  name) const noexcept;

  const variable_info*  find_variable_info(std::string_view  name) const noexcept;

  void  print() const noexcept override;

};




class
global_space: public ast_space
{

public:
  global_space(std::string_view  sv) noexcept;

  void  compile(compile_context&  ctx) const;

  void  print() const noexcept override;

};




class
ast_function: public ast_node
{
  std::vector<variable_info>  m_parameter_variable_info_table;

  std::string  m_name;

  function_signature  m_signature;

  pointer_wrapper<block_space>  m_main_block;

  void  read(token_iterator&  it);

public:
  ast_function(token_iterator&  it, std::string_view  name, function_signature&&  sig);

  block_space&  get_main_block() const noexcept{return *m_main_block;}

  const std::vector<variable_info>&  get_parameter_variable_info_table() const noexcept {return m_parameter_variable_info_table;}

  const std::string&  get_name() const noexcept{return m_name;}

  const function_signature&  get_signature() const noexcept{return m_signature;}

  void  push_variable_info(const type_info&  ti, std::string_view  name) noexcept{m_parameter_variable_info_table.emplace_back(std::make_unique<variable_info>(variable_scopes::parameter,ti,name));}

  const variable_info*  find_parameter_variable_info(std::string_view  name) const noexcept;

  void  compile(compile_context&  ctx) const;

  void  print() const noexcept override;

};




/*

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


class global_space;
class     function;
class  block_space;
class   space_node;




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
*/




}




#endif




