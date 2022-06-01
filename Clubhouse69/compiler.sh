#!/bin/zsh
compile_file=${1:-main.cpp}

rm ./clangcompiled
printf '%b\n' "\033[1moutput deleted.\033[0m"
clang++ -o clangcompiled "$compile_file" -std=c++11 -stdlib=libc++ -lglfw3 -lGLEW -framework CoreVideo -framework Cocoa -framework OpenGL -framework IOKit -framework Carbon
printf '%b\n' "\033[1mfile compiled.\033[0m"
./clangcompiled
echo $?
