#include <iostream>
#include <string>

namespace cpp_morijobi{
  namespace reversi{
    class reversi{
      void command_echo(const std::string& v) const{
        std::cout << v << std::endl;
      }
      void get_command_line(std::string& command_line) const{
        std::cout << "> ";
        std::getline(std::cin, command_line);
      }
    public:
      void run(){
        for(std::string command_line; command_line != "exit"; get_command_line(command_line))
          if(command_line.find("echo ") == 0)
            command_echo(command_line.substr(5));
      }
    };
  }
}

main(){
  cpp_morijobi::reversi::reversi().run();
}

