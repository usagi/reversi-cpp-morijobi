#include <vector>
#include <radom>

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

  template<class E = std::mt19937_64, S = 8>
  class random{
  public:
	typedef E engine_type;
	static const size_t seed_size = S;

	static const random& instance(){
		static random i = random();
		return i;
	}

	const engine_type& engine() const
	{ return e; }

  private:
	engine_type e;
	random(){ generate_random_engine(); }

	void generate_randome_engine(){
		typedef std::random_device device_type;

		auto s = [seed_size](){
		device_type r;
		std::vector<device_type::result_type> i(seed_size);
		std::generate(i.begin(), i.end(), std::ref(r));
		return std::seed_seq(i.begin(), i.end());
		}();

		e = engine_type(s);
	}
  }
}


