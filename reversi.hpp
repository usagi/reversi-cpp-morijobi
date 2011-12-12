#include <iostream>
#include <string>

#include <list>
#include <array>
#include <memory>

#include "utility.hpp"
#include "misc.hpp"
#include "stone.hpp"

namespace cpp_morijobi{
  namespace reversi{

    template<size_t L = 6, class P = position<uint_fast8_t>, class S = std::string>
    class reversi{
      static const size_t board_length = L;
      typedef P position_type;
      typedef stone<position_type> stone_type;
      typedef S string_type;
      typedef typename string_type::value_type charactor_type;
      typedef std::array<std::array<string_type, board_length> ,board_length> show_buffer_type;
      typedef std::shared_ptr<stone_type> pointer_stone_type;

    public:
    	typedef std::list<pointer_stone_type> stones_type;
    protected:

      bool is_running;
      std::vector<string_type> command_and_parameters;

      stones_type stones;

      void command_exit(){
        is_running = false;
      }
      void command_reset(){
        std::cout << "##WARNING## reset is not implemented" << std::endl;
      }
      void command_show() const{
        const string_type show_buffer_charactor_space = "＊";
        const string_type show_buffer_charactor_stone_black = "○";
        const string_type show_buffer_charactor_stone_white = "●";
        
        auto show_buffer = [&](){
          show_buffer_type r;
          for(auto& line: r) for(auto& element: line)
            element = show_buffer_charactor_space;
          return r;
        }();

        for(auto stone: stones){
          auto position = stone->position();
          show_buffer[position.y()][position.x()] = stone->is_white()
            ? show_buffer_charactor_stone_white
            : show_buffer_charactor_stone_black;
        }

        for(auto line: show_buffer){
          for(auto element: line)
            std::cout << element;
          std::cout << std::endl;
        }
      }
      void command_set(){
        // tmp
        // 本来はここでパラメータの文字列の値から座標として内部で使う数値の値を得るのだが、
        // 諸事情によりそれはもう少し後で実装を教える。
        stones.push_back(pointer_stone_type(new stone_type(true, position_type(board_length >> 1,  board_length >> 1))));
        stones.push_back(pointer_stone_type(new stone_type(true, position_type((board_length >> 1) - 1,  (board_length >> 1) - 1))));
        stones.push_back(pointer_stone_type(new stone_type(false, position_type(board_length >> 1,  (board_length >> 1) - 1))));
        stones.push_back(pointer_stone_type(new stone_type(false, position_type((board_length >> 1) - 1,  board_length >> 1) )));
      }
      void command_not_found() const{
        std::cout << "##ERROR##: command \"" << get_command() << "\" is not found" << std::endl;
      }
      void command_echo() const{
        if(is_not_exist_command_parameter(1)){
          std::cerr << "##ERROR##: echo command required one parameter." << std::endl;
          return;
        }
        std::cout << get_command_parameter(0) << std::endl;
      }

      inline const string_type& get_command() const{
        return command_and_parameters[0];
      }

      inline const string_type& get_command_parameter(const size_t n) const{
        return command_and_parameters[n+1];
      }

      inline const size_t count_command_parameters() const{
        return command_and_parameters.size() - 1;
      }

      inline const bool is_not_exist_command_parameter(const size_t n) const{
        return count_command_parameters() < n;
      }

      inline const bool check_command(const string_type& check_command){
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
        else if(check_command("set"))
          command_set();
        else
          command_not_found();
      }

      void get_command_line(){
        std::cout << "> ";
        string_type command_line;
        std::getline(std::cin, command_line);
        command_and_parameters = split_to_vector(command_line);
      }
    
    public:
      reversi()
        : is_running(false)
      {}

      void run(){
        if(is_running){
          std::cerr << "##ERROR##: reversi is running." << std::endl;
          return;
        }
        is_running = true;
        do{
          get_command_line();
          invoke_command();
        }while(is_running);
      }
    };
  }
}


