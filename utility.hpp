#pragma once

#include <vector>
#include <random>
#include <functional>
#include <algorithm>

namespace cpp_morijobi{

  template<class S>
  std::vector<S> split_to_vector(const S& s){
    std::vector<S> r;
    size_t a = 0, b;
    do{
      b = s.find(" ", a);
      r.push_back(s.substr(a, b-a));
      a = b+1;
    }while(b != std::string::npos);
    return r;
  }

  template<class E = std::mt19937_64, size_t S = 8>
  class random{
  public:
	  typedef random<E, S> this_type;
	  typedef E engine_type;
	  static const size_t seed_size = S;

	  inline static const random& instance(){
	  	static random i = random();
	  	return i;
	  }

	  inline static const engine_type& engine()
	  { return instance().e; }

	  template<class distribution_type>
	  inline static const typename distribution_type::result_type generate(const distribution_type& d)
	  { return d(engine()); }

  private:
	  engine_type e;
	  random(){ generate_random_engine(); }

	  inline void generate_random_engine(){
		  typedef std::random_device device_type;

  		auto s = [](){
	  		device_type r;
		  	std::vector<device_type::result_type> i(this_type::seed_size);
			  std::generate(i.begin(), i.end(), std::ref(r));
		  	return std::seed_seq(i.begin(), i.end());
		  }();

		  e = engine_type(s);
	  }
  };

}


