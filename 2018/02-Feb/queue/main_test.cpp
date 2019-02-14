#include <iostream>
#include "queue.h"

using namespace bcn_cpp;

int main()
{
	queue<int> qu;
	queue<int> as;


	qu.push(10);
	qu.push(11);
//	if(as != qu)
//					return -1;

	as = qu;
	qu.pop();
	qu.push(12);
	qu.emplace(13);
	qu.push(14);

	std::cout << qu.front() << '\n';	
	std::cout << qu.back() << '\n';
	qu.pop();
	std::cout << qu.front() << '\n';	
	qu.pop();
	std::cout << qu.front() << '\n';	
	std::cout << as.front() << '\n';	



	return 0;
};

