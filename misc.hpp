namespace cpp_morijobi{
	namespace reversi{

		template<class T>
		class position{
		public:
			typedef T value_type;
      typedef position<value_type> this_type;
			position(): x_(0), y_(0){}
			position(value_type x, value_type y): x_(x), y_(y){}
      inline const value_type& x(){ return x_; }
      inline const value_type& y(){ return y_; }
		protected:
			value_type x_, y_;
		};

	}
}
