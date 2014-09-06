#include <vector.h>
#include <hal/console.h>

void a() {
	std::vector<int> *v;
	v=new std::vector<int>(12);
	v->push_back(12);
	v->push_back(13);
	v->erase(0);
	hal::cout<<v->at(0)<<hal::endl;
}
