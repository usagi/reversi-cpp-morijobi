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
        : reversi(&reversi)
        , stones(&reversi.stones())
      {}
      
      virtual ~player(){}
      
      virtual void update() = 0;
      
      virtual const position_type& next_position() const
      { return next_position_; }
      
      virtual void next_position(const position_type& p)
      { next_position_ = p; }
      
    protected:
      const reversi_type* reversi;
      const stones_type* stones;
      position_type next_position_;
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
        typedef random<> random_type;
        typedef std::uniform_int_distribution<> distribution_type;
        //auto rne = random_type::engine();
        auto rnd = distribution_type(0, reversi_type::board_length);
        //auto r = [&](){ return rnd(rne); };
        //auto p = position_type( r(), r() );
        auto p = position_type( 
          random_type::generate(rnd),
          random_type::generate(rnd)
        );
        std::cout << "generate random position: " << p << std::endl;
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
