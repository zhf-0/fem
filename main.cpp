#include "tensor.hpp"
#include <iostream>

void TestTensor()
{
#if debug == 1
	std::cout<<"debug"<<std::endl;
#endif
	Mat<int,double> a(2,2,1.0);
	Mat<int,double> b(2,2,2.0);
	/* Mat c; */
	/* c = a*b; */
	/* std::cout<<c<<std::endl; */
	/* Mat d = c ; */ 
	std::cout<<"=========="<<std::endl;
	Mat<int,double> e = std::move(a);
	std::cout<<"the addres:"<<&e<<std::endl;
}

int main()
{
	TestTensor();
	return 0;
}
