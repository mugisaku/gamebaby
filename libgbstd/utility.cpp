#include"libgbstd/utility.hpp"
#include<cstdio>
#include<cstring>


#ifdef __EMSCRIPTEN__
#include<emscripten.h>
#endif




namespace gbstd{


int  reporting_counter;


std::string
make_string_from_file(const char*  filepath) noexcept
{
  std::string  s;

  auto  f = fopen(filepath,"rb");

    if(f)
    {
        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f))
            {
              break;
            }


          s += c;
        }


      fclose(f);
    }

  else
    {
      printf("make_string_from_file error: %sは開けない\n",filepath);
    }


  return std::move(s);
}


void
printf_with_indent(int  indent, const char*  fmt, ...) noexcept
{
  char  buf[256];

  va_list  ap;
  va_start(ap,fmt);

  vsnprintf(buf,sizeof(buf),fmt,ap);

  va_end(ap);

  const char*  p = buf;

    while(*p)
    {
      fputc(*p,stdout);

        if(*p++ == '\n')
        {
            for(int  n = 0;  n < indent;  ++n)
            {
              fputc(' ',stdout);
            }
        }
    }
}




#ifdef __EMSCRIPTEN__
#include<emscripten.h>
EM_JS(void,js_set_caption,(const char*  s),{
  var  cap = document.getElementById("caption");
  cap.innerHTML = UTF8ToString(s);
});
EM_JS(void,js_set_description,(const char*  s),{
  var  desc = document.getElementById("description");
  desc.innerHTML = UTF8ToString(s);
});
EM_JS(int,js_get_from_front_dropped_file,(int  i),{
  return g_dropped_file_list[0][i];
});
EM_JS(void,js_update_common_blob,(const uint8_t*  ptr, size_t  size),{
  var  base = new Blob([HEAP8],{type:"application/octet-stream"});

  g_object.common_blob = base.slice(ptr,ptr+size);
});
EM_JS(void,js_download_common_blob,(const char*  filename),{
    if(!window.FileReader || !g_object.common_blob)
    {
      console.log("error");
    }


  g_download_anchor = document.getElementById("ln");

  g_download_anchor.download = UTF8ToString(filename);

  var  fr = new FileReader();

  fr.onload = function(e){
    g_download_anchor.href = this.result;

    g_download_anchor.click();
  };


  fr.readAsDataURL(g_object.common_blob);
});


void
show_github_link() noexcept
{
  EM_ASM(
    var  ln = document.getElementById("github_link");

    ln.style.visibility = "visible";
  );
}
void
show_twitter_link() noexcept
{
  EM_ASM(
    var  ln = document.getElementById("twitter_link");

    ln.style.visibility = "visible";
  );
}
void
set_caption(const char*  s) noexcept
{
  js_set_caption(s);
}
void
set_description(const char*  s) noexcept
{
  js_set_description(s);
}
void
update_common_blob(const uint8_t*  ptr, size_t  size) noexcept
{
  js_update_common_blob(ptr,size);
}
void
download_common_blob(const char*  filename) noexcept
{
  js_download_common_blob(filename);
}
int
get_number_of_dropped_files() noexcept
{
  return EM_ASM_INT(return g_dropped_file_list.length);
}
std::vector<uint8_t>
pop_front_dropped_file() noexcept
{
  int  n = EM_ASM_INT(return g_dropped_file_list[0].length);

  std::vector<uint8_t>  buf(n);

    for(int  i = 0;  i < n;  ++i)
    {
      buf[i] = js_get_from_front_dropped_file(i);
    }


  EM_ASM(
    g_dropped_file_list.shift();
  );

  return std::move(buf);
}
#endif


bool
is_png(const void*  ptr) noexcept
{
  static const uint8_t  sig[] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};

  return std::memcmp(ptr,sig,sizeof(sig)) == 0;
}


bool
is_webp(const void*  ptr) noexcept
{
  auto  p = static_cast<const char*>(ptr);

  return (std::memcmp(p  ,"RIFF",4) == 0) &&
         (std::memcmp(p+8,"WEBP",4) == 0);
}




std::random_device
random_device;


std::default_random_engine
random_number_engine(random_device());




string_form::
string_form(const char*  fmt, ...) noexcept
{
  va_list  ap;
  va_start(ap,fmt);

  auto  res = vsnprintf(m_buffer,sizeof(m_buffer),fmt,ap);

    if((res < -1) || (res >= sizeof(m_buffer)))
    {
      report;

      m_length = 0;
    }

  else
    {
      m_length = res;
    }


  va_end(ap);
}


const char*
string_form::
operator()(const char*  fmt, ...) noexcept
{
  va_list  ap;
  va_start(ap,fmt);

  auto  res = vsnprintf(m_buffer,sizeof(m_buffer),fmt,ap);

    if((res < -1) || (res >= sizeof(m_buffer)))
    {
      report;

      m_length = 0;
    }

  else
    {
      m_length = res;
    }


  va_end(ap);

  return m_buffer;
}





void
fixed_t::
print() const noexcept
{
  printf("%d.",to_int(m_data));

  int  f = m_data&0xFFFF;

    if(!f)
    {
      printf("0");
    }

  else
    {
      int  div_amount = to_fpn(1)/10;

        while(div_amount)
        {
          int  i = f/div_amount;

          printf("%d",(i > 9)? 9:i);

          f %= div_amount      ;
               div_amount /= 10;
        }
    }
}




}




