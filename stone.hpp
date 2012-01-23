#pragma once

#include <vector>

namespace cpp_morijobi{
  namespace reversi{
    
    template<class P>
    class stone{
    public:
      typedef P position_type;
      typedef stone<position_type> this_type;
      
      inline const position_type& position() const
      { return position_; }
      
      const bool is_white() const { return is_white_; }
      stone()
        :is_white_(false)
      {}
      
      inline void reverse() { is_white_ = !is_white_; }
      
      stone(const bool white)
        :is_white_(white)
      {}
      
      stone(const bool white, const position_type& p)
        :is_white_(white)
        ,position_(p)
      {}
      
      friend std::ostream& operator<<(std::ostream& o, const this_type& t){
        o << "[ " << t.is_white_ << " , " << t.position_ << " ]";
        return o;
      }
      
    protected:
      bool is_white_;
      position_type position_;
    };
    
  }
}


