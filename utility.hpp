#include <vector>

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
}


