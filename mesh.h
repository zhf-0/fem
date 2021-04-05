#ifndef FEM_H
#define FEM_H
#include "tensor.hpp"

class Mesh
{
public:
	Mesh() = default;
	Mesh(const Mesh& ) = default;
	Mesh(Mesh&& ) = default;
	Mesh& operator=(const Mesh & ) = default;
	Mesh& operator=(Mesh && ) = default;
	virtual ~Mesh(){}

	virtual void InitMesh() = 0;

};

class QuadMesh:public Mesh
{
public:
	QuadMesh():x_min(0.0),x_max(1.0),y_min(0.0),y_max(1.0),x_num(2),y_num(2),num_node(9),num_elem(4),node_cood(9,2),elem_node(4,4),bd_node(8){}
	QuadMesh(double x0,double x1,double y0,double y1,int nx,int ny):x_min(x0),x_max(x1),y_min(y0),y_max(y1),x_num(nx),y_num(ny),num_node((nx+1)*(ny+1)),num_elem(nx*ny),node_cood((nx+1)*(ny+1),2),elem_node(nx*ny,4),bd_node(2*(nx+ny)){}
	QuadMesh(const QuadMesh &) = default;
	QuadMesh(QuadMesh &&) = default;
	QuadMesh& operator=(const QuadMesh &) = default;
	QuadMesh& operator=(QuadMesh &&) = default;
	virtual ~QuadMesh(){}

	void InitMesh();



protected:
	double x_min,x_max;
	double y_min,y_max;
	int x_num,y_num;
	int num_node;
	int num_elem;
public:
	Mat<int,double> node_cood; 
	Mat<int,int> elem_node;
	Vec<int,int> bd_node;
};

#endif
