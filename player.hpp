#pragma once

#include <random>

#include "utility.hpp"

namespace cpp_morijobi{
  namespace reversi{
    
    // base
    template<class reversi_type_>
    class player{
    public:
      typedef reversi_type_ reversi_type;
      typedef
        typename reversi_type::stones_type
        stones_type;
      typedef
        typename stones_type::value_type::element_type::position_type
        position_type;
      
      player(const reversi_type& reversi)
        : reversi_(&reversi)
        , stones_(&reversi.stones())
      {}
      
      virtual ~player(){}
      
      virtual void update() = 0;
      
      virtual const position_type& next_position() const
      { return next_position_; }
      
      virtual void next_position(const position_type& p)
      { next_position_ = p; }
      
    protected:
      const reversi_type* reversi_;
      const stones_type* stones_;
      position_type next_position_;
      
      const reversi_type& reversi() const { return *reversi_; }
      const stones_type& stones() const { return *stones_; }
    };
    
    template<class reversi_type_>
    class computer_player
      : public player<reversi_type_>
    {
    public:
      typedef reversi_type_ reversi_type;
      typedef player<reversi_type> player_type;
      typedef
        typename reversi_type::stones_type
        stones_type;
      typedef
        typename stones_type::value_type::element_type::position_type
        position_type;
      
      computer_player(const reversi_type& reversi)
        : player_type(reversi) 
      {}
      
      virtual void update(){
        std::cout << "computer_player update begin" << std::endl;
        
        auto is_white = this->reversi().player_is_white(this);
        std::cout << "my stone is " << (is_white ? "white":"black") << std::endl;
        
        // lookup around of a stone positions of another player
        auto ps = [&](){
          std::vector<position_type> r;
          for(const auto& s: this->stones())
            if(s->is_white() != is_white){
              const auto& base_position = s->position();
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
                r.push_back(base_position + direction);
              }
            }
          return r;
        }();
        
        typedef random<> random_type;
        typedef std::uniform_int_distribution<> distribution_type;
        auto rnd = distribution_type(0, ps.size() - 1);
        auto p = ps[random_type::generate(rnd)];
        
        std::cout << "generate position: " << p << std::endl;
        next_position( p );
        std::cout << "computer_player update end" << std::endl;
      }
      
    };
    
    template<class reversi_type>
    class user_player
      : public player<reversi_type>
    {
    public:
      typedef player<reversi_type> player_type;
      user_player(const reversi_type& reversi)
        : player_type(reversi)
      { }
      
      virtual void update(){
      
      }
    };
    
  }
}
