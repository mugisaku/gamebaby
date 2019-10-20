#include"libgbstd/ilc.hpp"




namespace gbstd{




#ifdef __EMSCRIPTEN__
#include<emscripten.h>
#endif




void
initialize_javascript() noexcept
{
#ifdef __EMSCRIPTEN__
  static bool  initialized;

    if(initialized)
    {
      return;
    }

  
  EM_ASM(
    var  els = document.getElementsByClassName("emscripten_border");

    var  cv = document.getElementById("canvas");

    var  parent = cv.parentNode;

    parent.removeChild(cv);

    g_output = document.getElementById("output");
    g_output.style.display = "none";

    document.getElementById("controls").style.visibility = "hidden";

    g_description = document.createElement("div");
        g_caption = document.createElement("div");
       g_twitter_link = document.createElement("div");
        g_github_link = document.createElement("div");
    g_download_anchor = document.createElement("a");

     g_github_link.style.display = "none";
    g_twitter_link.style.display = "none";

     g_github_link.innerHTML = "ソースコードは<a href=\"https://github.com/mugisaku/gamebaby\">こちら</a>";
    g_twitter_link.innerHTML = "意見・要望・不具合の報告などは<a href=\"https://twitter.com/sobicabo\">こちらへ</a>";


    var  table = document.createElement("table");

    var  ret_a = document.createElement("a");

    ret_a.innerHTML = "return to home";
    ret_a.href = "../index.htm";

    els[0].appendChild(table);
    document.body.appendChild(document.createElement("hr"));
    document.body.appendChild(ret_a);

    var  row_table = [];

      for(var  i = 0;  i < 6;  ++i)
      {
        row_table.push(table.insertRow(-1));
      }


    row_table[0].insertCell(-1).appendChild(g_caption);
    row_table[1].insertCell(-1).appendChild(cv);
    row_table[2].insertCell(-1).appendChild(g_download_anchor);
    row_table[3].insertCell(-1).appendChild(g_description);
    row_table[4].insertCell(-1).appendChild(g_github_link);
    row_table[5].insertCell(-1).appendChild(g_twitter_link);

    document.ondragover =
    function(e)
    {
      e.stopPropagation();
      e.preventDefault();
    };


    g_dropped_file_list = [];


    function
    append_file(e)
    {
      g_dropped_file_list.push(new Uint8Array(g_file_reader.result));
    }


    document.ondrop =
    function(e)
    {
      e.stopPropagation();
      e.preventDefault();

      var  f = e.dataTransfer.files[0];

      g_file_reader = new FileReader();

      g_file_reader.onload = append_file;

      g_file_reader.readAsArrayBuffer(f);

    };
  );


  initialized = true;
#endif
}




#ifdef __EMSCRIPTEN__
EM_JS(void,js_set_caption,(const char*  s),{
  g_caption.innerHTML = UTF8ToString(s);
});


EM_JS(void,js_set_description,(const char*  s),{
  g_description.innerHTML = UTF8ToString(s);
});


EM_JS(int,js_get_from_front_dropped_file,(int  i),{
  return g_dropped_file_list[0][i];
});


EM_JS(void,js_update_blob,(const uint8_t*  ptr, size_t  size),{
  var  base = new Blob([HEAP8],{type:"application/octet-stream"});

  g_blob = base.slice(ptr,ptr+size);
});


EM_JS(void,js_download_blob,(const char*  filename),{
    if(!window.FileReader || !g_blob)
    {
      console.log("error");
    }


  g_download_anchor.download = UTF8ToString(filename);

  var  fr = new FileReader();

  fr.onload = function(e){
    g_download_anchor.href = this.result;

    g_download_anchor.click();
  };


  fr.readAsDataURL(g_blob);
});
#endif


void
show_github_link() noexcept
{
#ifdef __EMSCRIPTEN__
  EM_ASM(
    g_github_link.style.display = "block";
  );
#endif
}


void
show_twitter_link() noexcept
{
#ifdef __EMSCRIPTEN__
  EM_ASM(
    g_twitter_link.style.display = "block";
  );
#endif
}


void
set_caption(const char*  s) noexcept
{
#ifdef __EMSCRIPTEN__
  js_set_caption(s);
#endif
}


void
set_description(const char*  s) noexcept
{
#ifdef __EMSCRIPTEN__
  js_set_description(s);
#endif
}


void
update_blob(const uint8_t*  ptr, size_t  size) noexcept
{
#ifdef __EMSCRIPTEN__
  js_update_blob(ptr,size);
#endif
}


void
download_blob(const char*  filename) noexcept
{
#ifdef __EMSCRIPTEN__
  js_download_blob(filename);
#endif
}


int
get_number_of_dropped_files() noexcept
{
#ifdef __EMSCRIPTEN__
  return EM_ASM_INT(return g_dropped_file_list.length);
#else
  return 0;
#endif
}


std::vector<uint8_t>
pop_front_dropped_file() noexcept
{
#ifdef __EMSCRIPTEN__
  int  n = EM_ASM_INT(return g_dropped_file_list[0].length);
#else
  int  n = 0;
#endif

  std::vector<uint8_t>  buf(n);

#ifdef __EMSCRIPTEN__
    for(int  i = 0;  i < n;  ++i)
    {
      buf[i] = js_get_from_front_dropped_file(i);
    }


  EM_ASM(
    g_dropped_file_list.shift();
  );
#endif

  return std::move(buf);
}




}




