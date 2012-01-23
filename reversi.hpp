#pragma once

#include <iostream>
#include <string>

#include <list>
#include <array>
#include <memory>
#include <utility>

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
        const auto& active_player = player_ptrs_[turn_ % player_ptrs_.size()];
        bool is_white = bool(turn_ % 2);
        
        active_player->update();
        
        auto next_position = active_player->next_position();
        auto next_stone = stone_type(is_white, next_position);
        auto reversi_stones = check_set_stone(next_stone);
        
        stones_.push_back(pointer_stone_type(new stone_type(std::move(next_stone))));
        ++turn_;
      }
      
      const std::list<pointer_stone_type>
      check_set_stone(const stone_type& next_stone){
        auto result = std::list<pointer_stone_type>();
        // check 8 direction
        for(auto direction: {
          position_type( 0,-1),
          position_type( 1,-1),
          position_type( 1, 0),
          position_type( 1, 1),
          position_type( 0, 1),
          position_type(-1, 1),
          position_type(-1,-1),
          position_type(-1, 1)
        }){
          for(
            auto p = next_stone.position();
            std::min(p.x(), p.y()) < 0 ||
            typename position_type::value_type(board_length)
              <= std::max(p.x(), p.y());
            p += direction
          ){
            auto find_result = std::find_if(
              stones_.begin(), stones_.end(),
              [&](const pointer_stone_type& pstone){
                return pstone->position() == p;
              }
            );
            
            if(
              find_result == stones_.end() ||
              (*find_result)->is_white() == next_stone.is_white()
            )
              break;
            
            result.push_back(*find_result);
            std::cout << "push " << (*find_result)->position() << std::endl;
          }
        }
        
        return result;
      }
      
      void command_show() const{
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
        
        for(auto line: show_buffer){
          for(auto element: line)
            std::cout << element;
          std::cout << std::endl;
        }
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
        else if(check_command("reset"))
          command_reset();
        else if(check_command("next"))
          command_next();
        else if(check_command("show"))
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
        do{
          get_command_line();
          invoke_command();
        }while(is_running);
      }
      
    };
  }
}


