#include"libgbstd/character.hpp"




namespace gbstd{


namespace{


const glyph
g_glyphs[] =
{
#include"libgbstd/characters/font_data.txt"
};


const glyph*
g_table[0x10000] = {0};


struct
initializer
{
  initializer() noexcept;

} init;


initializer::
initializer() noexcept
{
    for(auto&  gly: g_glyphs)
    {
      g_table[gly.unicode] = &gly;
    }


    for(int  i = 0x00;  i <= 0x5E;  ++i)
    {
      g_table[0xFF00+i] = g_table[' '+i];
    }
}
}


const glyph&
get_glyph(char16_t  c) noexcept
{
  static const glyph  null = {0};

  auto  ptr = g_table[c];

  return ptr? *ptr:null;
}


}




