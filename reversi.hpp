#include <iostream>
#include <string>

#include "utility.hpp"

namespace cpp_morijobi{
  namespace reversi{

    class reversi{

      bool is_running;
      std::vector<std::string> command_and_parameters;

      void command_exit(){
        is_running = false;
      }
      void command_reset(){
        std::cout << "##WARNING## reset is not implemented" << std::endl;
      }
      void command_show() const{
        std::cout << "##WARNING## show is not implemented" << std::endl;
      }
      void command_set(){
        std::cout << "##WARNING## set "<< get_command_parameter(0) <<" " << get_command_parameter(1) << " is not implemented" << std::endl;
      }
      void command_not_found() const{
        std::cout << "##ERROR##: command \"" << get_command() << "\" is not found" << std::endl;
      }
      void command_echo() const{
        std::cout << get_command_parameter(0) << std::endl;
      }

      inline const std::string& get_command() const{
        return command_and_parameters[0];
      }

      inline const std::string& get_command_parameter(size_t n) const{
        return command_and_parameters[n+1];
      }

      inline const bool check_command(const std::string& check_command){
        return get_command() == check_command;
      }

      inline const bool is_command_empty(){
        return command_and_parameters.empty() || get_command().empty();
      }

      void invoke_command(){
        if(is_command_empty())
          return;
        else if(check_command("exit"))
          command_exit();
        else if(check_command("echo"))
          command_echo();
        else if(check_command("reset"))
          command_reset();
        else if(check_command("show"))
          command_show();
        else if(check_command("set "))
          command_set();
        else
          command_not_found();
      }

      void get_command_line(){
        std::cout << "> ";
        std::string command_line;
        std::getline(std::cin, command_line);
        command_and_parameters = split_to_vector(command_line);
      }
    
    public:
      void run(){
        is_running = true;
        do{
          get_command_line();
          invoke_command();
        }while(is_running);
      }
    };
  }
}


