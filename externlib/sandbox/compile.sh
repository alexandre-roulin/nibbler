clear && clang++ -I ../commun/ -fsanitize=address -g3 main.cpp && ./a.out ../sfml/nibbler_sfml.so ../sdl/nibbler_sdl.so
