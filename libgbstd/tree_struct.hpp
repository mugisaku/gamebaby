#ifndef gbstd_tree_struct_HPP
#define gbstd_tree_struct_HPP


#include"libgbstd/misc.hpp"




namespace gbstd{




template<class  T>
class
tree_struct_node
{
public:
  using content_type = T;

private:
  content_type  m_content;

  std::string  m_path;
  std::string  m_name;

  gbstd::pointer_wrapper<tree_struct_node>  m_parent;

  tree_struct_node*  m_previous_sibling=nullptr;
  tree_struct_node*  m_next_sibling    =nullptr;

  int  m_number_of_children=0;

  tree_struct_node*  m_first_child=nullptr;
  tree_struct_node*  m_last_child =nullptr;

  tree_struct_node(tree_struct_node&  parent, const content_type&   c, std::string_view  name) noexcept: m_parent(&parent), m_content(           c){set_name(name);}
  tree_struct_node(tree_struct_node&  parent,       content_type&&  c, std::string_view  name) noexcept: m_parent(&parent), m_content(std::move(c)){set_name(name);}

  void  add_child(tree_struct_node*  child) noexcept
  {
      if(m_last_child)
      {
        m_last_child->m_next_sibling = child                                   ;
                                       child->m_previous_sibling = m_last_child;
      }

    else
      {
        m_first_child = child;
      }


    m_last_child = child                 ;
                   child->m_parent = this;

    ++m_number_of_children;
  }

  void  update_path() noexcept
  {
      if(m_parent)
      {
        m_path = m_parent->get_path()+m_name;
      }

    else
      {
        m_path = m_name;
      }


      for(auto&  child: *this)
      {
        child.update_path();
      }
  }

public:
  tree_struct_node(std::string_view  name="") noexcept: m_name(name), m_path(name){}
  tree_struct_node(const content_type&   c, std::string_view  name="") noexcept: m_content(c), m_name(name), m_path(name){}
  tree_struct_node(      content_type&&  c, std::string_view  name="") noexcept: m_content(std::move(c)), m_name(name), m_path(name){}
  tree_struct_node(const tree_struct_node& ) noexcept=delete;
  tree_struct_node(      tree_struct_node&&) noexcept=delete;
 ~tree_struct_node(){destroy_children();}

  tree_struct_node&  operator=(const tree_struct_node& ) noexcept=delete;
  tree_struct_node&  operator=(      tree_struct_node&&) noexcept=delete;

  tree_struct_node&  operator=(const content_type&   c) noexcept{  m_content =            c;  return *this;}
  tree_struct_node&  operator=(      content_type&&  c) noexcept{  m_content = std::move(c);  return *this;}

  operator       content_type&()       noexcept{return m_content;}
  operator const content_type&() const noexcept{return m_content;}

        content_type&  get_content()       noexcept{return m_content;}
  const content_type&  get_content() const noexcept{return m_content;}

  const std::string&  get_name() const noexcept{return m_name;}
  const std::string&  get_path() const noexcept{return m_path;}

  void  set_name(std::string_view  name) noexcept
  {
    m_name = name;

    update_path();
  }

  gbstd::pointer_wrapper<tree_struct_node>  get_parent() const noexcept{return m_parent;}

  gbstd::pointer_wrapper<tree_struct_node>  get_first_child() const noexcept{return m_first_child;}
  gbstd::pointer_wrapper<tree_struct_node>  get_last_child()  const noexcept{return  m_last_child;}

  int  get_number_of_children() const noexcept{return m_number_of_children;}

  gbstd::pointer_wrapper<tree_struct_node>  find_child_by_name(std::string_view  name) const noexcept
  {
    auto  ptr = m_first_child;

      while(ptr)
      {
          if(ptr->m_name == name)
          {
            return ptr;
          }


        ptr = ptr->m_next_sibling;
      }


    return nullptr;
  }

  gbstd::pointer_wrapper<tree_struct_node>  find_recursively_by_name(std::string_view  name) const noexcept
  {
    auto  nd = find_child_by_name(name);

      if(!nd)
      {
          for(auto&  child: *this)
          {
            auto  ndnd = child.find_recursively_by_name(name);

              if(ndnd)
              {
                return ndnd;
              }
          }
      }


    return nd;
  }

  std::unique_ptr<tree_struct_node>  clone() const noexcept
  {
    auto  new_nd = std::make_unique<tree_struct_node>(m_name);

    new_nd->m_content = m_content;

      for(auto&  child: *this)
      {
        new_nd->add_child(child.clone().release());
      }


    return std::move(new_nd);
  }

  tree_struct_node&  create_child(std::string_view  name) noexcept
  {
    return create_child(content_type(),name);
  }

  tree_struct_node&  create_child(const content_type&  c, std::string_view  name) noexcept
  {
    auto  nd = new tree_struct_node(*this,c,name);

    add_child(nd);

    return *nd;
  }

  tree_struct_node&  create_child(content_type&&  c, std::string_view  name) noexcept
  {
    auto  nd = new tree_struct_node(*this,std::move(c),name);

    add_child(nd);

    return *nd;
  }

  void  destroy() noexcept
  {
      if(m_parent)
      {
          if(m_previous_sibling){m_previous_sibling->m_next_sibling =     m_next_sibling;}
          if(m_next_sibling    ){m_next_sibling->m_previous_sibling = m_previous_sibling;}

          if(!--m_parent->m_number_of_children)
          {
            m_parent->m_first_child = nullptr;
            m_parent->m_last_child  = nullptr;
          }

        else
          {
              if(this == m_parent->m_first_child)
              {
                m_parent->m_first_child = m_next_sibling;
              }


              if(this == m_parent->m_last_child)
              {
                m_parent->m_last_child = m_previous_sibling;
              }
          }


        delete this;
      }
  }

  void  destroy_children() noexcept
  {
    auto  ptr = m_first_child;

      while(ptr)
      {
        auto  tmp = ptr->m_next_sibling;

        delete ptr      ;
               ptr = tmp;
      }


    m_first_child = nullptr;
    m_last_child  = nullptr;

    m_number_of_children = 0;
  }


  template<class  FN>
  void  read(FN  fn) const noexcept
  {
      if(fn(*this))
      {
          for(auto&  nd: *this)
          {
            nd.read(fn);
          }
      }
  }

  template<class  FN>
  void  process(FN  fn) noexcept
  {
      if(fn(*this))
      {
          for(auto&  nd: *this)
          {
            nd.process(fn);
          }
      }
  }

  class iterator{
    gbstd::pointer_wrapper<tree_struct_node>  m_pointer;
  public:
    constexpr iterator(gbstd::pointer_wrapper<tree_struct_node>  ptr=nullptr) noexcept:
    m_pointer(ptr){}

    tree_struct_node&  operator*() const noexcept{return *m_pointer;}

    iterator&  operator++() noexcept{
      m_pointer = m_pointer->m_next_sibling;

      return *this;
    }

    bool  operator!=(iterator  it) const noexcept{return m_pointer != it.m_pointer;}

  };

  iterator  begin() const noexcept{return {m_first_child};};
  iterator    end() const noexcept{return {             };};

};


}




#endif




