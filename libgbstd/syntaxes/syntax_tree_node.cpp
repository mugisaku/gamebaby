#include"libgbstd/syntax.hpp"




namespace gbstd{




void
syntax_tree_node::
print() const noexcept
{
    if(m_definition)
    {
      gbstd::print(m_definition->get_name());
    }

  else
    if(m_element)
    {
      m_element->print();
    }


  printf("{\n");

    for(auto&  child: *this)
    {
      auto&  nd = static_cast<const syntax_tree_node&>(child);

      nd.print();

      printf("\n");
    }


  printf("}\n");
}



}




