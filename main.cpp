#include "tensor.hpp"
#include "mesh.h"
#include <iostream>
#include "format.h"

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
	std::cout<<e<<std::endl;

	Mat<int,double> d(4,2,1.0);
	std::cout<<d<<std::endl;
	d.at(2,1) = 3.2;
	std::cout<<d<<std::endl;
}

void TestFormat()
{
	std::string res;
	int a =12,b=1;
	res = fmt::format("{},{}",a,b);
	std::cout<<res<<std::endl;
}

void TestMesh()
{
	QuadMesh m(0.0,1.0,0.0,1.0,4,3);
	m.InitMesh();
	m.DisplayNodeCood();
	m.DisplayElemNode();
	m.DisplayEdgeNode();
	m.DisplayBdNode();
	m.DisplayNode2Elem();
	std::cout<<"======================================="<<std::endl;
	TriMesh m1(0.0,1.0,0.0,1.0,4,3);
	m1.InitMesh();
	m1.DisplayNodeCood();
	m1.DisplayElemNode();
	m1.DisplayEdgeNode();
	m1.DisplayBdNode();
	m1.DisplayNode2Elem();
}

int main()
{
	/* TestTensor(); */
	/* TestFormat(); */
	TestMesh();
	return 0;
}
