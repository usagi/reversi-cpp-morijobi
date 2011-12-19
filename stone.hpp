#pragma once

#include <vector>

namespace cpp_morijobi{
	namespace reversi{
	
    template<class P>
		class stone{
		public:
      typedef P position_type;
      inline const position_type& position() const{ return position_; }
      const bool is_white() const { return is_white_; }
      stone()
        :is_white_(false)
      {}
      stone(const bool white)
        :is_white_(white)
      {}
      stone(const bool white, const position_type& p)
        :is_white_(white)
        ,position_(p)
      {}
		protected:
			position_type position_;
      bool is_white_;
		};

	}
}


