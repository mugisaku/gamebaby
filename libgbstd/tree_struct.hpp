#ifndef gbstd_tree_struct_HPP
#define gbstd_tree_struct_HPP


#include"libgbstd/misc.hpp"




namespace gbstd{




class
tree_struct_node
{
  gbstd::pointer_wrapper<tree_struct_node>  m_parent;

  tree_struct_node*  m_previous_sibling=nullptr;
  tree_struct_node*  m_next_sibling    =nullptr;

  int  m_number_of_children=0;

  tree_struct_node*  m_first_child=nullptr;
  tree_struct_node*  m_last_child =nullptr;

public:
  tree_struct_node() noexcept{}
  tree_struct_node(const tree_struct_node& ) noexcept=delete;
  tree_struct_node(      tree_struct_node&&) noexcept=delete;
 virtual ~tree_struct_node();

  tree_struct_node&  operator=(const tree_struct_node& ) noexcept=delete;
  tree_struct_node&  operator=(      tree_struct_node&&) noexcept=delete;

  gbstd::pointer_wrapper<tree_struct_node>  get_parent() const noexcept{return m_parent;}

  gbstd::pointer_wrapper<tree_struct_node>  get_first_child() const noexcept{return m_first_child;}
  gbstd::pointer_wrapper<tree_struct_node>  get_last_child()  const noexcept{return  m_last_child;}

  int  get_number_of_children() const noexcept{return m_number_of_children;}

  template<class  T, class...  Args>
  T&  create_child(Args&&...  args) noexcept
  {
    auto  nd = new T(std::forward<Args>(args)...);

    add_child(*nd);

    return *nd;
  }


  void     add_child(tree_struct_node&  child) noexcept;
  void  remove_child(tree_struct_node&  child) noexcept;

  void  isolate() noexcept;

  void  transfer_children(tree_struct_node&  new_parent) noexcept;
  void  destroy_children() noexcept;

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




