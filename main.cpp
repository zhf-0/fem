#include "tensor.hpp"
#include "mesh.h"
#include "format.h"
#include <iostream>
#include <chrono>

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

void TestRepeatedSpaseMat()
{
	SpaCOO<int, int> coo(3,3,6);
	coo.row_vec[0] = 0;coo.row_vec[1] = 0;coo.row_vec[2] = 1;coo.row_vec[3] = 1;
	coo.row_vec[4] = 1;coo.row_vec[5] = 2;

	coo.col_vec[0] = 0;coo.col_vec[1] = 0;coo.col_vec[2] = 1;coo.col_vec[3] = 1;
	coo.col_vec[4] = 1;coo.col_vec[5] = 0;

	coo.val[0] = 1;coo.val[1] = 1;coo.val[2] = 1;coo.val[3] = 1;coo.val[4] = 1;
	coo.val[5] = 1;
	coo.PrintPartialMat(6);

	SpaCSR<int,int> csr;
	csr.RepeatedCOO2CSR(coo);
	csr.PrintPartialMat(3);
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
	/* std::cout<<"======================================="<<std::endl; */
	/* TriMesh m1(0.0,1.0,0.0,1.0,4,3); */
	/* m1.InitMesh(); */
	/* m1.DisplayNodeCood(); */
	/* m1.DisplayElemNode(); */
	/* m1.DisplayEdgeNode(); */
	/* m1.DisplayBdNode(); */
	/* m1.DisplayNode2Elem(); */
}

void TestMeshSpeed()
{
	auto begin = std::chrono::system_clock::now();
	TriMesh m1(0.0,1.0,0.0,1.0,1000,1000);
	m1.InitMesh();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = end - begin;
	std::cout<<"time = "<<elapsed.count()<<std::endl;
	/* m1.elem_node.Write2file("mat.txt"); */
}


int main()
{
	/* TestTensor(); */
	/* TestRepeatedSpaseMat(); */
	/* TestFormat(); */
	TestMesh();
	/* TestMeshSpeed(); */
	return 0;
}
