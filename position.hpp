#pragma once

#include <ostream>

namespace cpp_morijobi{
  namespace reversi{
    
    template<class T>
    class position{
    public:
      typedef T value_type;
      typedef position<value_type> this_type;
      
      position(): x_(0), y_(0){}
      position(value_type x, value_type y): x_(x), y_(y){}
      
      inline const value_type& x() const { return x_; }
      inline const value_type& y() const { return y_; }
      
      const this_type operator+=(const this_type& t){
        x_+=t.x_;
        y_+=t.y_;
        return *this;
      }
      
      friend const this_type operator+(const this_type& a, const this_type& b)
      { return this_type( a.x_ + b.x_, a.y_ + b.y_ ); }
      
      friend const bool operator==(const this_type& a, const this_type& b)
      { return a.x_ == b.x_ && a.y_ == b.y_; }
      
      friend std::ostream& operator<<(std::ostream& o, const this_type& t){
        o << "( " << t.x_ << " , " << t.y_ << " )";
        return o;
      }
    protected:
      value_type x_, y_;
    };
    
  }
}
