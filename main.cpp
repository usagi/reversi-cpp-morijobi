#include <iostream>
#include <exception>
#include "reversi.hpp"

int main() try {
  cpp_morijobi::reversi::reversi<>().run();
}catch(const std::exception& e){
  std::cerr << e.what();
}

