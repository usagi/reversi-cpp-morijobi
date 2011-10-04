#include <iostream>
#include <string>

namespace cpp_morijobi{
  namespace reversi{

    class reversi{
    
      void command_reset(){
        std::cout << "##WARNING## reset is not implemented" << std::endl;
      }
      void command_show() const{
        std::cout << "##WARNING## show is not implemented" << std::endl;
      }
      void command_set(const std::string& x, const std::string& y){
        std::cout << "##WARNING## set "<< x <<" " << y << " is not implemented" << std::endl;
      }
      void command_not_found(const std::string& command) const{
        std::cout << "##ERROR##: command \"" << command << "\" is not found" << std::endl;
      }
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
          else if(command_line.find("reset") == 0)
            command_reset();
          else if(command_line.find("show") == 0)
            command_show();
          else if(command_line.find("set ") == 0)
            command_set(command_line.substr(4,1), command_line.substr(6,1));
          else if(command_line.empty())
            continue;
          else
            command_not_found(command_line.substr(0, command_line.find(" ")));
      }
    };
  }
}


