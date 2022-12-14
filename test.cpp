#include "safe_vector.hpp"

int main() {
	SafeVector<int> vec = {1,2,3,4,5};
  auto it = vec.begin();
  ++it;
  cout<<*it<<"\n";
  vec.insert(vec.begin(),6);
  cout<<*it;
	return 0;
}