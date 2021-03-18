#include"libgbstd/vms/ast.hpp"




namespace gbstd{




ast_content&
ast_content::
assign(ast_content&&  rhs) noexcept
{
    if(this != &rhs)
    {
      std::swap(m_deleter,rhs.m_deleter);
      std::swap(m_data   ,rhs.m_data);
      std::swap(m_token  ,rhs.m_token);
    }


  return *this;
}


namespace{
class
node_iterator
{
  const ast_node::child_type*  m_begin;
  const ast_node::child_type*  m_current;
  const ast_node::child_type*  m_end;

  static const ast_node  m_null;

public:
  node_iterator(const ast_node&  nd) noexcept:
  m_begin(nd.begin()), m_current(nd.begin()), m_end(nd.end()){}

  operator bool() const noexcept{return m_current < m_end;}

  const ast_node*  operator->() const noexcept{return (*this)? m_current->get():&m_null;}
  const ast_node&  operator*() const noexcept{return (*this)? **m_current:m_null;}

  node_iterator&  operator++() noexcept{  ++m_current;  return *this;}

  node_iterator  operator++(int) noexcept{
    auto  tmp = *this;

    ++m_current;

    return tmp;
  }

  const ast_node&  operator[](int  i) const noexcept
  {
    return (m_current+i) < m_end? *m_current[i]:m_null;
  }

  const ast_node&  operator[](std::string_view  name) const noexcept
  {
      for(auto  it = m_current;  it < m_end;  ++it)
      {
        auto&  child = *it++;

          if(child->is(name))
          {
            return *child;
          }
      }


    return m_null;
  }

};


const ast_node
node_iterator::
m_null;


void
print_variable_declaration(node_iterator  it) noexcept
{
}
void
print_type_declaration(node_iterator  it) noexcept
{
  auto&  child = *it;

    if(child.is("enum declaration"))
    {
    }

  else
    if(child.is("struct declaration"))
    {
    }

  else
    if(child.is("union declaration"))
    {
    }

  else
    if(child.is("alias declaration"))
    {
    }
}
void
print_function_declaration(node_iterator  it) noexcept
{
    for(auto&  child: it->children())
    {

    }
}
void
print_declaration(node_iterator  it) noexcept
{
    for(auto&  child: it->children())
    {
      auto&  name = child->name();

        if(name == "variable declaration")
        {
          print_variable_declaration(*child);
        }

      else
        if(name == "type declaration")
        {
          print_type_declaration(*child);
        }

      else
        if(name == "function declaration")
        {
          print_function_declaration(*child);
        }
    }
}
void
print_global_space(node_iterator  it) noexcept
{
    for(auto&  child: it->children())
    {
      auto&  name = child->name();

        if(name == "declaration")
        {
          print_declaration(*child);
        }
    }
}
}


void
print(const ast_node&  nd) noexcept
{
    for(auto&  child: nd)
    {
        if(child->name() == "global space")
        {
          printf("//global space\n");

          print_global_space(*child);

          printf("\n\n");
        }
    }
}




}




