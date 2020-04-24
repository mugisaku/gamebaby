



class
block_statement: public basic_space
{
  std::vector<statement>  m_statement_list;

public:
  block_statement(space_node&  nd) noexcept: basic_space(nd){}

  const statement&  operator[](int  i) const noexcept;

  void  clear() noexcept;

  const std::vector<statement>&  get_statement_list() const noexcept{return m_statement_list;}

  void  push_statement(statement&&  st) noexcept;

  block_statement&  read(token_iterator&  it) noexcept;
  block_statement&  read(std::string_view  sv) noexcept;

  type_info  create_type_from_string(std::string_view  sv) noexcept;

  void  print() const noexcept;

};


class
return_statement
{
  expression  m_expression;

public:
  return_statement() noexcept{}
  return_statement(expression&&  e) noexcept: m_expression(std::move(e)){}

        expression&  get_expression()       noexcept{return m_expression;}
  const expression&  get_expression() const noexcept{return m_expression;}

  void  print() const noexcept{  printf("return ");  m_expression.print();}

};


class
label_statement
{
  std::string  m_string;

public:
  label_statement() noexcept{}
  label_statement(std::string_view  sv) noexcept: m_string(sv){}

  const std::string&  get_string() const noexcept{return m_string;}

  void  print() const noexcept{printf("label:%s",m_string.data());}

};


class
jump_statement
{
  std::string  m_string;

public:
  jump_statement() noexcept{}
  jump_statement(std::string_view  sv) noexcept: m_string(sv){}

  const std::string&  get_string() const noexcept{return m_string;}

  void  print() const noexcept{printf("jump %s",m_string.data());}

};


class
control_statement
{
  std::string  m_string;

public:
  control_statement(std::string_view  sv) noexcept: m_string(sv){}

  bool  operator==(std::string_view  sv) const noexcept{return m_string == sv;}

  const std::string&  get_string() const noexcept{return m_string;}

  void  print() const noexcept{printf("%s",m_string.data());}

};


class
let_statement
{
  std::string  m_target_name;

  expression  m_expression;

public:
  let_statement(std::string_view  sv, expression&&  e) noexcept:
  m_target_name(sv), m_expression(std::move(e)){}

  const std::string&  get_target_name() const noexcept{return m_target_name;}

        expression&  get_expression()       noexcept{return m_expression;}
  const expression&  get_expression() const noexcept{return m_expression;}

  void  print() const noexcept{
    printf("let %s",m_target_name.data());
      if(m_expression){  printf(" = (");  m_expression.print();  printf(")");}
  }

};


class
condition_statement
{
  expression  m_expression;

  pointer_wrapper<block_statement>  m_block;

public:
  const expression&  get_expression() const noexcept{return m_expression;}

  const block_statement&  get_block() const noexcept{return *m_block;}

};


class
if_string_statement
{
  std::vector<condition_statement>  m_if_list;

  pointer_wrapper<block_statement>  m_else_block;

public:
  const block_statement&  get_else_block() const noexcept{return *m_else_block;}

  void  print() const noexcept{}

};


class
statement
{
  enum class kind{
    null,
    return_,
    block,
    control,
    let,
    if_string,
    expression,
    jump,
    label,

  } m_kind=kind::null;

  union data{
    return_statement       ret;
    control_statement     ctrl;
    let_statement          let;
    if_string_statement    ifs;
    jump_statement         jmp;
    label_statement         lb;
    expression            expr;

    pointer_wrapper<block_statement>  blk;

   data() noexcept{}
  ~data(){}
  } m_data;

public:
  statement() noexcept{}
  statement(const statement&   rhs) noexcept=delete;//{assign(rhs);}
  statement(      statement&&  rhs) noexcept{assign(std::move(rhs));}
  statement(return_statement&&      st) noexcept{assign(std::move(st));}
  statement(label_statement&&       st) noexcept{assign(std::move(st));}
  statement(jump_statement&&        st) noexcept{assign(std::move(st));}
  statement(if_string_statement&&   st) noexcept{assign(std::move(st));}
  statement(block_statement&        st) noexcept{assign(st);}
  statement(control_statement&&     st) noexcept{assign(std::move(st));}
  statement(let_statement&&         st) noexcept{assign(std::move(st));}
  statement(expression&&  e) noexcept{assign(std::move(e));}
 ~statement(){clear();}

  statement&  operator=(const statement&   rhs) noexcept=delete;//{return assign(rhs);}
  statement&  operator=(      statement&&  rhs) noexcept{return assign(std::move(rhs));}

  operator bool() const noexcept{return m_kind != kind::null;}

//  statement&  assign(const statement&   rhs) noexcept;
  statement&  assign(      statement&&  rhs) noexcept;
  statement&  assign(return_statement&&       st) noexcept;
  statement&  assign(label_statement&&       st) noexcept;
  statement&  assign(jump_statement&&        st) noexcept;
  statement&  assign(if_string_statement&&   st) noexcept;
  statement&  assign(block_statement&        st) noexcept;
  statement&  assign(control_statement&&     st) noexcept;
  statement&  assign(let_statement&&         st) noexcept;
  statement&  assign(expression&&  e) noexcept;

  void  clear() noexcept;

  bool  is_null()       const noexcept{return m_kind == kind::null;}
  bool  is_return()     const noexcept{return m_kind == kind::return_;}
  bool  is_label()      const noexcept{return m_kind == kind::label;;}
  bool  is_jump()       const noexcept{return m_kind == kind::jump;}
  bool  is_if_string()  const noexcept{return m_kind == kind::if_string;}
  bool  is_block()      const noexcept{return m_kind == kind::block;}
  bool  is_control()    const noexcept{return m_kind == kind::control;}
  bool  is_let()        const noexcept{return m_kind == kind::let;}
  bool  is_expression() const noexcept{return m_kind == kind::expression;}

  const return_statement&      get_return()     const noexcept{return m_data.ret;}
  const label_statement&       get_label()      const noexcept{return m_data.lb;}
  const jump_statement&        get_jump()       const noexcept{return m_data.jmp;}
  const if_string_statement&   get_if_string()  const noexcept{return m_data.ifs;}
  const block_statement&       get_block()      const noexcept{return *m_data.blk;}
  const control_statement&     get_control()    const noexcept{return m_data.ctrl;}
  const let_statement&         get_let()        const noexcept{return m_data.let;}
  const expression&            get_expression() const noexcept{return m_data.expr;}

  void  print(const context*  ctx=nullptr, const function*  fn=nullptr) const noexcept;

};




