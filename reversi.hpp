#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <list>
#include <array>
#include <memory>
#include <utility>
#include <stdexcept>

#include "utility.hpp"
#include "position.hpp"
#include "stone.hpp"
#include "player.hpp"

namespace cpp_morijobi{
  namespace reversi{
    
    template<
      size_t L = 6,
      class P = position<int_fast64_t>,
      class S = std::string
    >
    class reversi{
    public:
      typedef reversi<L, P, S> this_type;
      static const size_t board_length = L;
      typedef P position_type;
      typedef stone<position_type> stone_type;
      typedef S string_type;
      typedef typename string_type::value_type charactor_type;
      typedef
        std::array<std::array<string_type, board_length> ,board_length>
        show_buffer_type;
      typedef std::shared_ptr<stone_type> pointer_stone_type;
      typedef player<this_type> player_type;
      typedef computer_player<this_type> computer_player_type;
      typedef user_player<this_type> user_player_type;
      typedef std::unique_ptr<player_type> player_ptr_type;
      typedef std::vector<player_ptr_type> player_ptrs_type;
      typedef std::list<pointer_stone_type> stones_type;
      
      const stones_type& stones() const
      { return stones_; }
      
      const size_t turn() const
      { return turn_; }
      
      const bool player_is_white(const player_type* p) const {
        for(size_t n = 0; n < player_ptrs_.size(); ++n)
          if(player_ptrs_[n].get() == p)
            return bool(n % 2);
        throw std::logic_error("player_is_white; unknown player");
      }
      
    protected:
      bool is_running;
      std::vector<string_type> command_and_parameters;
      stones_type stones_;
      player_ptrs_type player_ptrs_;
      size_t turn_;
      
      void command_exit(){
        is_running = false;
      }
      
      void command_reset(){
        reset_players();
        reset_board();
        reset_turn();
        std::cout << "reset done" << std::endl;
      }
      
      void reset_turn(){
        turn_ = 0;
        std::cout << "reset turn" << std::endl;
      }
      
      void reset_board(){
        stones_.clear();
        std::cout << "clear stones" << std::endl;
        // center right bottom
        stones_.push_back(pointer_stone_type(new stone_type(
          true,
          position_type(board_length >> 1,  board_length >> 1
        ))));
        // center left top
        stones_.push_back(pointer_stone_type(new stone_type(
          true,
          position_type((board_length >> 1) - 1,  (board_length >> 1) - 1
        ))));
        // center right top
        stones_.push_back(pointer_stone_type(new stone_type(
          false,
          position_type(board_length >> 1,  (board_length >> 1) - 1
        ))));
        // center left bottom
        stones_.push_back(pointer_stone_type(new stone_type(
          false, position_type((board_length >> 1) - 1,  board_length >> 1
        ))));
        std::cout << "reset stones" << std::endl;
      }
      
      void reset_players(){
        player_ptrs_.clear();
          std::cout << "clear players" << std::endl;
          for(auto n = 2; n; --n){
            player_ptrs_.push_back(player_ptr_type(
              new computer_player_type(*this)
            ));
            std::cout
              << "add computer_player["
              << player_ptrs_.size() - 1
              << "]"
              << std::endl;
          }
      }
      
      void command_next(){
        std::cout << "next begin" << std::endl;
        
        const auto& active_player = player_ptrs_[turn_ % player_ptrs_.size()];
        bool is_white = player_is_white(active_player.get());
        
        active_player->update();
        
        auto next_position = active_player->next_position();
        auto next_stone = stone_type(is_white, next_position);
        auto reversible_stones = check_set_stone(next_stone);
        
        if(reversible_stones.empty()){
          std::cout << "fail, it's not placeable." << std::endl;
        }else{
          std::cout << "succeed, it's placeable." << std::endl;
          stones_.push_back(pointer_stone_type(new stone_type(std::move(next_stone))));
          for(auto s: reversible_stones)
            s->reverse();
        }
        
        // auto show
        command_show();
        
        check_game_end();
        
        ++turn_;
        
        std::cout << "next end" << std::endl;
      }
      
      void check_game_end(){
        size_t count_of_white = 0, count_of_black = 0;
        for(const auto& s: stones_)
          ++(s->is_white() ? count_of_white : count_of_black);
        if(
          count_of_white == 0 ||
          count_of_black == 0 ||
          stones_.size() == board_length * board_length
        ){
          std::cout
            << "GAME END\n"
            << "black: " << count_of_black << "\n"
            << "white: " << count_of_white << "\n";
          if(count_of_black == count_of_white)
            std::cout << "draw" << std::endl;
          else
            std::cout
              << "the winner is "
              << ((count_of_black > count_of_white)? "black": "white")
              << "\n\n" << std::endl;
          
          // auto reset
          command_reset();
        }
          
      }
      
      const std::list<pointer_stone_type>
      check_set_stone(const stone_type& next_stone){
        std::cout
          << "check_set_stone begin\n"
          << "next_stone is " << next_stone << std::endl;
        
        typedef std::list<pointer_stone_type> result_type;
        auto result = result_type();
        // check 8 direction
        for(auto direction: {
          position_type( 0,-1),
          position_type( 1,-1),
          position_type( 1, 0),
          position_type( 1, 1),
          position_type( 0, 1),
          position_type(-1, 1),
          position_type(-1, 0),
          position_type(-1,-1)
        }){
          auto result_buffer = result_type();
          for(
            auto p = next_stone.position() + direction;
            0 <= std::min(p.x(), p.y()) &&
            std::max(p.x(), p.y())
              < typename position_type::value_type(board_length);
            p += direction
          ){
            std::cout << "check of " << p << " ... ";
            auto find_result = std::find_if(
              stones_.begin(), stones_.end(),
              [&](const pointer_stone_type& pstone){
                return pstone->position() == p;
              }
            );
            
            if( find_result == stones_.end() ){
              std::cout << "empty" << std::endl;
              break;
            }
            
            if( (*find_result)->is_white() == next_stone.is_white() ){
              std::cout << "my stone" << std::endl;
              std::move(
                result_buffer.begin(), result_buffer.end(),
                std::back_inserter(result)
              );
              break;
            }
            
            std::cout << "another player stone" << std::endl;
            result_buffer.push_back(*find_result);
          }
        }
        
        if(result.empty())
          std::cout << "reversible stone is not found" << std::endl;
        else if(result.size() == 1)
          std::cout << "reversible stone is " << **(result.begin()) << std::endl;
        else{
          std::cout << "reversible stones are\n";
          for(const auto& v: result)
            std::cout << " " << v << std::endl;
        }
        
        std::cout
          << "check_set_stone end"
          << std::endl;
        
        return result;
      }
      
      void command_show() const{
        std::cout
          << "show begin ( turn is " << turn_  << " )\n"
          << std::endl;
        
        const string_type show_buffer_charactor_space = "* ";
        const string_type show_buffer_charactor_stone_black = "B ";
        const string_type show_buffer_charactor_stone_white = "W ";
        
        auto show_buffer = [&](){
          show_buffer_type r;
          for(auto& line: r) for(auto& element: line)
            element = show_buffer_charactor_space;
          return r;
        }();
        
        for(auto stone: stones_){
          auto position = stone->position();
          show_buffer[position.y()][position.x()] = stone->is_white()
            ? show_buffer_charactor_stone_white
            : show_buffer_charactor_stone_black;
        }
        
        {
          size_t n = 0;
          auto left_area = std::string("     ");
          std::cout << left_area;
          while(n < board_length)
            std::cout << n++ << " ";
          std::cout << std::endl;
          n = 0;
          for(auto line: show_buffer){
            std::cout
              << std::setw(left_area.size() - 1)
              << std::right
              << n++ << " ";
            for(auto element: line)
              std::cout << element;
            std::cout << std::endl;
          }
        }
        
        std::cout << "\nshow end" << std::endl;
      }
      
      void command_not_found() const{
        std::cout
          << "##ERROR##: command \""
          << get_command()
          << "\" is not found"
          << std::endl;
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
        else if(check_command("exit") || check_command("e"))
          command_exit();
        else if(check_command("reset") || check_command("r"))
          command_reset();
        else if(check_command("next") || check_command("n"))
          command_next();
        else if(check_command("show") || check_command("s"))
          command_show();
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
        
        // auto reset at the first time
        command_reset();
        
        do{
          get_command_line();
          invoke_command();
        }while(is_running);
        
      }
      
    };
  }
}


