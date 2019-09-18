#include"libsns/sns_sha256.hpp"


using namespace sns;




int
main(int  argc, char**  argv)
{
  --argc;
  ++argv;

  sha256_hash_generator  gen;

    while(argc--)
    {
      auto  path = *argv++;

      auto  f = fopen(path,"rb");

        if(f)
        {
          gen.reset();

            for(;;)
            {
              auto  c = fgetc(f);

                if(feof(f))
                {
                  break;
                }


              gen.push(static_cast<uint8_t>(c));
            }


          sha256_string  s(gen.finish());

          printf("%s  %s\n",s.get_string().data(),path);

          fclose(f);
        }
    }


  return 0;
}




