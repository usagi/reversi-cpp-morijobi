#include <iostream>
#include <string>

main(){

  for(std::string input_command_line = ""; input_command_line != "exit"; std::cin >> input_command_line)
    std::cout << input_command_line;

}

