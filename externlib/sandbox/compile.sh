#!/usr/bin/env bash
clear && clang++ -I ../../incs/ -I ../commun/ -fsanitize=address -g3 main.cpp && ./a.out ../display_sdl/display_sdl.so
