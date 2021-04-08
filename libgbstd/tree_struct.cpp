#include"libgbstd/tree_struct.hpp"




namespace gbstd{




void
tree_struct_node::
add_child(tree_struct_node&  child) noexcept
{
    if(m_last_child)
    {
      m_last_child->m_next_sibling = &child                                  ;
                                      child.m_previous_sibling = m_last_child;
    }

  else
    {
      m_first_child = &child;
    }


  m_last_child = &child;

  child.m_parent = this;

  ++m_number_of_children;
}


void
tree_struct_node::
remove_child(tree_struct_node&  child) noexcept
{
    if(child.m_parent == this)
    {
        if(child.m_previous_sibling){child.m_previous_sibling->m_next_sibling = child.m_next_sibling    ;}
        if(child.m_next_sibling    ){child.m_next_sibling->m_previous_sibling = child.m_previous_sibling;}


        if(m_first_child == &child)
        {
          m_first_child = child.m_next_sibling;
        }


        if(m_last_child == &child)
        {
          m_last_child = child.m_previous_sibling;
        }


      child.m_parent           = nullptr;
      child.m_previous_sibling = nullptr;
      child.m_next_sibling     = nullptr;

      --m_number_of_children;
    }
}


tree_struct_node::
~tree_struct_node()
{
  destroy_children();
}




void
tree_struct_node::
transfer_children(tree_struct_node&  new_parent) noexcept
{
    if(m_first_child)
    {
      auto  ptr = m_first_child;

        while(ptr)
        {
          ptr->m_parent = &new_parent;

          ptr = ptr->m_next_sibling;
        }


        if(new_parent.m_last_child)
        {
          new_parent.m_last_child->m_next_sibling = m_first_child                                              ;
                                                    m_first_child->m_previous_sibling = new_parent.m_last_child;
        }

      else
        {
          new_parent.m_first_child = m_first_child;
        }


      new_parent.m_last_child          = m_last_child;
      new_parent.m_number_of_children += m_number_of_children;

      m_first_child = nullptr;
      m_last_child  = nullptr;

      m_number_of_children = 0;
    }
}


void
tree_struct_node::
destroy_children() noexcept
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




void
tree_struct_node::
isolate() noexcept
{
    if(m_parent)
    {
      m_parent->remove_child(*this);
    }
}




}




