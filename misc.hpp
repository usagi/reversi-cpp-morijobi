#pragma once

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
      const this_type operator+=(const this_type& t)
      {
        x_+=t.x_;
        y_+=t.y_;
        return *this;
      }
		protected:
			value_type x_, y_;
		};

    template<class t>
    const position<t> operator+(const position<t> a, const position<t> b)
    { return position<t>(a.x()+b.x(), a.y()+b.y()); }
  }
}
