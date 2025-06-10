find src/ -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -style=file:./.clang-format -i
find tests/ -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -style=file:./.clang-format -i
