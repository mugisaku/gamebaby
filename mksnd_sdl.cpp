#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/parser.hpp"
#include"libgbstd/file_op.hpp"
#include"libonch/onch.hpp"
#include"sdl.hpp"
#include<list>
#include<vector>




#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{



onch_sound_spec
g_spec;




std::vector<uint8_t>
g_source_code;

std::vector<uint8_t>
g_wave_bin;

onch_space
g_space;


void
update_wave_binary(std::vector<uint8_t>&&  new_code, gbstd::f32_t  volume) noexcept
{
    if(g_source_code != new_code)
    {
      g_source_code = std::move(new_code);

      g_space.clear();

      g_space.load_from_string(reinterpret_cast<const char*>(g_source_code.data()));

      g_spec.volume = volume;

      g_wave_bin = g_space.make_wave_format_binary(g_spec);
#ifdef __EMSCRIPTEN__
      gbstd::update_common_blob(g_wave_bin.data(),g_wave_bin.size());
#endif
    }
}




#ifdef __EMSCRIPTEN__
void
main_loop() noexcept
{
  sdl::update_control();

    if(gbstd::g_dropped_file.size())
    {
      auto&  back = gbstd::g_dropped_file.back();

      auto  o = back    ;
                back = 0;

        if(g_source_code != gbstd::g_dropped_file)
        {
          update_wave_binary(std::move(gbstd::g_dropped_file),(o == 's')? 1.0:0.2);
        }


        if(o == 'p')
        {
        EM_ASM(
             if(g_object.audio)
             {
               g_object.audio.pause();
             }


          g_object.audio = new Audio();

          g_object.audio.src = URL.createObjectURL(g_object.common_blob);

          g_object.audio.play();
        );
        }

      else
        if(o == 't')
        {
        EM_ASM(
             if(g_object.audio)
             {
               g_object.audio.pause();
             }
        );
        }

      else
        if(o == 's')
        {
          gbstd::download_common_blob("new.wav");
        }


      gbstd::g_dropped_file.clear();
    }
}
#endif
}




int
main(int  argc, char**  argv)
{
  g_spec.sampling_rate = 24000;

  g_spec.bit_depth = 8;

  g_spec.volume = 1.0;

#ifdef __EMSCRIPTEN__
  set_description("<pre>"
                  "</pre>");

  show_github_link();
  show_twitter_link();

EM_ASM(
  g_object.transfer = function(o)
  {
    var  src = g_object.textarea.value;

    var  arr = new Uint8Array(src.length+1);

    var  dst_i = 0;

      for(var  src_i = 0;  src_i < src.length;  ++src_i)
      {
        var  c = src.charCodeAt(src_i);

        arr[dst_i++] = (c <= 0x7F)? c:' '.charCodeAt(0);
      }


    arr[dst_i++] = o;

    g_dropped_file_list.push(arr);
  };

  var  free = document.getElementById('free');

  var  play_button = document.createElement('button');
  var  stop_button = document.createElement('button');
  var  save_button = document.createElement('button');

  play_button.innerText = 'play';
  play_button.onclick = function(){g_object.transfer('p'.charCodeAt(0));};

  stop_button.innerText = 'stop';
  stop_button.onclick = function(){g_object.transfer('t'.charCodeAt(0));};

  save_button.innerText = 'save';
  save_button.onclick = function(){g_object.transfer('s'.charCodeAt(0));};


  var  textarea = document.createElement('textarea');

  textarea.id = 'source';
  textarea.cols = 100;
  textarea.rows =  28;
  textarea.value =
   "/*この間がコメント*/"+"\n"
  +"//行末までコメント"+"\n"
  +""+"\n"
  +"//'p5:v15:f48->52:b2'といった記述が演奏指示の最小単位でword要素と呼びます"+"\n"
  +"//pは演奏です。後ろに演奏時間(1000分の1秒単位)を指定します"+"\n"
  +"//rは休止です。後ろに休止時間(1000分の1秒単位)を指定します"+"\n"
  +"//vは音量です。後ろに、開始時音量->終了時音量(0から31)を指定します"+"\n"
  +"//fは音高です。後ろに、開始時音高->終了時音高(1から108)を指定します"+"\n"
  +"//　48が基準音です"+"\n"
  +"//　終了時の音量、音高は省略することができます"+"\n"
  +"//bはビブラートを表し、後ろに続く数字は、ビブラート頻度(0から7)を表します"+"\n"
  +""+"\n"
  +"//word要素の列をtext要素と呼びます"+"\n"
  +"//下記の構文は、txt1と言う名前とtext要素とを結びつけます"+"\n"
  +"txt1 = text{'p1000:v7:f48' 'p500:v7:f40'}"+"\n"
  +""+"\n"
  +"//下記のように、名前と結びつけた要素は再利用することができ"+"\n"
  +"//繰り返しを表すのに便利です"+"\n"
  +"txt2 = text{'p700-v10:f67:b3' txt1 'r400' txt1}"+"\n"
  +""+"\n"
  +"//square,triangle,sawtooth,sine,noise,short_noiseは、cell要素と呼びます"+"\n"
  +"//word要素とtext要素を内容とし、その音色を指示します"+"\n"
  +"//squareは、矩形波です"+"\n"
  +"//triangleは、三角波です"+"\n"
  +"//sawtoothは、ノコギリ波です"+"\n"
  +"//sineは、正弦波です"+"\n"
  +"//noiseは、ノイズです"+"\n"
  +"//short_noiseは、短周期ノイズです"+"\n"
  +"//"+"\n"
  +"sq =      square{                         txt2}"+"\n"
  +"tr =    triangle{'r2'                     txt2}"+"\n"
  +"sa =    sawtooth{'r2' 'r2'                txt2}"+"\n"
  +"si =        sine{'r2' 'r2' 'r2'           txt2}"+"\n"
  +"no =       noise{'r2' 'r2' 'r2' 'r2'      txt2}"+"\n"
  +"sh = short_noise{'r2' 'r2' 'r2' 'r2' 'r2' txt2}"+"\n"
  +""+"\n"
  +"//column,rowは、table要素と呼びます"+"\n"
  +"//columnは内容を同時に演奏し、rowは内容を順番に演奏します"+"\n"
  +"//table要素の内容となるのは、table要素とcell要素です"+"\n"
  +"main = column{sq tr sa si no sh}"+"\n"
  +""+"\n"
  +"//mainという名前と結びついた要素が、最終結果として"+"\n"
  +"//WAVE形式で出力されます"+"\n";

  g_object.textarea = textarea;

  free.appendChild(textarea);
  free.appendChild(play_button);
  free.appendChild(stop_button);
  free.appendChild(save_button);
);


  emscripten_set_main_loop(main_loop,0,false);
#else

  --argc;
  ++argv;

  int  n = 0;

    while(argc--)
    {
      onch_space  sp;

      auto  path = *argv++;

        if(*path == '-')
        {
          int  vol;

            if(sscanf(path,"-sps:%d",&g_spec.sampling_rate) == 1)
            {
              printf("sampling rate was changed to %d\n",g_spec.sampling_rate);
            }

          else
            if(sscanf(path,"-bps:%d",&g_spec.bit_depth) == 1)
            {
              printf("bits per sample was changed to %d\n",g_spec.bit_depth);
            }

          else
            if(sscanf(path,"-vol:%d",&vol) == 1)
            {
              g_spec.volume = static_cast<f32_t>(vol)/10;

              printf("volume was changed to %f\n",g_spec.volume);
            }
        }

      else
        {
          sp.load_from_file(path);

          sp.print();

          printf("\n");

          auto  wave_bin = sp.make_wave_format_binary(g_spec);

            if(wave_bin.size())
            {
              std::string  s(path);

              s += ".wav";

              gbstd::write_to_file(wave_bin.data(),wave_bin.size(),s.data());

              ++n;

              printf("%s -> %s\n",path,s.data());
            }
        }
    }


  printf("%d files were processed\n",n);
#endif


  return 0;
}




